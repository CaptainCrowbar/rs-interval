#include <string>
#include <array>

namespace RS::Format {

    inline std::array<int, 3> version() noexcept {
        return {{ 0, 1, 8 }};
    }

    inline std::string version_string() {
        return "0.1.8";
    }

}
