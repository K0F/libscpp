libsc++
=======

SuperCollider C++ library for writing programs using scsynth as an internal server
Author: Chad McKinney
Contact: seppukuzombie@gmail.com
website: http://chadmckinneyaudio.com/

This is an extremely early version that is concurrently undergoing active and heavy development. SuperCollider is a large language and this is the beginning of a meager attempt to replicate some of the functionality. It is (and will likely always be) incomplete and not thoroughly tested. 


PREREQUISITES
=============

SuperCollider Source: currently tested against SC 3.5 and 3.6 for OS X and Linux.
Boost: Version 1.49 (not just the header only library, but also the built static libraries). 


Be sure that your program is correctly linking to these:

libboost_date_time.a
libsclang.a
libscsynth.dylib
libsc++.a

as well as the header includes for SuperCollider and its external_libraries folder as well as boost and libsc++.

If you wish to compiled libsc++.a yourself you should only need to change these user variables for correct linking and including:

BOOST_LIBRARIES_PATH
BOOST_PATH
SC_SOURCE_PATH

ibboost_date_time.a, libsclang.a, and libscsynth.dylib, and a precompiled libsc++.a have been provided, compiled for os x 10.6 x86_64 however you will still need the SuperCollider source and Boost Headers

SC Source:

http://supercollider.sourceforge.net/downloads/

Boost:
http://www.boost.org/


BUILDING
========

There are two options for building libsc++. The first is to use the provided XCode project file, however this is falling out of favor as I've moved to linux and may not be completely up to date. The preferred choice is to use the QtCreator .pro file. This project uses no Qt libraries and your program won't have such dependancies. QtCreator has always been my preferred IDE for C++ development and since my move to linux I've now converted all my projects over. 

Building SuperCollider for libsc++ requires some specific tweaks. To make things easier you should use cmake-gui so you can quickly find all the arguments you need to set. You should build a special version (not for personal SC usage) that is built with no Qt, no Avahi, with the native flag checked, as RELEASE, and NO SCIDE. If you would like to statically link to libsc++ you'll need to include several dependancies in your program (reference the .pro file see a full list), and you'll need to make sure to build SuperCollider WITHOUT the libscynth option checked (by default SC builds against static libs). In general this build process is complicated and unless you're familiar with building SC you may find it (too) difficult. Best of luck! 


USAGE
=======

libsc++ provides a namespace (sc::) for creating C++ applications using the SuperCollider server internally in the same process (however, in another thread). You do not require any kind of OSC networking to use the library as it uses the built in message system that the SC source provides.

There are several things to consider. The idea is that we can make a C++ program that is completely independent of an SC installation and is therefor easily deployable. Because of this any synths that you want to use will need to be compiled in an actual SuperCollider app and the compiled .scsyndef files included in the plugins folder you link to. 

The Plugin Path path defaults to /Applications/SuperCollider/SuperCollider.app/Contents/Resources/plugins
The Synth Defs path defaults to /Users/USER_NAME/Library/Application\ Support/SuperCollider/synthdefs (with USER_NAME replaced by the actual user name)

Both of these can be (and likely SHOULD BE) changed via the ServerOptions class. When you create a new Server instance you can provide it an instance of ServerOptions. For the most part these can be left to their defaults however the two mentioned path as well as the port number should likely be changed. It is entirely possible that your program will be run on the same machine as SuperCollider so setting the port for your Server to be different than the default 57110 will prevent several issues for the user. 

```c++
//EXAMPLE CODE

sc::ServerOptions options;
options.setPortNum(67110); // Change from the defaul 57110 to prevent conflicts with SC
options.pluginPath = "./Yig.app/Contents/MacOS/plugins"; // Path in our app bundle
options.synthDefPath = "./Yig.app/Contents/MacOS/synthdefs"; // Path in our app bundle

// Create a new Server instance. 
// This will default as the Server::internal instance which other classes will reference.
sc::Server* server = new sc::Server(options); 
server->boot(); // Boot the server

// Play a sound (NOTE: "TestSine" is not provided with the library.) 
// The synth defaults to using the internal server which is our Server instance.
sc::Synth::grain("TestSine"); 

/*
  Here is an example of providing arguments to a synth with the sc::ARGS() macro. 
  It makes proving a variable number of arguments easier.
  CAUTION. Your values must be in (const char*), (double) pairs or the behavior 
  IS UNDEFINED. YOU CANNOT USE AN INT YOU MUST CAST TO FLOAT OR DOUBLE.
*/
sc::Synth::grain("Test",sc::ARGS("freq",(double)(rand()%2000),"amp",0.1,"mod",30));



// Recording is easy, BUT YOU MUST FIRST HAVE COMPILED THE "libsc++-server-record" 
// SYNTHDEF IN libSC++SynthDefs.rtf AND PUT IT IN THE SYNTHDEFS FOLDER YOU LINK TO
server->record();
// ...do stuff...
server->stopRecording();
```

This will drop it by default into the SuperCollider recordings  path, for example on OS X: 
/Users/USER_NAME/Music/SuperCollider\ Recordings


STATUS
=======
libsc++ is a project mainly used by myself and members of my band Glitch Lich (www.glitchlich.com), however I've released it as open source along with all my other software in the hopes other may find it useful as well. That said, it's not the cleanest of projects and other developers may find it somewhat lacking in documentation. I would welcome external help to develop a more user friendly and extensive library, but as it stands development mainly focuses on the needs of my personal projects and will see continued, if not sporadic development for the foreseeable future.


License
=======

libsc++ is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version. See "COPYING" for the license text.
