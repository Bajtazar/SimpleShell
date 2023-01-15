#include <SimpleShell/Parser/Entities/ProgramParser.hpp>
#include <SimpleShell/System/ChildProcess.hpp>
#include <SimpleShell/System/Command.hpp>

namespace shell {

    void ProgramParser::registerCallbacks(ParsingEntityMap const& parsinMap) {
        expressionParser = parsinMap.at("expression").get();
    }

    std::any ProgramParser::operator() (std::string const& command) {
        auto invocable = std::any_cast<Command>((*expressionParser)(command));
        if (invocable.isExternalProgram())
            ChildProcess process{std::move(invocable)};
        else
            invocable();
        return 0;
    }

}
