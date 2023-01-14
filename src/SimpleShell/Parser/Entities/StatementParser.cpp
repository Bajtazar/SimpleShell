#include <SimpleShell/Parser/Entities/StatementParser.hpp>
#include <SimpleShell/Parser/Entities/CommandParser.hpp>
#include <SimpleShell/Util/Strings.hpp>

#include <ranges>

namespace shell {

    void StatementParser::registerCallbacks(ParsingEntityMap const& parsinMap) {
        commandParser = parsinMap.at("command").get();
        variableParser = parsinMap.at("variable").get();
        stringParser = parsinMap.at("string").get();
    }

    std::any StatementParser::operator()(std::string const& command) {
        auto parsed = splitOnQuotes(command, '\'');
        std::vector<std::string> args;
        auto statement =
            std::any_cast<Command>((*commandParser)(parsed.front()));
        for (auto& str : parsed | std::views::drop(1))
          args.push_back(std::any_cast<std::string>(
              (*(str.front() == '$' ? variableParser : stringParser))(str)));
        statement.setArgs(std::move(args));

        return statement;
    }

}
