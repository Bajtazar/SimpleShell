#include <SimpleShell/System/BuiltInCommands.hpp>
#include <SimpleShell/System/LocalVariables.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>
#include <SimpleShell/System/Util.hpp>

#include <algorithm>
#include <stdexcept>

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
                if (args.size() > 1)
                    throw std::runtime_error{"Echo can only have 1 argument"};
                std::printf("%s\n", args[0].c_str());
            }},
            {"cd", [](Args const& args) {
                if (args.size() > 1)
                    throw std::runtime_error{"Cd can only have 1 argument"};
                if (chdir(args[0].c_str()) == -1)
                    throw std::runtime_error{"Invalid directory"};
            }},
            {"export", [](Args const& args){
                if (args.size() > 1)
                    throw std::runtime_error{"Export can only have 1 argument"};
                if (not isVariableSetter(args[0]))
                    throw ParsingException{"\"" + args[0] + "\" is not a valid variable setter"};
                auto splitPoint = std::find(args[0].begin(), args[0].end(), '=');
                auto status = setenv(
                    std::string{args[0].begin(), splitPoint}.c_str(),
                    std::string{std::next(splitPoint), args[0].end()}.c_str(),
                    false);
                if (status == -1)
                    throw std::runtime_error{"Variable cannot be exportex"};
            }},
            {"exit", [](Args const& args) {
                if (not args.empty())
                    throw std::runtime_error{"Exit does not take any argument"};
                exit(0);
            }}
    }};

}
