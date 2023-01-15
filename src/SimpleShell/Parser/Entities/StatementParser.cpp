#include <SimpleShell/Parser/Entities/StatementParser.hpp>
#include <SimpleShell/Parser/Entities/CommandParser.hpp>
#include <SimpleShell/Util/Strings.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>

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
        auto [parsed, ioctl] = getIoctlRange(splitOnQuotes(command, '\''));

        if (parsed.empty())
            return Command{[]([[maybe_unused]] Args const& args) {}};

        auto statement = std::any_cast<Command>((*commandParser)(parsed.front()));
        statement.setArgs(getArgs(parsed));
        statement.setCallbacks(generateIoctlCallbacks(ioctl));

        return statement;
    }

    StatementParser::Args StatementParser::getArgs(Args const& parsed) const {
        Args args;
        for (auto& str : parsed | std::views::drop(1))
          args.push_back(std::any_cast<std::string>(
              (*(str.front() == '$' ? variableParser : stringParser))(str)));

        return args;
    }

    StatementParser::ArgsAndIoctl StatementParser::getIoctlRange(Args&& parsed) const {
        static constexpr auto isIoctl = [](std::string const& str) -> bool {
            if (str.front() == '\'')
                return false;
            return std::ranges::count(str, '<') || std::ranges::count(str, '>');
        };

        Args args;
        Args ioctl;
        for (auto&& string : parsed) {
            if (isIoctl(string))
                ioctl.push_back(std::move(string));
            else
                args.push_back(std::move(string));
        }
        return { args, ioctl };
    }

    StatementParser::Callbacks StatementParser::generateIoctlCallbacks(Args const& ioctl) const {
        Callbacks callbacks;
        for (auto const& string : ioctl)
            callbacks.push_back(std::any_cast<Callback>((*ioctlParser)(string)));
        return callbacks;
    }

}
