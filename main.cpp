#include "logger.h"
#include "file_reader.h"

#include <iostream>

int main() {
    using namespace graphix::utilities;
    Logger::initialize("opengl.log");
    try {

        std::string content = FileReader::read_content("../shaders/fragment_shader.glsl");

        std::cout << content.c_str() << std::endl;

        LOG(TRACE) << "HELLO BOY\n";
        LOG(DEBUG) << "HELLO BOY\n";
        LOG(INFO) << "HELLO BOY\n";
        LOG(WARNING) << "HELLO BOY\n";
        LOG(ERROR) << "HELLO BOY\n";
        LOG(FATAL) << "HELLO BOY\n";
    } catch (std::exception& e) {
        LOG(FATAL) << e.what() << "\n";
    }

    return 0;
}
