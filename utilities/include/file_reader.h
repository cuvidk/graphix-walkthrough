#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>

namespace graphix {
namespace utilities {

class FileReader {
public:
    static std::string read_content(const std::string& filename);
};

} /* namespace utilities */
} /* namespace graphix */

#endif /* FILE_READER_H */
