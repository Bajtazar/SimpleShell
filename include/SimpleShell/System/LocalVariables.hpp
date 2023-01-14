#pragma once

#include <map>
#include <optional>
#include <string>

namespace shell {

    class LocalVariables {
    public:
        static void setVariable(
            std::string const& variable,
            std::string const& value);

        static std::optional<std::string> getVariable(
            std::string const& variable);
    private:
        static std::map<std::string, std::string> variables;
    };

}
