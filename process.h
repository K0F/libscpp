#ifndef PROCESS_H
#define PROCESS_H

/* The classes below are exported */
#pragma GCC visibility push(default)

#include <cstdio>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <thread>

// #include <boost/signal.hpp>
#include "customboost/customsignals2.hpp"
#include <boost/bind.hpp>

#include "externals/pstreams-0.7.0/pstream.h";

namespace sc {

// Generic class for controlling child processes
class UnixProcess
{
public:

    enum OpenMode{ NotOpen, ReadOnly, WriteOnly, ReadWrite, Truncate, Unbuffered };
    enum ExitStatus { NormalExit, CrashExit };
    enum ProcessChannel { StandardOutput, StandardError };
    enum ProcessChannelMode { SeparateChannels, MergedChannels, ForwardedChannels };
    enum ProcessError { FailedToStart, Crashed, Timedout, WriteError, ReadError, UnknownError };
    enum ProcessState { NotRunning, Starting, Running };

    UnixProcess(std::string programPath);
    virtual ~UnixProcess();

    void sendCommand(const std::string& command);
    void sendCommand(const std::list<std::string>& command);

    const std::string& programPath() const;
    ProcessError error() const;
    int	exitCode() const;
    ExitStatus exitStatus() const;
    std::string	nativeArguments() const;
    pid_t pid() const;
    void setNativeArguments(const std::string& arguments);
    void setWorkingDirectory(const std::string& dir);
    void start(const std::string& arguments = "");
    ProcessState state() const;
    void waitForFinished();
    void waitForStarted();
    std::string	workingDirectory() const;
    void iorun();
    void kill();
    void terminate();

    virtual void close();

    // Signals
    boost::signals2::signal<void (ProcessError processError)> errored;
    boost::signals2::signal<void (ExitStatus exitStatus)> finished;
    boost::signals2::signal<void (std::string errorOutput)> readyReadStandardError;
    boost::signals2::signal<void (std::string standardOutput)> readyReadStandardOutput;
    boost::signals2::signal<void ()> started;
    boost::signals2::signal<void (ProcessState newState)> stateChanged;

    static const redi::pstreams::pmode all3streams;

protected:

    void setProcessState(ProcessState state);
    virtual void setupChildProcess();
    ExitStatus cleanup();

    ProcessError mProcessError;
    ProcessState mProcessState;
    unsigned int mExitCode;
    ExitStatus mExitStatus;
    std::list<std::string> mEnvironment;
    std::string mNativeArguments;
    std::string mWorkingDirectory;
    std::string mProgramPath;
    redi::pstream* ps;
    std::thread* iothread;
};

} // sc namespace

#endif // PROCESS_H
