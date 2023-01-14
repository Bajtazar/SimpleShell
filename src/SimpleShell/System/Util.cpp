#include <SimpleShell/System/Util.hpp>

#include <regex>

namespace shell {

    [[nodiscard]] bool isVariableSetter(std::string const& command) noexcept {
        static const std::regex setterRegex{R"([A-Za-z_][_A-Za-z0-9]+=[_A-Za-z0-9]*)"};

        return std::regex_match(command, setterRegex);
    }

}
