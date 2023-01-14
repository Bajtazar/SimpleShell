#include <SimpleShell/Util/Strings.hpp>

#include <algorithm>
#include <ranges>

namespace shell {

    static bool isspace(char c) noexcept {
        return std::isspace(c);
    }

    std::string ltrim(std::string const& string) {
        auto iter = std::ranges::find_if_not(string, isspace);
        return { iter, string.end() };
    }

    std::string rtrim(std::string const& string) {
        auto iter = std::ranges::find_if_not(string | std::views::reverse, isspace);
        return { string.begin(), iter.base() };
    }

    std::string trim(std::string const& string) {
        return ltrim(rtrim(string));
    }

    std::vector<std::string> split(std::string string) {
        std::vector<std::string> splitted;
        string = ltrim(string);
        while (not string.empty()) {
            auto next = std::ranges::find_if(string, isspace);
            splitted.emplace_back(string.begin(), next);
            string.erase(string.begin(), next);
            string = ltrim(string);
        }
        return splitted;
    }

    std::vector<std::string> splitOnQuotes(std::string string, char quote) {
        string = trim(string);
        std::vector<std::string> result;
        auto iter = string.begin();
        while (iter != string.end()) {
            auto quoteIter = std::find(iter, string.end(), quote);
            if (quoteIter != iter)
                std::ranges::move(split({iter, quoteIter}), std::back_inserter(result));
            auto quoteEnd = quoteIter;
            if (quoteIter == string.end())
                return result;
            while (quoteEnd != string.end()) {
                quoteEnd = std::find(std::next(quoteEnd), string.end(), quote);
                if (*std::prev(quoteEnd) != '\\')
                    break;
            }
            if (quoteEnd == string.end())
                throw std::runtime_error{"Invalid quoted range"};
            result.emplace_back(quoteIter, std::next(quoteEnd));
            iter = std::next(quoteEnd);
        }
        return result;
    }

}
