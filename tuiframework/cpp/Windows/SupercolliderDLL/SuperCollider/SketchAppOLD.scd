//BOOT SERVER
(
Server.default = Server.internal;
s = Server.default;
s.boot;
)


(
var roughnessgrainamp = 0.5;
var grainposition = 0.5;
var grainlengthwindowmin = 0.2;
var grainlengthwindowmax = 0.5;
var grainminamppiano = 0.6;
var grainmaxamppiano = 1.0;
var grainminamproughness = 0.6;
var grainmaxamproughness = 1.0;
var grainreverb = 0.5;
var grainklankfreq1 = 600;
var grainklankamp1 = 1.0;
var grainklankdecay1 = 0.8;
var grainklankfreq2 = 800;
var grainklankamp2 = 1.0;
var grainklankdecay2 = 0.8;
var grainklankfreq3 = 1000;
var grainklankamp3 = 1.0;
var grainklankdecay3 = 0.8;
var globalamp = 1.0;
var numInstruments = 3;
var maindir = "/Users/csapoadam/Documents/Development/SpiralDiscoveryMethod/SuperCollider";

//var env = Env([0,1,0.3,1.0],[2.0,2.5,0.5]); //use a language side envelope to vary position

SynthDef("DecayPink", {arg outBus = 0, effectBus, amp = 1, reverb = 0.5, impulsefreq = 1, attacktime = 0.01, decaytime = 0.2;
	var source;
	//Decaying pulses of PinkNoise.  Reverb will be added later
	source = Decay2.ar(Impulse.ar(impulsefreq, 0.25), attacktime, decaytime, 0.1*PinkNoise.ar);
	//main output
	Out.ar(outBus, amp*source*(1-reverb));
	//effects output
	Out.ar(effectBus, amp*source*reverb);
}).add;

SynthDef("DecaySin", {arg outBus = 0, effectBus, amp = 1, reverb = 0.5, modulfreq = 0.01, impulsefreq = 5;
	var source;
	//Decaying pulses of sine wave.  Reverb will be added later
	//source = Decay2.ar(Impulse.ar(impulsefreq, 0.25), rrand(0.3, 0.6), rrand(0.2, 0.8), SinOsc.ar(SinOsc.kr(modulfreq, 0, 110, 440)));
	source = Decay2.ar(Impulse.ar(impulsefreq, LinCongL.ar(impulsefreq/2.0, 1.1, 0.13, 1, 0, 0.5, 1)), rrand(0.3, 0.6), rrand(0.2, 0.8), SinOsc.ar(SinOsc.kr(modulfreq, 0, 110, 440)));
	//main output
	Out.ar(outBus, 0.1*amp*source*(1-reverb));
	//effects output
	Out.ar(effectBus, 0.1*amp*source*reverb);
}).add;

SynthDef("Reverb", {arg outBus = 0, inBus;
	var input;
	input = In.ar(inBus, 1);
	//a low rent reverb
	//aNumber.do will evaluate it's function argument a corresponding number of times
	//{}.dup(n) will evaluate the function n times and return an array of the results
	//The default for n is 2, so this makes a stero reverb
	16.do({input = AllpassC.ar(input, 0.04, {Rand(0.001,0.04)}.dup, 3)});
	Out.ar(outBus, input);
}).add;


SynthDef("SampledGrain",{arg buffer, effectBus, dur=0.05, pos = 0.0, amp= 1, pan= 0.0, rate1 = 0.5, rate2 = 1, rate3 = 2, klankfreq1 = 600, klankfreq2 = 800, klankfreq3 = 1000, klankamp1 = 1, klankamp2 = 1, klankamp3 = 1, klankdecay1 = 0.5, klankdecay2 = 0.5, klankdecay3 = 0.5, reverb = 0.5;
	var env, source1, source2, source3;

	source1 = PlayBuf.ar(1,buffer,rate:rate1, startPos:pos*BufFrames.ir(buffer));
	source2 = PlayBuf.ar(1,buffer,rate:rate2, startPos:pos*BufFrames.ir(buffer));
	source3 = PlayBuf.ar(1,buffer,rate:rate3, startPos:pos*BufFrames.ir(buffer));

	env = EnvGen.ar(Env([0,1,1,0],[0.01,dur,0.01]),doneAction:2);

	Out.ar(0,Pan2.ar(Klank.ar(`[[klankfreq1, klankfreq2, klankfreq3], [klankamp1, klankamp2, klankamp3], [klankdecay1, klankdecay2, klankdecay3]], (1-reverb)*env*0.01*amp*0.33*(source1+source2+source3)),pan));

	Out.ar(effectBus, 0.01*amp*0.33*(source1+source2+source3)*reverb)

}).add;


b = Bus.audio(s, 1); //effects bus
c = Buffer.read(s, maindir++"/sounds/Bach.wav");
d = Buffer.read(s, maindir++"/sounds/Rough9.wav");

f = {
	inf.do{arg i;
		var count = (i%500);

		Synth("SampledGrain", [\buffer, c.bufnum, \effectBus, b, \pos, rrand(grainposition, grainposition+0.1),\dur, rrand(grainlengthwindowmin, grainlengthwindowmax), \amp, globalamp*exprand(grainminamppiano, grainmaxamppiano), \rate1, 1.22, \rate2, 0.95, \rate3, 1.5, \klankfreq1, grainklankfreq1, \klankamp1, globalamp*grainklankamp1, \klankdecay1, grainklankdecay1, \klankfreq2, grainklankfreq2, \klankamp2, globalamp*grainklankamp2, \klankdecay2, grainklankdecay2, \klankfreq3, grainklankfreq3, \klankamp3, globalamp*grainklankamp3, \klankdecay3, grainklankdecay3, \reverb, grainreverb]);
		(((grainposition*0.2)+0.01).max(0.01)).wait
	};
};


g = {
	inf.do{arg i;
		var count = (i%500);

		Synth("SampledGrain", [\buffer, d.bufnum, \effectBus, b, \pos, rrand(grainposition, grainposition+0.1),\dur, rrand(grainlengthwindowmin, grainlengthwindowmax), \amp, globalamp*exprand(7*grainminamproughness, 7*grainmaxamproughness), \rate1, 1.22, \rate2, 0.95, \rate3, 1.5, \klankfreq1, grainklankfreq1, \klankamp1, globalamp*grainklankamp1, \klankdecay1, grainklankdecay1, \klankfreq2, grainklankfreq2, \klankamp2, globalamp*grainklankamp2, \klankdecay2, grainklankdecay2, \klankfreq3, grainklankfreq3, \klankamp3, globalamp*grainklankamp3, \klankdecay3, grainklankdecay3, \reverb, grainreverb]);
		(((grainposition*0.2)+0.01).max(0.01)).wait
	};
};


//INITIALIZE NETWORKING
n = NetAddr.new("127.0.0.1", 57120); // the url should be the one of computer of app 2 (or nil)

OSCFunc.newMatching({|msg, time, addr, recvPort| \startSound.postln;
	x = Synth.new("Reverb", [\inBus, b]);
	y = Synth.before(x, "DecayPink", [\effectBus, b, \amp, (globalamp*msg[20])/numInstruments, \impulsefreq, msg[21], \reverb, msg[22], \attacktime, msg[23], \decaytime, msg[24]]);
	z = Synth.before(x, "DecaySin", [\effectBus, b, \outBus, 1, \amp, (globalamp*msg[25])/numInstruments, \reverb, msg[28], \modulfreq, msg[26], \impulsefreq, msg[27]]);
	grainposition = msg[3];
	grainlengthwindowmin = msg[4];
	grainlengthwindowmax = msg[5];
	grainminamppiano = msg[6];
	grainmaxamppiano = msg[7];
	grainminamproughness = msg[8];
	grainmaxamproughness = msg[9];
	grainreverb = msg[10];
	grainklankfreq1 = msg[11];
	grainklankamp1 = msg[14];
	grainklankdecay1 = msg[17];
	grainklankfreq2 = msg[12];
	grainklankamp2 = msg[15];
	grainklankdecay2 = msg[18];
	grainklankfreq3 = msg[13];
	grainklankamp3 = msg[16];
	grainklankdecay3 = msg[19];
	globalamp = msg[2];
	globalamp.postln;
	grainposition.postln;
	w = f.fork;
	u = g.fork;},'/startRoughness');

OSCFunc.newMatching({|msg, time, addr, recvPort| \stopSound.postln; w.stop; w.free; u.stop; u.free; y.free; z.free; x.free;},'/stop');

OSCFunc.newMatching({|msg, time, addr, recvPort| \quitting.postln; Server.internal.freeAll;},'/quit');

//OSCFunc({|msg, time, addr, recvPort| \oneShot.postln}, '/chat', n).oneShot; // once only
//OSCdef(\test, {|msg, time, addr, recvPort| a = 3; v = 5;}, '/chat', n); // def style
//OSCFunc.trace(false); // just for diagnostics


)


//grainlastval important parameter 0.01 -0.7  0.7 medium 0.01 smooth 0.1 rough?
//grainreverb
//impulsefreq (y)
//direct (y)
//attacktime (y)
//decaytime(y)

(
SynthDef("Bubbling", {arg outBus = 0;
	var sound;
	sound = ({RHPF.ar(
		OnePole.ar(BrownNoise.ar, 0.99),
		LPF.ar(BrownNoise.ar, 14) * 400 + 500,
		0.03,
		0.003)}!2) +
	({RHPF.ar(
		OnePole.ar(BrownNoise.ar, 0.99),
		LPF.ar(BrownNoise.ar, 20)* 800 + 1000,
		0.03, 0.005)}!2) * 4;

	//main output
	Out.ar(outBus, sound);
}).add;
)


(
x = Synth.new("Bubbling");
)


(
x = Synth.new("Reverb", [\inBus, b]);
y = Synth.before(x, "DecayPink", [\effectBus, b]);
z = Synth.before(x, "DecaySin", [\effectBus, b, \outBus, 1, \direct, 0, \modulfreq, 100, \impulsefreq, 5]);
w = f.fork;
)

u = g.fork;

s.scope;


z.set(\modulfreq, 1000, \reverb, 0.9, \impulsefreq, 2)
y.set(\impulsefreq, 3, \attacktime, 3, \decaytime, 1)

x.free; y.free; z.free; b.free;
w.stop;
w.free;

