#include <SimpleShell/Parser/Entities/ExpressionParser.hpp>
#include <SimpleShell/Util/Strings.hpp>
#include <SimpleShell/System/ChildProcess.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>
#include "IoctlHelper.hpp"

#include <algorithm>

namespace shell {

    void ExpressionParser::registerCallbacks(ParsingEntityMap const& parsinMap) {
        statementParser = parsinMap.at("statement").get();
        programParser = parsinMap.at("program").get();
        ioctlParser = parsinMap.at("ioctl").get();
    }

    std::any ExpressionParser::operator() (std::string const& command) {
        if (std::ranges::find(command, '(') != command.end())
            return createSubshellExecution(command);
        return (*statementParser)(command);
    }

    Command ExpressionParser::createSubshellExecution(std::string const& command) const {
        auto [args, ioctl] =
            getIoctlRange(splitOnBrackets(command, '(', ')'), '(');

        if (args.size() > 1)
            throw ParsingException{"Invalid subshell syntax"};
        Command package {
            [program=programParser, cmnd=args.front()]([[maybe_unused]] Command::Args const& args) {
                (*program)({ std::next(cmnd.begin()), std::prev(cmnd.end()) });
            }
        };
        package.setCallbacks(generateIoctlCallbacks(ioctl, ioctlParser));
        package.setExternalExecution();
        return package;
    }

}
