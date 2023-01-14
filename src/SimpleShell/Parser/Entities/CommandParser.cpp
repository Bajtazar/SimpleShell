#include <SimpleShell/Parser/Entities/CommandParser.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>
#include <SimpleShell/System/BuiltInCommands.hpp>
#include <SimpleShell/System/LocalVariables.hpp>
#include <SimpleShell/System/Util.hpp>

#include <algorithm>
#include <filesystem>

extern "C" {
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
}

#include <iostream>

namespace shell {


    Command::Command(Invocable invocable) :
        state{std::move(invocable)} {}

    Command::Command(std::string const& path) :
        state{path} {}

    std::string Command::getPureCommand(void) {
        auto const& command = std::get<std::string>(state);
        auto iter = std::ranges::find(command, '/');
        if (iter == command.end())
            return command;
        return { iter, command.end() };
    }

    char** Command::prepareArgTable(Args const& args) {
        char** table = new char*[args.size() + 2]{};
        auto const command = getPureCommand();
        table[0] = new char[command.size() + 1]{};
        std::ranges::copy(command, table[0]);
        for (size_t i = 1; auto const& arg : args) {
            table[i] = new char[arg.size() + 1]{};
            std::ranges::copy(arg, table[i++]);
        }
        table[args.size() + 1] = nullptr;
        return table;
    }

    void Command::operator()(Args const& args) {
        if (std::holds_alternative<Invocable>(state))
            std::get<Invocable>(state)(args);
        else {
            execvp(std::get<std::string>(state).c_str(), prepareArgTable(args));
        }
    }

    void CommandParser::registerCallbacks(
        [[maybe_unused]] ParsingEntityMap const& parsinMap) {}

    std::any CommandParser::operator() (std::string const& command) {
        if (auto invocable = builtins(command))
            return Command{*invocable};
        if (isVariableSetter(command))
            return generateVariableSetter(command);
        if (isSpecialDot(command))
            return Command{generateSpecialDotPath(command)};
        if (isUsrBinExec(command))
            return Command{generateUsrBinPath(command)};
        if (isPathBasedExec(command))
            return Command{command};
        throw std::runtime_error{"\"" + command + "\" is not a valid command"};
    }

    bool CommandParser::isSpecialDot(std::string const& command) const {
        if (command.front() == '.')
            return doesExecExist({ std::next(command.begin()), command.end() });
        return false;
    }

    bool CommandParser::isUsrBinExec(std::string const& command) const {
        if (std::ranges::find(command, '/') != command.end())
            return false;
        return doesExecExist("/usr/bin/" + command);
    }

    bool CommandParser::isPathBasedExec(std::string const& command) const {
        if (std::ranges::find(command, '/') == command.end())
            return false;
        return doesExecExist(command);
    }

    bool CommandParser::doesExecExist(std::string const& path) const {
        if (not std::filesystem::exists(path))
            return false;
        struct stat stats{};
        return not stat(path.c_str(), &stats) && stats.st_mode & S_IXUSR;
    }

    Command CommandParser::generateVariableSetter(std::string const& command) const {
        auto const split = std::ranges::find(command, '=');
        std::string variable{command.begin(), split};
        std::string value{std::next(split), command.end()};
        return Command{[variable = std::move(variable),
                        value = std::move(value)]
                        (Command::Args const& args)
            {
                if (not args.empty())
                    throw ParsingException{
                        "Too many arguments to set the local variable"};
                LocalVariables::setVariable(variable, value);
            }
        };
    }

    std::string CommandParser::generateSpecialDotPath(std::string const& command) const {
        return "./" + std::string{ std::next(command.begin()), command.end() };
    }

    std::string CommandParser::generateUsrBinPath(std::string const& command) const {
        return "/usr/bin/" + command;
    }

}
