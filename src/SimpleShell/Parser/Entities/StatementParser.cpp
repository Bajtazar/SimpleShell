#include <SimpleShell/Parser/Entities/StatementParser.hpp>
#include <SimpleShell/Parser/Entities/CommandParser.hpp>
#include <SimpleShell/Util/Strings.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>
#include "IoctlHelper.hpp"

#include <algorithm>
#include <ranges>

namespace shell {

    void StatementParser::registerCallbacks(ParsingEntityMap const& parsinMap) {
        commandParser = parsinMap.at("command").get();
        variableParser = parsinMap.at("variable").get();
        stringParser = parsinMap.at("string").get();
        ioctlParser = parsinMap.at("ioctl").get();
    }

    std::any StatementParser::operator()(std::string const& command) {
        std::vector<std::string> splitted;
        for (auto const& spl : splitOnQuotes(command, '\''))
            for (auto&& x : split(spl))
                splitted.push_back(std::move(x));

        auto [parsed, ioctl] = getIoctlRange(std::move(splitted), '\'');

        if (parsed.empty())
            return Command{[]([[maybe_unused]] Args const& args) {}};

        auto statement = std::any_cast<Command>((*commandParser)(parsed.front()));
        statement.setArgs(getArgs(parsed));
        statement.setCallbacks(generateIoctlCallbacks(ioctl, ioctlParser));

        return statement;
    }

    StatementParser::Args StatementParser::getArgs(Args const& parsed) const {
        Args args;
        for (auto& str : parsed | std::views::drop(1))
          args.push_back(std::any_cast<std::string>(
              (*(str.front() == '$' ? variableParser : stringParser))(str)));

        return args;
    }

}
