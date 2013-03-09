#include "LinuxPlatform.h"
#include "Server.h"

namespace sc {

//////////////////
// Unix Platform
/////////////////

LinuxPlatform::LinuxPlatform() : UnixPlatform()
{
    recordingsDir = getenv("HOME");
    recordingsDir.append("/.local/share/SuperCollider/Recordings");
}

void LinuxPlatform::initPlatform()
{
    UnixPlatform::initPlatform();
}

const char* LinuxPlatform::getClassLibraryDir()
{
    return UnixPlatform::getClassLibraryDir();
}

void LinuxPlatform::setClassLibraryDir(const char* _dir)
{
    UnixPlatform::setClassLibraryDir(_dir);
}

const char* LinuxPlatform::getRecordingsDir()
{
    return recordingsDir.c_str();
}

void LinuxPlatform::setRecordingsDir(const char* _dir)
{
    UnixPlatform::setRecordingsDir(_dir);
}

OperatingSystem LinuxPlatform::name()
{
    return LINUX;
}

void LinuxPlatform::recompile()
{
    UnixPlatform::recompile();
}

void LinuxPlatform::startup()
{
    Server::program = "exec scsynth";

    // Default jack port hookup
    putenv("SC_JACK_DEFAULT_INPUTS=system");
    putenv("SC_JACK_DEFAULT_OUTPUTS=system");

    // automatically start jack when booting the server
    // can still be overridden with JACK_NO_START_SERVER
    putenv("JACK_START_SERVER=true");
}

void LinuxPlatform::shutdown()
{
    UnixPlatform::shutdown();
}

} // sc namespace
