#include "setup.h"
#include <iostream>

void SetUpLoggingSystem(c_string projectName)
{
    gbt::LoggingStreamSettings fullSettings;
    fullSettings.levelFlags = (gbt::LogLevelFlag)(~gbt::LogLevelFlag::PROFILE);
    fullSettings.usePrefix = gbt::LogPrefix::SHORT;
    fullSettings.logFullPath = false;
    fullSettings.showTextColour = false;
    fullSettings.useLogTime = gbt::LogTime::HMS;
    REGISTER_FILE_FOR_LOGGING_CUSTOM(fullSettings, std::format("logs/{}.full.log", projectName).c_str());

    gbt::LoggingStreamSettings profileSettings;
    profileSettings.levelFlags = gbt::LogLevelFlag::PROFILE;
    profileSettings.usePrefix = gbt::LogPrefix::NONE;
    profileSettings.logFullPath = false;
    profileSettings.showTextColour = false;
    profileSettings.showThreadId = false;
    profileSettings.useLogTime = gbt::LogTime::HMS_S;
    REGISTER_FILE_FOR_LOGGING_CUSTOM(profileSettings, std::format("logs/{}.profile.log", projectName).c_str());

    // TODO could one day add cerr to just log errors. For now, not necessary
    REGISTER_LOGGING_STREAM_VERBOSITY(gbt::LogLevel::MSG, gbt::LogVerbosity::LOW, std::cout);
}
