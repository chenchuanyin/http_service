#include "AuthHelper.h"

#include "Environment.h"
#include "Log.h"


AuthHelper::AuthHelper() {
    try {
        needAuth_ = Environment::Instance().getInt("default.need_auth") == 1 ? true : false;
    } catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
    }

}

AuthHelper::~AuthHelper() {

}

void AuthHelper::auth(const std::string &appid,
                      const std::string &timestamp,
                      const std::string &appkey,
                      std::map<std::string, std::string> &result) {
    try {
        if (needAuth_) {
            //Todo
            return;
        } else {
            result["rc"] = "0";
            result["error"] = "";
            return;
        }
    } catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
    }
}
void AuthHelper::load_appkeys() {
    //Todo
}

void AuthHelper::connectToMySQL() {
    //Todo
}
