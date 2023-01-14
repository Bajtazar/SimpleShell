#include <SimpleShell/System/LocalVariables.hpp>

namespace shell {

    void LocalVariables::setVariable(
        std::string const& variable,
        std::string const& value)
    {
        if (variables.contains(variable))
            variables.at(variable) = value;
        else
            variables.emplace(variable, value);
    }

    std::optional<std::string> LocalVariables::getVariable(
        std::string const& variable)
    {
        if (not variables.contains(variable))
            return std::nullopt;
        return variables.at(variable);
    }

    std::map<std::string, std::string> LocalVariables::variables{};

}
