#include <SimpleShell/Parser/Entities/StringParser.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>

#include <algorithm>
#include <regex>

#include <iostream>

namespace shell {


    void StringParser::registerCallbacks(
        [[maybe_unused]] ParsingEntityMap const& parsinMap) {}

    std::any StringParser::operator() (std::string const& command) {
        if (isQuotedString(command))
            return std::any{ praseQuotedString(std::next(command.begin()), std::prev(command.end())) };
        else if (isInvalidQuotedString(command))
            throw ParsingException{"\"" + command + "\" string is invalid"};
        return parseConsoleString(command);
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

    std::string StringParser::parseConsoleString(std::string const& string) const {
        static std::regex pattern{R"([a-zA-Z0-9_.,/\-]+)"};

        if (not std::regex_match(string, pattern))
            throw ParsingException{
                "\"" + string +
                "\" does not follow the string rules. Raw console string must "
                "satisfy the [a-zA-Z0-9_.,/\\-]+ regex"};
        return string;
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
