#include "logger.h"

int main() {
    Logger::initialize("opengl.log");

    LOG(TRACE) << "HELLO BOY\n";
    LOG(DEBUG) << "HELLO BOY\n";
    LOG(INFO) << "HELLO BOY\n";
    LOG(WARNING) << "HELLO BOY\n";
    LOG(ERROR) << "HELLO BOY\n";
    LOG(FATAL) << "HELLO BOY\n";
    return 0;
}
