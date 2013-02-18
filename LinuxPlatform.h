#ifndef LINUX_H
#define LINUX_H

#include "Platform.h"

namespace sc {

class LinuxPlatform : public UnixPlatform
{
public:
    LinuxPlatform();

    virtual void initPlatform();
    virtual OperatingSystem name();
    virtual void recompile();

    const char* getClassLibraryDir();
    void setClassLibraryDir(const char* _dir);
    const char* getRecordingsDir();
    void setRecordingsDir(const char* _dir);

    virtual void startup();
    virtual void shutdown();

    const static OperatingSystem os = LINUX;
};

} // sc namespace

#endif // LINUX_H
