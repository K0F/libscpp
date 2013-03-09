

// libsc++ Recording Synth


SynthDef.new("libsc++-server-record",{
	|bufnum=65536, gate=1|
	var env;
	env = EnvGen.ar(Env.asr(0.1,1,0.1),gate:gate,doneAction:2);
	DiskOut.ar(bufnum, In.ar(0, 2));
}).store;
