#include <string>
#include <array>

namespace RS::Intervals {

    inline std::array<int, 3> version() noexcept {
        return {{ 0, 1, 17 }};
    }

    inline std::string version_string() {
        return "0.1.17";
    }

}
