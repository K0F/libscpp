#include "process.h"

using namespace redi;

namespace sc {

const pstreams::pmode UnixProcess::all3streams = pstreams::pstdin|pstreams::pstdout|pstreams::pstderr;

UnixProcess::UnixProcess(std::string programPath) :
    mProgramPath(programPath),
    ps(0),
    iothread(0)
{
    mProcessError = UnixProcess::UnknownError;
    mProcessState = UnixProcess::NotRunning;
    mExitCode = 0;
    mExitStatus = UnixProcess::NormalExit;
}

UnixProcess::~UnixProcess()
{
    if(this->mProcessState != UnixProcess::NotRunning)
    {
        waitForFinished();
    }
}

void UnixProcess::sendCommand(const std::string& command)
{
    *ps << command.c_str() << std::endl;
}

void UnixProcess::sendCommand(const std::list<std::string>& command)
{
    std::list<std::string>::const_iterator iter = command.begin();

    while(iter != command.end())
    {
        sendCommand(*iter);
        ++iter;
    }
}

UnixProcess::ExitStatus UnixProcess::cleanup()
{
    ExitStatus exitStatus = UnixProcess::NormalExit;

    if(ps)
    {
        if(ps->is_open())
        {
            ps->flush();
            *ps << redi::peof;
            ps->close();
        }

        if(ps->rdbuf()->error() != errno)
        {
            exitStatus = UnixProcess::CrashExit;
        }

        delete ps;
        ps = 0;
    }

    if(iothread)
    {
        iothread->join();
        delete iothread;
        iothread = 0;
    }

    return exitStatus;
}

const std::string& UnixProcess::programPath() const
{
    return mProgramPath;
}

UnixProcess::ProcessError UnixProcess::error() const
{
    return mProcessError;
}

int	UnixProcess::exitCode() const
{
    return mExitCode;
}

UnixProcess::ExitStatus UnixProcess::exitStatus() const
{
    return mExitStatus;
}

std::string	UnixProcess::nativeArguments() const
{
    return mNativeArguments;
}

void UnixProcess::setNativeArguments(const std::string& arguments)
{
    this->mNativeArguments = arguments;
}

void UnixProcess::setWorkingDirectory(const std::string& dir)
{
    this->mWorkingDirectory = dir;
}

void UnixProcess::iorun()
{
    while(mProcessState == UnixProcess::Running)
    {
        std::cout << "READING IO FROM PROCESS!!!" << std::endl;

        std::string line;
        // read child's stdout
        while(std::getline(ps->out(), line))
        {
            readyReadStandardOutput(line);
            std::cout  << line << std::endl;
        }

        // read child's stderr
        while(std::getline(ps->err(), line))
        {
            readyReadStandardError(line);
            std::cerr << line << std::endl;
        }
    }
}

void UnixProcess::start(const std::string& arguments)
{
    mNativeArguments = arguments;
    ps = new pstream(mProgramPath.append(arguments), UnixProcess::all3streams);

    if(ps->is_open())
    {
        setProcessState(UnixProcess::Running);
        iothread = new boost::thread(boost::bind(&UnixProcess::iorun, this));
    }

    else
    {
        setProcessState(UnixProcess::NotRunning);
        mProcessError = UnixProcess::FailedToStart;
        errored(UnixProcess::FailedToStart);
    }
}

UnixProcess::ProcessState UnixProcess::state() const
{
    return mProcessState;
}

void UnixProcess::waitForFinished()
{
    setProcessState(UnixProcess::NotRunning);
    mExitStatus = cleanup();
    finished(mExitStatus);
}

void UnixProcess::waitForStarted()
{

}

std::string	UnixProcess::workingDirectory() const
{
    return mWorkingDirectory;
}

void UnixProcess::close()
{
    waitForFinished();
}

void UnixProcess::kill()
{
    ps->rdbuf()->kill();
    waitForFinished();
}

void UnixProcess::terminate()
{
    waitForFinished();
}

void UnixProcess::setProcessState(ProcessState state)
{
    this->mProcessState = state;
    stateChanged(state);
}

void UnixProcess::setupChildProcess()
{

}

} // sc namespace
