#include "Environment.h"

#include "Log.h"

Environment::Environment() : isInitialized_(false) {
}

Environment::~Environment() {
}

bool Environment::initialize(std::string filename) {
    configFile_ = new Poco::Util::IniFileConfiguration(filename);
    if (NULL == configFile_) {
        isInitialized_ = false;
    }
    isInitialized_ = true;
    return isInitialized_;
}

std::string Environment::getString(const std::string &key) {
    try {
        return configFile_->getString(key);
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
    }
}

int Environment::getInt(const std::string &key) {
    try {
        return configFile_->getInt(key);
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
    }

}

bool Environment::getBool(const std::string &key) {
    try {
        return configFile_->getBool(key);
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
    }
}
