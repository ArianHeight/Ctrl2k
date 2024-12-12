#include "setup.h"
#include <iostream>

void SetUpLoggingSystem(c_string projectName)
{
    gbt::LoggingStreamSettings fullSettings;
    fullSettings.levelFlags = (gbt::LogLevelFlag)(~gbt::LogLevelFlag::LOGLEVELFLAG_PROFILE);
    fullSettings.usePrefix = gbt::LogPrefix::LOGPREFIX_SHORT;
    fullSettings.logFullPath = false;
    fullSettings.showTextColour = false;
    fullSettings.useLogTime = gbt::LogTime::LOGTIME_HMS;
    REGISTER_FILE_FOR_LOGGING_CUSTOM(fullSettings, std::format("logs/{}.full.log", projectName));

    gbt::LoggingStreamSettings profileSettings;
    profileSettings.levelFlags = gbt::LogLevelFlag::LOGLEVELFLAG_PROFILE;
    profileSettings.usePrefix = gbt::LogPrefix::LOGPREFIX_NONE;
    profileSettings.logFullPath = false;
    profileSettings.showTextColour = false;
    profileSettings.showThreadId = false;
    profileSettings.useLogTime = gbt::LogTime::LOGTIME_HMS_S;
    REGISTER_FILE_FOR_LOGGING_CUSTOM(profileSettings, std::format("logs/{}.profile.log", projectName));

    // TODO could one day add cerr to just log errors. For now, not necessary
    REGISTER_LOGGING_STREAM_VERBOSITY(gbt::LogLevel::LOGLEVEL_MSG, gbt::LogVerbosity::LOGVERBOSITY_LOW, std::cout);
}
