#include <SimpleShell/Parser/Entities/ProgramParser.hpp>
#include <SimpleShell/System/ChildProcess.hpp>
#include <SimpleShell/System/Command.hpp>

namespace shell {

    void ProgramParser::registerCallbacks(ParsingEntityMap const& parsinMap) {
        pipeParser = parsinMap.at("pipe").get();
    }

    std::any ProgramParser::operator() (std::string const& command) {
        auto invocable = std::any_cast<Command>((*pipeParser)(command));
        if (invocable.isExternalProgram())
            ChildProcess process{std::move(invocable)};
        else
            invocable();
        return 0;
    }

}
