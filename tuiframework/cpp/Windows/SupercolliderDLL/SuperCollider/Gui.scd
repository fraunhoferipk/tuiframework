

(
var hosvdRnk = 0, numParams, numParamss, paramNamess, paramLimitss, paramUnitss;
var communicatorPort = 57130;
var communicatorNetAddr = NetAddr.new("127.0.0.1", communicatorPort);
var mainwindow, tuningwindow;
var windowHeightTuning, windowWidthTuning, sliderHeight, sliderWidth;
var windowHeightMain, windowWidthMain;
var numGradations, numDimensions, numDimensionsTunable, dimNames, dimNamesTunable, tunabilityArray, tunableDimensions;
var selectedGradationsInxes;
var selectedDimensionInx = 0, selectedGradationInx = 0, selectedGradationWithInx = 0;
var hosvdValues, paramValues, hosvdSliders, paramSliders;
var outfile;
var maindir = "/Users/csapoadam/Documents/Development/SpiralDiscoveryMethod/SuperCollider";

var mainWindowCode, tuningWindowCode, calculateWindowDimensions, setParamSliderCode, setHosvdSliderCode;

~initAudioChannelGUI = {
	|dimensionNamesTunable, dimensionNames, tunableOrNotArray,
	numberGradations, hosvdRank, parameterNames, parameterLimits, parameterUnits|

	dimNamesTunable = dimensionNamesTunable;
	numDimensionsTunable = dimNamesTunable.size;
	dimNames = dimensionNames;
	numDimensions = dimNames.size;
	tunabilityArray = tunableOrNotArray;
	selectedDimensionInx = 0;
	selectedGradationInx = 0;
	numGradations = numberGradations;
	selectedGradationsInxes = Array.series(numDimensions, 0, 0);
	hosvdRnk = hosvdRank;
	hosvdValues = Array.series(hosvdRnk, 0, 0);
	paramValues = Array.series(hosvdRnk, 0, 0);
	paramNamess = parameterNames;
	numParamss = (0..paramNamess.size-1).collect({|i| paramNamess[i].size;});
	paramLimitss = parameterLimits;
	paramUnitss = parameterUnits;
	calculateWindowDimensions.value;
};

~setParamSlider = {|sliderinx, val|
	{setParamSliderCode.value(sliderinx, val);}.defer;
};

setParamSliderCode = {|sliderinx, val|
	"setting slider...".postln;
	paramSliders[sliderinx].value_(val);
};

~setHosvdSlider = {|sliderinx, val|
	{setHosvdSliderCode.value(sliderinx, val);}.defer;
};

setHosvdSliderCode = {|sliderinx, val|
	"setting hosvd slider...".postln;
	hosvdSliders[sliderinx].value_(val);
};


calculateWindowDimensions = {

	numParams = numParamss[selectedDimensionInx];

	windowHeightTuning = 600;
	windowWidthTuning = 850;
	sliderHeight = (windowHeightTuning*2 - 50) / (hosvdRnk + numParams + 2);
	sliderWidth = 400;

	if(sliderHeight > 30,
		{
			sliderHeight = 30;
			windowHeightTuning = (sliderHeight * (hosvdRnk + numParams + 2) + 30) / 2;
		});

	windowHeightMain = 500 + (35*numDimensions);
	windowWidthMain = 500;

};

~mainWindow = {
	{mainWindowCode.value;}.defer;
};

mainWindowCode = {
	var popupmenusDimensions, popupmenusGradations;
	var popupmenuTuneDimension, popupmenuTuneGradation, popupmenuWith;
	var playButton, tuneButton, hooiButton, saveButton, exitButton, copyButton, swapButton, insertButton;
	mainwindow = Window.new("Main Window", Rect(200,200,windowWidthMain,windowHeightMain)).front;

	outfile = File(maindir++"/tuningresults.txt", "w");
	File.getcwd.asString.postln;

	popupmenusDimensions = Array.series(numDimensions,0,0);

	popupmenusDimensions.do({|elem, inx|
		popupmenusDimensions[inx] = EZPopUpMenu.new(
			mainwindow,
			Rect(
				20,
				20+(inx*(windowHeightMain-85)/(4*numDimensions)),
				windowWidthMain - 40,
				(windowHeightMain-85)/(4*numDimensions)
			),
			dimNames[inx],
			(0..numGradations[inx]-1).collect({|i| ("gradation"++i.asString)->{|i| i.value;}});,
			{|item| (tunabilityArray[inx]==1).if(
				{
					selectedGradationsInxes[inx] = item.value;
					selectedGradationInx = selectedGradationsInxes[selectedDimensionInx];
					("Selected dimension "++selectedDimensionInx).postln;
					("Selected gradation "++selectedGradationInx).postln;
					"".postln;
				},{
					selectedGradationsInxes[inx] = item.value;}
			)
			}
		);
		popupmenusDimensions[inx].setColors(
			Color(0.8, 0.2, 0.1, 0.8),
			Color.black,
			Color.white,
			Color(0.6,0.9,0.1,0.95),
			Color.white);
	});

	popupmenuTuneDimension = EZPopUpMenu.new(
		mainwindow,
		Rect(
			20,
			((windowHeightMain-85)/4) + ((windowHeightMain-85)/16)  + 20 + 15,
			windowWidthMain - 40,
			2*(windowHeightMain-85)/16
		),
		"Play/Tune Dimension",
		dimNamesTunable.collect({|name, i| name->{|name| i.value;}});,
		{
			|item|
			var inx = tunableDimensions[item.value];
			popupmenuWith.items = (0..numGradations[inx]-1).collect({|n| ("gradation"++n.asString)});
			selectedDimensionInx = tunableDimensions[item.value];
			selectedGradationInx = selectedGradationsInxes[selectedDimensionInx];
			selectedGradationWithInx = 0;
			("selected dimension "++selectedDimensionInx).postln;
			("Selected gradation "++selectedGradationInx).postln;
			"".postln;
			calculateWindowDimensions.value;
		},
		labelWidth:(windowWidthMain-40)/3);


	playButton = Button(
		mainwindow,
		Rect(
			40,
			(2*(windowHeightMain-85)/4) + ((windowHeightMain-85)/64) + 20 + (2*15),
			(windowWidthMain - 100) / 3,
			3*(windowHeightMain-85)/32
		)
	).states_([["Play", Color.black, Color.yellow]]);

	tuneButton = Button(
		mainwindow,
		Rect(
			40 + 10 + ((windowWidthMain - 100) / 3),
			(2*(windowHeightMain-85)/4) + ((windowHeightMain-85)/64) + 20 + (2*15),
			(windowWidthMain - 100) / 3,
			3*(windowHeightMain-85)/32
		)
	).states_([["Tune", Color.black, Color(0.6,0.9,0.1,0.95)]]);


	hooiButton = Button(
		mainwindow,
		Rect(
			40 + 20 + (2*((windowWidthMain - 100) / 3)),
			(2*(windowHeightMain-85)/4) + ((windowHeightMain-85)/64) + 20 + (2*15),
			(windowWidthMain - 100) / 3,
			3*(windowHeightMain-85)/32
		)
	).states_([["Hooi", Color.black, Color(0.6,0.9,0.1,0.95)]]);


	saveButton = Button(
		mainwindow,
		Rect(40,
			(2*(windowHeightMain-85)/4) + ((windowHeightMain-85)/8) + ((windowHeightMain-85)/64) + 20 + (2*15),
			(windowWidthMain - 90) / 2,
			3*(windowHeightMain-85)/32
		)
	).states_([["Save channel", Color.black, Color(0.6,0.9,0.1,0.95)]]);

	exitButton = Button(
		mainwindow,
		Rect(
			40 + 10 + ((windowWidthMain - 90) / 2),
			(2*(windowHeightMain-85)/4) + ((windowHeightMain-85)/8) + ((windowHeightMain-85)/64) + 20 + (2*15),
			(windowWidthMain - 90) / 2,
			3*(windowHeightMain-85)/32
		)
	).states_([["Exit", Color.black, Color(0.8, 0.2, 0.1, 0.8)]]);


	copyButton = Button(
		mainwindow,
		Rect(
			40,
			(3*(windowHeightMain-85)/4) + ((windowHeightMain-85)/64) + 20 + (3*15),
			0.8*(windowWidthMain - 90) / 2.0,
			3*(windowHeightMain-85)/32
		)
	).states_([["Copy to...", Color.black, Color(0.6,0.9,0.1,0.95)]]);

	swapButton = Button(
		mainwindow,
		Rect(
			40,
			(3*(windowHeightMain-85)/4) + ((windowHeightMain - 85)/8) + ((windowHeightMain-85)/64) + 20 + (3*15),
			0.8*(windowWidthMain - 90) / 2.0,
			3*(windowHeightMain-85)/32
		)
	).states_([["Swap with...", Color.black, Color(0.6,0.9,0.1,0.95)]]);


	/*insertButton = Button(
		mainwindow,
		Rect(40,
			115 + 45*numDimensions,
			windowWidthMain / 3.35,
			20)).states_([["Insert gradation after...", Color.black]]);*/


	//////////////////////
	/// BUTTON ACTIONS ///
	//////////////////////

	playButton.action_({
		var strHosvdValues = "", strParamValues = "", gradstr = "";

		selectedGradationsInxes.do({|item, inx|
			gradstr = gradstr++item.asString++";"});

		gradstr.postln;
		selectedDimensionInx.postln;
		communicatorNetAddr.sendMsg("/playSound",selectedDimensionInx, gradstr);

	});

	tuneButton.action_({var gradstr;
		gradstr = "";
		selectedGradationsInxes.do({|item, inx|
			gradstr = gradstr++item.asString++";"});
		communicatorNetAddr.sendMsg("/getWeights",
			selectedDimensionInx,
			numDimensions,
			gradstr
		);
		//do not call tuningWindowCode because weights must still be received through OSCFunc
		//tuningWindowCode.value;});
	});

	hooiButton.action_({
		communicatorNetAddr.sendMsg("/hooiDimension",
			selectedDimensionInx
		);

	});

	saveButton.action_({
		"saving channel...".postln;
		Dialog.savePanel({ arg path;
			communicatorNetAddr.sendMsg("/saveChannel", path);
		},
		{
			"cancelled".postln;
		});
	});

	exitButton.action_({
		communicatorNetAddr.sendMsg("/quitProgram");
		outfile.close;
		mainwindow.close;
	});

	copyButton.action_({
		communicatorNetAddr.sendMsg("/copyGradation", selectedDimensionInx, selectedGradationInx, selectedGradationWithInx);
	});

	swapButton.action_({
		communicatorNetAddr.sendMsg("/swapGradations", selectedDimensionInx, selectedGradationInx, selectedGradationWithInx);
	});

	/*insertButton.action_({
		"insert new gradation...".postln;
		Dialog.savePanel({ arg path;
			communicatorNetAddr.sendMsg("/insertGradation", selectedDimensionInx, selectedGradationWithInx, path);
		},
		{
			"cancelled".postln;
		});
	});*/


	popupmenuWith = EZPopUpMenu.new(
		mainwindow,
		Rect(
			40 + 10 + (0.8*(windowWidthMain - 90) / 2.0),
			(3*(windowHeightMain-85)/4) + ((windowHeightMain-85)/16)  + 20 + (3*15),
			(1.2*(windowWidthMain - 90) / 2.0),
			2*(windowHeightMain-85)/16
		),
		"",
		(0..numGradations[0]-1).collect({|n|
			("gradation"++n.asString)}),
		{|item| selectedGradationWithInx = item.value;},
		labelWidth:0);

};


~tuningWindow = {
	{tuningWindowCode.value;}.defer;
};

tuningWindowCode = {
	var sliderValues, paramNameDisplays, playButton, stopButton, keepButton, saveButton, sampleButton, ezSlider;
	var paramNames = paramNamess[selectedDimensionInx];
	var paramLimits = paramLimitss[selectedDimensionInx];
	var paramUnits = paramUnitss[selectedDimensionInx];

	var slidersHalf = (hosvdRnk + numParams) / 2;
	tuningwindow = Window.new(
		"Tuning Window",
		Rect(200,
			200,
			windowWidthTuning,
			windowHeightTuning)).front;

	hosvdSliders = Array.new(hosvdRnk);
	paramSliders = Array.new(numParams);
	~parameterValues = Array.new(hosvdRnk + numParams);

	hosvdRnk.do{
		arg i;
		var col;

		if(i < slidersHalf,
			{col = 0;},
			{col = 1;}
		);

		hosvdSliders = hosvdSliders.add(EZSlider.new(
			tuningwindow,
			Rect(20+(col*(sliderWidth+10)),
				(i-(col*slidersHalf))*(sliderHeight),
				sliderWidth,
				sliderHeight
			),
			"svd",
			ControlSpec(0,1,\lin,0.001,0,""),
			{|ez|
				var gradstr;
				hosvdValues[i] = ez.value;
				ez.value.postln;
				("slider number changed: "++i).postln;
				selectedDimensionInx.postln;
				selectedGradationInx.postln;
				gradstr = "";
				selectedGradationsInxes.do({|item, inx|
					gradstr = gradstr++item.asString++"."}
				);
				communicatorNetAddr.sendMsg(
					"/modifyWeights",
					selectedDimensionInx,
					numDimensions,
					gradstr,
					i,
					ez.value.asFloat);
			},
			hosvdValues[i],
			labelWidth:sliderWidth/15,
			unitWidth:sliderWidth/75)
		);

		hosvdSliders[i].setColors(Color.gray, Color.black, Color(0.6,0.9,0.1,0.95), Color.white, Color.black);
	};

	("numParams "++numParams).postln;

	numParams.do{
		arg i;
		var count = i + hosvdRnk;
		var col;

		if(count < slidersHalf,
			{col = 0;},
			{col = 1;}
		);

		paramSliders = paramSliders.add(EZSlider.new(
			tuningwindow,
			Rect(20+(col*(sliderWidth+10)),
				(count - (col*slidersHalf))*(sliderHeight),
				sliderWidth,
				sliderHeight
			),
			paramNames[i],
			ControlSpec(
				paramLimits[i][0],
				paramLimits[i][1],
				\lin,
				0.001,
				0,
				paramUnits[i]
			),
			{|ez|
				paramValues[i] = ez.value;
				//also be sure to check if this is not unshadowed... in that case shadowTensor
				//may be temporarily updated
			},
			paramValues[i],
			labelWidth:sliderWidth/2.5,
			unitWidth:sliderWidth/7.5)
		);

		paramSliders[i].setColors(
			Color.white,
			Color.black,
			Color(0.8, 0.2, 0.1, 0.8),
			Color.white,
			Color.black);
	};

	playButton = Button(
		tuningwindow,
		Rect(
			60,
			(slidersHalf + 0.5)*sliderHeight,
			((windowWidthTuning - 138) / 4),
			20
		)
	).states_([["Play", Color.black]]);


	stopButton = Button(
		tuningwindow,
		Rect(
			60 + 6 + ((windowWidthTuning - 138) / 4),
			(slidersHalf + 0.5)*sliderHeight,
			((windowWidthTuning - 138) / 4),
			20
		)
	).states_([["Stop Playing", Color.black]]);

	keepButton = Button(
		tuningwindow,
		Rect(
			60 + 12 + (2*((windowWidthTuning - 138) / 4)),
			(slidersHalf + 0.5)*sliderHeight,
			((windowWidthTuning - 138) / 4),
			20
		)
	).states_([["Keep Parameters", Color.black]]);

	sampleButton = Button(
		tuningwindow,
		Rect(
			60 + 18 + (3*((windowWidthTuning - 138) / 4)),
			(slidersHalf + 0.5)*sliderHeight,
			((windowWidthTuning - 138) / 4),
			20
		)
	).states_([["Sample Tuner", Color.black]]);

	///////////////////////
	//BUTTON ACTIONS...
	///////////////////////

	playButton.action_({
		var strHosvdValues = "", strParamValues = "", gradstr = "";

		selectedGradationsInxes.do({|item, inx|
			gradstr = gradstr++item.asString++";"});

		hosvdValues.do({|item, inx|
			strHosvdValues = strHosvdValues++item.asString++";"}
		);

		paramValues.do({|item, inx|
			strParamValues = strParamValues++item.asString++";"}
		);

		communicatorNetAddr.sendMsg("/playWeights",
			selectedDimensionInx,
			hosvdRnk,
			numParams,
			strHosvdValues,
			strParamValues,
			gradstr
		);
	});

	stopButton.action_({
		communicatorNetAddr.sendMsg("/stopSound");

	});


	keepButton.action_({
		var strHosvdValues = "", strParamValues = "", gradstr = "";

		selectedGradationsInxes.do({|item, inx|
			gradstr = gradstr++item.asString++";"});

		hosvdValues.do({|item, inx|
			strHosvdValues = strHosvdValues++item.asString++";"}
		);

		paramValues.do({|item, inx|
			strParamValues = strParamValues++item.asString++";"}
		);

		communicatorNetAddr.sendMsg("/keepWeights",
			selectedDimensionInx,
			hosvdRnk,
			numParams,
			strHosvdValues,
			strParamValues,
			gradstr
		);

		tuningwindow.close; // important because number of hosvd weights can change as a result of the manipulation

	});

	sampleButton.action_({
		var curvalue, newvalue, numsamples, samplediff, gradstr = "";
		selectedGradationsInxes.do({|item, inx|
			gradstr = gradstr++item.asString++"."}
		);

		numsamples = 100;
		samplediff = 1.0/numsamples;

		curvalue = -1*samplediff; //hosvdSliders[0].value;

		numsamples.do{
			curvalue = curvalue + samplediff;
			curvalue.postln;
			~setHosvdSlider.value(0,curvalue);

			communicatorNetAddr.sendMsg(
				"/modifyWeights",
				selectedDimensionInx,
				numDimensions,
				gradstr,
				i,
				curvalue.asFloat
			);
		}

	});

};

thisProcess.openUDPPort(57121);

//startGUI message...
OSCFunc.newMatching({|msg, time, addr, recvPort| ~mainWindow.value;},'/startGUI', recvPort:57121);

//setDimensionsNamesTunable message...
OSCFunc.newMatching({|msg, time, addr, recvPort|
	dimNamesTunable = (2..msg[1]+1).collect({|i| msg[i]});
	numDimensionsTunable = dimNamesTunable.size;
	("dimNamesTunable "++dimNamesTunable).postln;
	("numDimensionsTunable "+numDimensionsTunable).postln;
	"".postln;
	},
	'/setDimensionNamesTunable', recvPort:57121);

//setDimensionNames message...
OSCFunc.newMatching({|msg, time, addr, recvPort|
	dimNames = (2..msg[1]+1).collect({|i| msg[i]});
	numDimensions = dimNames.size;
	tunabilityArray = Array.series(msg[1], msg[1]+2, 1).collect({|i| msg[i]});
	tunableDimensions = Array.series(tunabilityArray.size, 0,1).select({|item, inx| tunabilityArray[item]==1});
	selectedGradationsInxes = Array.series(numDimensions, 0, 0);

	selectedDimensionInx = 0;
	selectedGradationInx = 0;

	("numDimensions "++numDimensions).postln;
	("tunabilityArray "++tunabilityArray).postln;
	("tunableDimensions "++tunableDimensions).postln;
	"".postln;
	},
	'/setDimensionNames', recvPort:57121);


//setNumberGradations message...
OSCFunc.newMatching({|msg, time, addr, recvPort|
	numGradations = (2..msg[1]+1).collect({|i| msg[i]});
	("numGradations "++numGradations).postln;
	"".postln;
}, '/setNumberGradations', recvPort:57121);


//setIndependentParameters message...
OSCFunc.newMatching({|msg, time, addr, recvPort| var npgroups, cnt=0;
	npgroups = msg[1];
	numParamss = (2..npgroups+1).collect({|i| msg[i]});

	paramNamess = numParamss.collect({|i| Array.series(i, 1, 0).collect(
		{|j| cnt = cnt + 1; msg[j+npgroups+cnt]})});

	paramLimitss = numParamss.collect({|i| Array.series(i, 1, 0).collect(
		{|j| cnt = cnt + 2; [msg[j+npgroups+cnt-1], msg[j+npgroups+cnt]]})});

	paramUnitss = numParamss.collect({|i| Array.series(i, 1, 0).collect(
		{|j| cnt = cnt + 1; msg[j+npgroups+cnt]})});

	calculateWindowDimensions.value;

	("numParamss "++numParamss).postln;
	("paramNamess "++paramNamess).postln;
	("numDimensions "++numDimensions).postln;
	("paramLimitss "++paramLimitss).postln;
	("paramUnitss "++paramUnitss).postln;
	"".postln;
}, '/setIndependentParameters', recvPort:57121);

//setWeights message...
OSCFunc.newMatching({|msg, time, addr, recvPort|
	hosvdRnk = msg[1];
	hosvdValues = Array.series(hosvdRnk, 2,1).collect({|n| msg[n]});
	paramValues = Array.series(numParamss[selectedDimensionInx],hosvdRnk+2, 1).collect({|n| msg[n]});
	calculateWindowDimensions.value;

	("hosvdRnk "++hosvdRnk).postln;
	("hosvdValues "++hosvdValues).postln;
	("paramValues "++paramValues).postln;
	"".postln;
	~tuningWindow.value;}, '/setWeights');

//refreshWeights message...
OSCFunc.newMatching({|msg, time, addr, recvPort|
	var numWeights = msg[1];
	"setting sliders to: ".post;
	(2..numWeights+1).do({
		|item, i|
		(msg[item].asString++", ").post;
		paramValues[i] = msg[item];
		outfile.write(msg[item].asString++" ");
		~setParamSlider.value(i, msg[item]);
	});
	outfile.write("\n");
	},
	'/refreshWeights');

//play dummy
OSCFunc.newMatching({|msg, time, addr, recvPort|
	var numWeights = msg[1];
	"Playing with parameters: ".post;
	(2..numWeights+1).do({
		|item, i|
		(msg[item].asString++", ").post;
	});
	"".postln;
	},
	'/playSound');

)




thisProcess.openPorts;


~initAudioChannelGUI.value(dimensionNames:["r", "so", "st"], numberGradations:[10, 2, 5], hosvdRank:2, parameterNames:[["apples"], ["pears", "oranges"], ["kiwis", "mangoes", "beans"]],parameterLimits:[[[0,10]],[[1,5],[1,2]],[[0,1],[0,1],[0,1]]], parameterUnits:[["hz"], ["on", "un"], ["l", "f", "f"]]);


~mainWindow.value;

~tuningWindow.value;
~parameterValues


(
Dialog.savePanel({ arg path;
    path.postln;
},{
    "cancelled".postln;
});
)

