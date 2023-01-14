#pragma once

#include <string>

namespace shell {

    [[nodiscard]] bool isVariableSetter(std::string const& command) noexcept;

}
