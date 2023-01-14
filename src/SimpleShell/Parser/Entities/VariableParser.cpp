#include <SimpleShell/Parser/Entities/VariableParser.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>
#include <SimpleShell/System/LocalVariables.hpp>

#include <cstring>

extern "C" {
    #include <unistd.h>
}

namespace shell {

    void VariableParser::registerCallbacks(
        [[maybe_unused]] ParsingEntityMap const& parsinMap) {}

    std::any VariableParser::operator() (std::string const& command) {
        if (not isVariable(command))
            throw ParsingException{"$" + command + " does not exist"};
        std::string commandName{std::next(command.begin()), command.end()};
        if (auto local = getLocalVariable(commandName))
            return *local;
        if (auto enviromental = getEnviromentalVariable(commandName))
            return *enviromental;
        throw ParsingException{"$" + commandName + " does not exist"};
    }

    bool VariableParser::isVariable(std::string const& command) noexcept {
        return not command.empty() && command.front() == '$';
    }

    std::optional<std::string> VariableParser::getLocalVariable(std::string const& variable) const {
        return LocalVariables::getVariable(variable);
    }

    std::optional<std::string> VariableParser::getEnviromentalVariable(std::string const& variable) const {
        if (char* env = getenv(variable.c_str()))
            return std::string{ env, env + std::strlen(env) };
        return std::nullopt;
    }

}
