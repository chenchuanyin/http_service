#include "Application.h"
#include "Log.h"


int main(void) {
    Application application("default.conf");
    if (application.initialize()) {
        application.run();
    }
    getchar();
    return 0;
}
