#include "ProxyApplication.h"
#include "Log.h"

int main(int argc, char **argv) {
    try {
        ProxyApplication app("default.conf");
        app.init();
        app.run(argc, argv);
    } catch (Poco::Exception &e) {
        std::cerr << "some error:  " << e.what() << std::endl;
    }
    return 0;
}
