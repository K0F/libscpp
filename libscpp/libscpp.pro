QT -= core gui
CONFIG -= qt
TARGET = SC++
TEMPLATE = lib
CONFIG += staticlib

# BUILD SUPERCOLLIDER WITH NO QT, NO AVAHI, AND AS STATIC LIBS!!!!!!!!!!!!############################################
# Supercollider vars #################################################################################################
SUPERCOLLIDER_SOURCE = /home/octopian/Documents/source/SuperCollider/SuperCollider-3.6-beta/supercollider/
LIBSCSYNTH = /home/octopian/Documents/source/SuperCollider/SuperCollider-3.6-beta/supercollider/noqtbuild/server/scsynth
LIBSCLANG = /home/octopian/Documents/source/SuperCollider/SuperCollider-3.6-beta/supercollider/noqtbuild/lang
BOOST = /usr/include
BOOST_LIBS = /usr/lib
LIBSNDFILE = /usr/lib/x86_64-linux-gnu
AVAHI_COMMON = /usr/lib/x86_64-linux-gnu
AVAHI_CLIENT = /usr/lib/x86_64-linux-gnu
FFTWF = /usr/lib/x86_64-linux-gnu
JACK = /usr/lib/x86_64-linux-gnu
FLAC = /usr/lib/x86_64-linux-gnu
OGG = /usr/lib/x86_64-linux-gnu
######################################################################################################################

#CONFIG+=no_smart_library_merge
#LIBS += -Wl,--start-group
QMAKE_LFLAGS += -Wl,--start-group,--end-group

#libsclang.a
LIBS += $${LIBSCLANG}/libsclang.a -Wl,--no-whole-archive
PRE_TARGETDEPS += $${LIBSCLANG}/libsclang.a

#libscsynth.a
LIBS += -Wl,--whole-archive $${LIBSCSYNTH}/libscsynth.a
PRE_TARGETDEPS += $${LIBSCSYNTH}/libscsynth.a

#boost
LIBS += $${BOOST_LIBS}/libboost_date_time.a
PRE_TARGETDEPS += $${BOOST_LIBS}/libboost_date_time.a

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

# Jack
LIBS += $${JACK}/libjack.a
PRE_TARGETDEPS += $${JACK}/libjack.a

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
}

linux-g++ {
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
    ../AbstractFunction.cpp

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
    ../AbstractFunction.h
