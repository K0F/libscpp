#include "scprocess.h"

namespace sc {

ScProcess* ScProcess::internal = 0;
const std::string ScProcess::sclangPath = "/usr/local/bin/sclang";

ScProcess::ScProcess(const std::vector<sclang_response_t>& startupResponses) :
    UnixProcess(ScProcess::sclangPath)
{
    init();

    for(int i = 0; i < startupResponses.size(); ++i)
    {
        started.connect(startupResponses.at(i));
    }
}


ScProcess::ScProcess(sclang_response_t startupResponse) :
    UnixProcess(ScProcess::sclangPath)
{
    init();
    started.connect(startupResponse);
}

ScProcess::ScProcess() :
    UnixProcess(ScProcess::sclangPath)
{
    init();
}

void ScProcess::init()
{
    readyReadStandardOutput.connect(boost::bind(&ScProcess::onReadyRead, this, _1));
    readyReadStandardError.connect(boost::bind(&ScProcess::onReadyRead, this, _1));
    stateChanged.connect(boost::bind(&ScProcess::onProcessStateChanged, this, _1));
    mTerminationRequested = false;

    ScProcess::internal = this;
    addResponse("startup", boost::bind(&ScProcess::onStart, this));
}

void ScProcess::addResponse(std::string name, sclang_response_t response)
{
    std::string prefix("__LIBSCPP_SIGNAL__");
    sclangResponseMap[prefix.append(name)] = response;
}

void ScProcess::removeResponse(std::string name)
{
    std::string prefix("__LIBSCPP_SIGNAL__");
    sclangResponseMap.erase(prefix.append(name));
}

void ScProcess::compileFile(std::string filePath)
{
    std::string scCommand = "LibSCPP.compileFile(";
    scCommand.append("\"");
    scCommand.append(filePath);
    scCommand.append("\");");
    sendCommand(scCommand);
}


void ScProcess::toggleRunning()
{
    switch(state())
    {
    case UnixProcess::NotRunning:
        startLanguage();
        break;

    default:
        stopLanguage();
        break;
    }
}

void ScProcess::startLanguage()
{
    if(state() != UnixProcess::NotRunning)
    {
        std::cout << "SCLang Interpreter is already running." << std::endl;
        return;
    }

    start();
}

void ScProcess::stopLanguage()
{
    if(state() != UnixProcess::Running)
    {
        std::cout << "SCLang Interpreter is not running!" << std::endl;
        return;
    }

    mTerminationRequested = true;
    mTerminationRequestTime = boost::posix_time::second_clock::universal_time();
    waitForFinished();
    mTerminationRequested = false;
}

void ScProcess::restartLanguage()
{
    stopLanguage();
    startLanguage();
}

void ScProcess::recompileClassLibrary()
{
    // write("\x18");
}

void ScProcess::stopMain()
{

}

void ScProcess::evaluateCode(std::string const & commandString, bool silent)
{
    if(state() != UnixProcess::Running)
    {
        std::cout << "SCLang Interpreter is not running!" << std::endl;
        return;
    }


}

void ScProcess::onNewIpcConnection()
{

}

void ScProcess::onIpcData()
{

}

void ScProcess::finalizeConnection()
{

}

void ScProcess::onProcessStateChanged(UnixProcess::ProcessState state)
{

}

void ScProcess::onReadyRead(std::string output)
{
    boost::unordered_map<std::string, sclang_response_t>::iterator find = sclangResponseMap.find(output);

    if(find != sclangResponseMap.end())
    {
        find->second(); // Call the reponse
    }

    else
    {
        scPost(output);
    }
}

void ScProcess::updateToggleRunningAction()
{

}

void ScProcess::onStart()
{
    started();
}

void ScProcess::onResponse(const std::string& selector, const std::string& data)
{

}

} // sc
