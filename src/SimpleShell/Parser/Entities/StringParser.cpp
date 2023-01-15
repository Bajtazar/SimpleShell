#include <SimpleShell/Parser/Entities/StringParser.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>

#include <algorithm>
#include <regex>

namespace shell {


    void StringParser::registerCallbacks(
        [[maybe_unused]] ParsingEntityMap const& parsinMap) {}

    std::any StringParser::operator() (std::string const& command) {
        if (isQuotedString(command))
            return std::any{ praseQuotedString(std::next(command.begin()), std::prev(command.end())) };
        else if (isInvalidQuotedString(command))
            throw ParsingException{"\"" + command + "\" string is invalid"};
        return command;
    }

    bool StringParser::isQuotedString(std::string const& quoted) const {
        return quoted.front() == '\'' && quoted.back() == '\'';
    }

    bool StringParser::isInvalidQuotedString(std::string const& quoted) const {
        return quoted.front() == '\'' || quoted.back() == '\'';
    }

    std::string StringParser::praseQuotedString(Iter iter, Iter const& sent) const {
        using namespace std::literals;

        for (std::string string; iter != sent; ++iter) {
            auto const next = std::find(iter, sent, '\\');
            string += std::string{iter, next};
            if (next == sent)
                return string;
            iter = std::next(next);
            if (auto value = parseAdvancedChar(*iter))
                string.push_back(*value);
            else
                throw ParsingException{"The \"\\"s + *iter + "\" char is invalid"s};
        }
        return "";
    }

    std::optional<char> StringParser::parseAdvancedChar(char nextChar) const {
        switch (nextChar) {
            case '\\':
                return '\\';
            case '\'':
                return '\'';
            case 'n':
                return '\n';
            case '0':
                return '\0';
            case 't':
                return '\t';
            default:
                break;
        }
        return std::nullopt;
    }

}
