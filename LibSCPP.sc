/*
LibSCPP is a static class used to communicate with a program using the C++ library libsc++.
*/
LibSCPP {
	classvar signalPrefix;

	*initClass {
		signalPrefix = \__LIBSCPP_SIGNAL__;

		StartUp.add {
			LibSCPP.emitSignal(\startup);
		}
	}

	*emitSignal {
		arg name;
		(signalPrefix++name).postln;
	}

	*compileFile {
		arg path;
		thisProcess.interpreter.compileFile(path).value;
	}
}
