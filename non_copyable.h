#ifndef NON_COPYABLE_H
#define NON_COPYABLE_H

namespace graphix {
namespace engine {
namespace utilities {

class NonCopyable {
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable& other) = delete;
    NonCopyable& operator=(const NonCopyable& other) = delete;
    NonCopyable(NonCopyable&& other) = default;
    NonCopyable& operator=(NonCopyable&& other) = default;
};

} /* namespace utilities */
} /* namespace engine */
} /* namespace graphix */

#endif /* NON_COPYABLE_H */
