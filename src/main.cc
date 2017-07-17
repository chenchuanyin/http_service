#include "Application.h"
#include "Log.h"


int main(void) {
    Application application("default.conf");
    if (application.init()) {
        application.run();
        LOG_INFO << "服务开启\n";
    }
    getchar();
    return 0;
}
