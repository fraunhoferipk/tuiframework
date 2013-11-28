//BOOT SERVER
(
Server.default = Server.internal;
s = Server.default;
s.boot;
)



(
//Global variables which can be updated from C++ code using OSC messages
//var pathToAudio = "/Users/csapoadam/Documents/Development/TNAProjectBerlin/audioConfig2/sounds/";
var pathToAudio = "C:/git/tuiframework/tuiframework/cpp/tuilibs/sound/sounds/";
var totalTime = 10;
var amplitude = 1;
var bufferToUse = 1;
var grainSamplingRate = 50;
var centerPosition = 0.5; //from 0 to 1, selecting position within sound buffer
var whiteNoiseExponent = 1; //from 0 to 3, specifying white noise parameter for rate exponent
var decayRatio = 0.3; // relative amount of time dedicated to decay
var reverberation = 0.5;

// Load audio files
~bufRough0 = Buffer.read(s, pathToAudio++"roughness0.wav");
~bufRough1 = Buffer.read(s, pathToAudio++"roughness1.wav");
~bufRough2 = Buffer.read(s, pathToAudio++"roughness2.wav");
~bufRough3 = Buffer.read(s, pathToAudio++"roughness3.wav");
~bufRough4 = Buffer.read(s, pathToAudio++"roughness4.wav");
~bufRough5 = Buffer.read(s, pathToAudio++"roughness5.wav");
~bufRough6 = Buffer.read(s, pathToAudio++"roughness6.wav");
~bufRough7 = Buffer.read(s, pathToAudio++"roughness7.wav");
~bufRough8 = Buffer.read(s, pathToAudio++"roughness8.wav");
~bufRough9 = Buffer.read(s, pathToAudio++"roughness9.wav");
~audioBuffers = [~bufRough0, ~bufRough1, ~bufRough2, ~bufRough3, ~bufRough4, ~bufRough5, ~bufRough6, ~bufRough7, ~bufRough8, ~bufRough9];

//effectsBus
~effectsBus = Bus.audio(s, 1); //effects bus

// INSTRUMENT TO PLAY FRICTION
SynthDef("Friction", {
	arg buffer, outBus=0, effectsBus, amp=1, reverb=0.5, trate=50, centerPos=0.5, whiteExp=1.0, soundDuration=1.0, decayRat=0.3;
	var sound, grainDur, attackTime;
	//trate = MouseY.kr(2,120,1);
	grainDur = 1.2/trate;
	attackTime = soundDuration - (decayRatio*soundDuration);

	sound = TGrains.ar(
		2,
		Impulse.ar(trate),
		buffer,
		(1.2 ** WhiteNoise.kr(whiteExp).round(1)),
		//MouseX.kr(0,BufDur.kr(buffer)),
		centerPos*BufDur.kr(buffer),
		11*grainDur,
		WhiteNoise.kr(0.6),
		1.2*EnvGen.kr(Env([0,0.2*amp,amp,0],
			[
				0.1*attackTime,
				0.2*attackTime,
				0.7*attackTime,
				decayRatio*soundDuration
	])));

	//main output
	Out.ar(outBus, sound*(1-reverb));
	//effects output
	Out.ar(effectsBus, sound*reverb);

}).add;

SynthDef("Reverb", {
	arg outBus=0, inBus;
	var input;
	input = In.ar(inBus, 1);
	16.do({input = AllpassC.ar(input, 0.04, {Rand(0.001,0.04)}.dup, 3)});
	Out.ar(outBus, input);
}).add;

~frictionTask = Task({
	loop({
		v = Synth(\Reverb, [\inBus, ~effectsBus]);
		x = Synth(\Friction,
			[
				\amp, amplitude,
				\buffer, ~audioBuffers[bufferToUse],
				\trate, grainSamplingRate,
				\centerPos, centerPosition,
				\whiteExp, whiteNoiseExponent,
				\soundDuration, totalTime,
				\decayRat, decayRatio,
				\effectsBus, ~effectsBus,
				\reverb, reverberation
		]);
		totalTime.wait;
		x.free;
		v.free;
		"Press continue".postln;
		nil.yield;
	});
});


////////////////////////////////////////////////////////////////
/////////////////////// NETWORKING /////////////////////////////
////////////////////////////////////////////////////////////////

//////INITIALIZE NETWORKING
n = NetAddr.new("127.0.0.1", 57120);

OSCFunc.newMatching({|msg, time, addr, recvPort| \startSound.postln;
	msg.postln;
	~frictionTask.stop;
	~frictionTask.reset;
	totalTime = msg[2];
	("totalTime = "++totalTime).postln;
	amplitude = msg[3];
	("amplitude = "++amplitude).postln;
	bufferToUse = msg[4].round;
	("bufferToUse = "++bufferToUse).postln;
	grainSamplingRate = msg[5];
	("grainSamplingRate = "++grainSamplingRate).postln;
	centerPosition = msg[6];
	("centerPosition = "++centerPosition).postln;
	whiteNoiseExponent = msg[7];
	("whiteNoiseExponent = "++whiteNoiseExponent).postln;
	decayRatio = msg[8];
	("decayRatio = "++decayRatio).postln;
	reverberation = msg[9];
	("reverberation = "++reverberation).postln;
	~frictionTask.resume(0);},'/startFriction');

OSCFunc.newMatching({|msg, time, addr, recvPort| \stopSound.postln; ~frictionTask.stop; x.free;},'/stop');

OSCFunc.newMatching({|msg, time, addr, recvPort| \quitting.postln; Server.internal.freeAll;},'/quit');


////////////////////////////////////////////////////////////////
////// CONTROL INTERFACE TO START AND STOP SOUND ///////////////
////////////////////////////////////////////////////////////////

w = Window.new("Example", Rect(400, 400, 200, 30)).front;
w.view.decorator = FlowLayout(w.view.bounds);
Button.new(w, Rect(0, 0, 100, 20)).states_([["Continue", Color.black, Color.clear]]).action_({~frictionTask.resume(0);});
Button.new(w, Rect(0, 0, 40, 20)).states_([["Pause", Color.black, Color.clear]]).action_({~frictionTask.pause;});
Button.new(w, Rect(0, 0, 40, 20)).states_([["Finish", Color.black, Color.clear]]).action_({~frictionTask.stop; w.close;});
)







