#pragma once

#include <array>
#include <string>

namespace RS::Interval {

    inline std::array<int, 3> version() noexcept {
        return {0, 3, 0};
    }

    inline std::string version_string() {
        return "0.3.0";
    }

}
