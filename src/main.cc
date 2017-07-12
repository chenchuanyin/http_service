#include "Application.h"
#include "Log.h"


int main(void) {
    Application application("default.conf");
    if (application.initialize()) {
        application.run();
        LOG_INFO << "the server is running\n";
    }
    getchar();
    return 0;
}
