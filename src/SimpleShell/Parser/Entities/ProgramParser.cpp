#include <SimpleShell/Parser/Entities/ProgramParser.hpp>
#include <SimpleShell/System/ChildProcess.hpp>
#include <SimpleShell/System/Command.hpp>
#include <SimpleShell/Util/Strings.hpp>

namespace shell {

    void ProgramParser::registerCallbacks(ParsingEntityMap const& parsinMap) {
        pipeParser = parsinMap.at("pipe").get();
    }

    std::any ProgramParser::operator() (std::string const& command) {
        for (auto&& invocable : getCommands(trim(command)))
            invokeCommand(std::move(invocable));
        return 0;
    }

    void ProgramParser::invokeCommand(Command&& command) const {
        if (command.isExternalProgram())
            ChildProcess process{std::move(command)};
        else
            command();
    }

    std::vector<Command> ProgramParser::getCommands(std::string const& command) const {
        std::vector<Command> commands;
        for (auto const& sequence : advancedSplitter(command, ';'))
            commands.push_back(std::any_cast<Command>((*pipeParser)(sequence)));
        return commands;
    }

}
