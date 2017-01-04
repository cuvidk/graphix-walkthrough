#include "file_reader.h"

#include <fstream>
#include <streambuf>
#include <stdexcept>

namespace graphix {
namespace engine {
namespace utilities {

std::string FileReader::read_content(const std::string& filename) {
    std::ifstream file_stream{filename.c_str()};

    if (file_stream) {
        std::string file_content{std::istreambuf_iterator<char>{file_stream},
                                 std::istreambuf_iterator<char>{}};

        return file_content;
    }
    throw std::runtime_error{"Unable to read from file: " + filename +
                             ". File does not exist/ failed to open."};
}

} /* namespace utilities */
} /* namespace engine */
} /* namespace graphix */
