QT -= core gui
CONFIG -= qt
TARGET = SC++
TEMPLATE = lib
CONFIG += x86_64
CONFIG += staticlib

include(./QMakeVars.pri)

INCLUDEPATH += customboost/
DEPENDPATH += customboost/
INCLUDEPATH += /
DEPENDPATH += /

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# BUILD SUPERCOLLIDER WITH NO QT, NO AVAHI, AND MAKE SURE YOU HAVE LIBREADLINE INSTALLED BEFORE BUILDING SUPERCOLLIDER!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

###################################################################################################################
# Supercollider qmake variables
# DEFINE THESE IN QMakeVars.pri, CHANGING THEM TO YOUR LOCAL DIRECTORIES
# SUPERCOLLIDER_SOURCE
# LIBSCSYNTH
# LIBSCLANG
# BOOS
# BOOST_LIBS
# LIBSNDFILE
# AVAHI_COMMON
# AVAHI_CLIENT
# FFTWF
# JACK
# FLAC
# OGG
###################################################################################################################

#CONFIG+=no_smart_library_merge
#LIBS += -Wl,--start-group
#QMAKE_LFLAGS += -Wl,--start-group,--end-group

#libsclang.a
LIBS += $${LIBSCLANG}/libsclang.a# -Wl,--no-whole-archive
PRE_TARGETDEPS += $${LIBSCLANG}/libsclang.a

#libscsynth.a
LIBS += $${LIBSCSYNTH}/libscsynth.a #-Wl,--whole-archive
PRE_TARGETDEPS += $${LIBSCSYNTH}/libscsynth.a

#boost
LIBS += $${BOOST_LIBS}/libboost_date_time.a
PRE_TARGETDEPS += $${BOOST_LIBS}/libboost_date_time.a

LIBS += $${BOOST_LIBS}/libboost_signals.a
PRE_TARGETDEPS += $${BOOST_LIBS}/libboost_signals.a

# Libsndfile
LIBS += $${LIBSNDFILE}/libsndfile.a
PRE_TARGETDEPS += $${LIBSNDFILE}/libsndfile.a

# Avahi
#LIBS += $${AVAHI_COMMON}/libavahi-common.a
#LIBS += $${AVAHI_CLIENT}/libavahi-client.a
#PRE_TARGETDEPS += $${AVAHI_COMMON}/libavahi-common.a
#PRE_TARGETDEPS += $${AVAHI_CLIENT}/libavahi-client.a

# FFTWF
LIBS += $${FFTWF}/libfftw3f.a
PRE_TARGETDEPS += $${FFTWF}/libfftw3f.a

linux-g++ {

# Jack
LIBS += $${JACK}/libjack.so
PRE_TARGETDEPS += $${JACK}/libjack.so

}

# FLAC
LIBS += $${FLAC}/libFLAC.a
PRE_TARGETDEPS += $${FLAC}/libFLAC.a

# Vorbis
LIBS += $${VORBIS}/libvorbis.a
LIBS += $${VORBIS}/libvorbisenc.a


# OGG
LIBS += $${OGG}/libogg.a
PRE_TARGETDEPS += $${OGG}/libogg.a

#LIBS += -Wl,--end-group

mac {
    DEFINES += __APPLE__
    INCLUDEPATH += $${BOOST}
    DEPENDPATH += $${BOOST}
    CMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11
    QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11
    LIBS += -lc++
    QMAKE_CXXFLAGS += -mmacosx-version-min=10.8
    CXX_FLAGS += -mmacosx-version-min=10.8


    # Change to your OS version (and make sure its there!)
    MAC_SDK  = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk
    if( !exists( $$MAC_SDK) ) {
      error("The selected Mac OSX SDK does not exist at $$MAC_SDK!")
    }

    QMAKE_MAC_SDK = $$MAC_SDK
    INCLUDEPATH += $$QMAKE_MAC_SDK/System/Library/Frameworks/CoreFoundation.framework/Versions/A/Headers
    DEPENDPATH  += $$QMAKE_MAC_SDK/System/Library/Frameworks/CoreFoundation.framework/Versions/A/Headers
}

linux-g++-64 {
    DEFINES += __LINUX__
    DEFINES += SC_AUDIO_API=SC_AUDIO_API_JACK
}


INCLUDEPATH += $${SUPERCOLLIDER_SOURCE}/include/common \
    $${SUPERCOLLIDER_SOURCE}/include/plugin_interface \
    $${SUPERCOLLIDER_SOURCE}/include/server \
    $${SUPERCOLLIDER_SOURCE}/include/lang \
    $${SUPERCOLLIDER_SOURCE}/external_libraries \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/boost \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/boost/libs/date_time/src/gregorian \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/boost/libs/date_time/src/posix_time \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/boost_endian \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/boost_lockfree \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/curl \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/icu \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/libsndfile \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/nova-simd \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/nova-tt \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/oscpack \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/pycxx-6.22 \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/simplejson-2.3.2 \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/threadpool \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/TLSF-2.4.6/src \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/yaml-cpp-0.3.0

DEPENDPATH += $${SUPERCOLLIDER_SOURCE}/include/common \
    $${SUPERCOLLIDER_SOURCE}/include/plugin_interface \
    $${SUPERCOLLIDER_SOURCE}/include/server \
    $${SUPERCOLLIDER_SOURCE}/include/lang \
    $${SUPERCOLLIDER_SOURCE}/external_libraries \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/boost \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/boost/libs/date_time/src/gregorian \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/boost/libs/date_time/src/posix_time \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/boost_endian \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/boost_lockfree \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/curl \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/icu \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/libsndfile \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/nova-simd \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/nova-tt \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/oscpack \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/pycxx-6.22 \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/simplejson-2.3.2 \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/threadpool \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/TLSF-2.4.6/src \
    $${SUPERCOLLIDER_SOURCE}/external_libraries/yaml-cpp-0.3.0

SOURCES += \
    ../Thread.cpp \
    ../SystemActions.cpp \
    ../Stream.cpp \
    ../Server.cpp \
    ../SequencableCollection.cpp \
    ../sc.cp \
    ../Quant.cpp \
    ../Platform.cpp \
    ../OSXPlatform.cpp \
    ../OSCMessages.cpp \
    ../Node.cpp \
    ../NetAddr.cpp \
    ../Main.cpp \
    ../List.cpp \
    ../Kernel.cpp \
    ../InternalSynthServer.cpp \
    ../Function.cpp \
    ../Event.cpp \
    ../Environment.cpp \
    ../Engine.cpp \
    ../Collection.cpp \
    ../Clock.cpp \
    ../Bus.cpp \
    ../Buffer.cpp \
    ../AbstractFunction.cpp \
    ../LinuxPlatform.cpp \
    ../scprocess.cpp \
    ../process.cpp

HEADERS += \
    ../Thread.h \
    ../SystemActions.h \
    ../Stream.h \
    ../Server.h \
    ../SequenceableCollection.h \
    ../SC++Priv.h \
    ../scmath.h \
    ../sc.h \
    ../Quant.h \
    ../Platform.h \
    ../OSXPlatform.h \
    ../OSCMessages.h \
    ../Node.h \
    ../NetAddr.h \
    ../Main.h \
    ../List.h \
    ../Kernel.h \
    ../InternalSynthServer.h \
    ../Function.h \
    ../Event.h \
    ../Environment.h \
    ../Engine.h \
    ../Collection.h \
    ../Clock.h \
    ../Bus.h \
    ../Buffer.h \
    ../AbstractFunction.h \
    ../LinuxPlatform.h \
    ../scprocess.h \
    ../process.h \
    ../externals/pstreams-0.7.0/pstream.h

OTHER_FILES += \
    QMakeVars.pri
