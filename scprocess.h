#ifndef SCPROCESS_H
#define SCPROCESS_H

// STL
#include <string>
#include <cstdio>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

// Boost
#include <boost/format.hpp>
#include <boost/date_time.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
// #include <boost/signal.hpp>
#include "customboost/customsignals2.hpp"
#include <boost/bind.hpp>
#include <boost/unordered_map.hpp>

// libscpp
#include "process.h"

namespace sc {

typedef boost::function<void ()> sclang_response_t;

// A standard library / boost translation/reimagination of sc_process.hpp
// Used to create and communicated with a child SCLang process
class ScProcess : public UnixProcess
{
public:
    ScProcess(const std::vector<sclang_response_t>& startupResponses);
    ScProcess(sclang_response_t startupResponse);
    ScProcess();

    enum ActionRole
    {
        ToggleRunning = 0,
        Start,
        Stop,
        Restart,
        RecompileClassLibrary,
        StopMain,

        ActionCount
    };

    void sendRequest(const std::string& id, const std::string& command, const std::string& data)
    {
        std::stringstream cmd;
        cmd << boost::format("ScIDE.request(\"%1%\",'%2%',\"%3%\")") % id % command % data;

        evaluateCode(cmd.str(), true);
    }

    void addResponse(std::string name, sclang_response_t response);
    void removeResponse(std::string name);
    void compileFile(std::string filePath);

    // Slots
    void toggleRunning();
    void startLanguage(const std::string& arguments = "");
    void stopLanguage();
    void restartLanguage();
    void recompileClassLibrary();
    void stopMain();
    void evaluateCode(std::string const & commandString, bool silent = false);

    // Signals
    boost::signals2::signal<void (std::string const &)> scPost;
    boost::signals2::signal<void (const std::string &)> statusMessage;
    boost::signals2::signal<void (const std::string& selector, const std::string& data)> response;
    boost::signals2::signal<void ()> classLibraryCompiled;

    static ScProcess* internal;
    static const std::string sclangPath;

protected:

    void onNewIpcConnection();
    void onIpcData();
    void finalizeConnection();
    void onProcessStateChanged(UnixProcess::ProcessState state);
    void onReadyRead(std::string output);
    void updateToggleRunningAction();
    void onStart();
    void onResponse(const std::string& selector, const std::string& data);

    bool mTerminationRequested;
    boost::posix_time::ptime mTerminationRequestTime;
    boost::unordered_map<std::string, sclang_response_t> sclangResponseMap;

private:

    void init();
};

class ScRequest
{
public:

    ScRequest(ScProcess* sc) :
        mSc(sc)
    {
        // response.connect(boost::bind(&ScRequest::onResponse, this, _1, _2));
        mSc->classLibraryCompiled.connect(boost::bind(&ScRequest::cancel, this));
    }

    std::string idToString()
    {
        std::stringstream id;

        for(int i = 0; i < boost::uuids::uuid::static_size(); ++i)
        {

            id << mId.data[i] << " ";
        }

        return id.str();
    }

    void send(const std::string& command, const std::string& data)
    {
        mId = boost::uuids::random_generator()();
        mIdString = idToString();
        mCommand = command;
        mSc->sendRequest(mIdString, command, data);
    }

    void cancel()
    {
        mId = boost::uuids::uuid();
        mIdString.clear();
        cancelled();
    }

    // Signals
    boost::signals2::signal<void ()> cancelled;
    boost::signals2::signal<void (const std::string&, const std::string&)> response;

    void onResponse(const std::string& responseID, const std::string& responseData)
    {
        if(responseID.compare(mIdString) == 0)
        {
            response(mCommand, responseData);
        }
    }

private:

    std::string mCommand;
    boost::uuids::uuid mId;
    std::string mIdString;
    ScProcess* mSc;
};

} // sc namespace

#endif // SCPROCESS_H
