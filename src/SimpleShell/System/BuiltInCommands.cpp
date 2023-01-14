#include <SimpleShell/System/BuiltInCommands.hpp>
#include <SimpleShell/System/LocalVariables.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>

#include <algorithm>
#include <stdexcept>
#include <regex>

extern "C" {
    #include <unistd.h>
}

namespace shell {

    [[nodiscard]] std::optional<typename BuiltInCommands::Invocable>
        BuiltInCommands::operator()(
            std::string const& name) const
    {
        if (builtIns.contains(name))
            return builtIns.at(name);
        return std::nullopt;
    }

    std::map<std::string, typename BuiltInCommands::Invocable>
        BuiltInCommands::builtIns{{
            {"echo", [](Args const& args) {
                if (args.size() > 2)
                    throw std::runtime_error{"Echo can only have 1 argument"};
                std::printf("%s\n", args[1].c_str());
            }},
            {"cd", [](Args const& args) {
                if (args.size() > 2)
                    throw std::runtime_error{"Cd can only have 1 argument"};
                if (chdir(args[1].c_str()) == -1)
                    throw std::runtime_error{"Invalid directory"};
            }},
            {"export", [](Args const& args){
                static const std::regex variableRegex{R"((A-Za-z_)[_A-Za-z0-9]+=[_A-Za-z0-9]+)"};
                if (args.size() > 2)
                    throw std::runtime_error{"Export can only have 1 argument"};
                if (not std::regex_match(args[1], variableRegex))
                    throw ParsingException{"\"" + args[1] + "\" is not a valid variable setter"};
                auto splitPoint = std::find(args[1].begin(), args[1].end(), '=');
                auto status = setenv(
                    std::string{args[1].begin(), splitPoint}.c_str(),
                    std::string{std::next(splitPoint), args[1].end()}.c_str(),
                    false);
                if (status == -1)
                    throw std::runtime_error{"Variable cannot be exportex"};
            }}
    }};

}
