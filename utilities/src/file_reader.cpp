#include "file_reader.h"

#include <fstream>
#include <streambuf>
#include <stdexcept>

namespace graphix {
namespace utilities {

std::string FileReader::read_content(const std::string& filename) {
    std::ifstream file_stream{filename.c_str()};

    if (file_stream) {
        std::string file_content{std::istreambuf_iterator<char>{file_stream},
                                 std::istreambuf_iterator<char>{}};

        return file_content;
    }
    throw std::runtime_error{"Unable to read from file: " + filename +
                             ". Cannot open file."};
}

} /* namespace utilities */
} /* namespace graphix */
