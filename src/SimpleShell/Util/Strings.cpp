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

    std::string removeComments(std::string const& string, char comment) {
        return { string.begin(), std::ranges::find(string, comment) };
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

    std::vector<std::string> splitOn(std::string string, char term) {
        std::vector<std::string> splitted;
        auto prev = string.begin();
        auto iter = std::ranges::find(string, term);
        for (; iter != string.end(); iter = std::ranges::find(iter, string.end(), term)) {
            splitted.emplace_back(prev, iter);
            prev = ++iter;
        }
        if (prev != string.end())
            splitted.emplace_back(prev, iter);
        return splitted;
    }

    std::vector<std::string> splitOnQuotes(std::string string, char quote) {
        string = trim(string);
        std::vector<std::string> result;
        auto iter = string.begin();
        while (iter != string.end()) {
            auto quoteIter = std::find(iter, string.end(), quote);
            if (quoteIter != iter)
                result.emplace_back(iter, quoteIter);
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
            iter = std::next(quoteEnd);
            result.emplace_back(quoteIter, iter);
        }
        return result;
    }

    std::vector<std::string> splitOnBrackets(std::string string, char open, char close) {
        string = trim(string);
        std::vector<std::string> ranges;
        auto iter = string.begin();
        auto prev = iter;
        for (;iter < string.end(); ++iter) {
            if (*iter != open)
                continue;
            if (iter != prev)
                ranges.emplace_back(prev, iter);
            prev = iter;
            size_t stack = 1;
            for (++iter; stack && iter < string.end(); ++iter) {
                if (*iter == open)
                    ++stack;
                else if (*iter == close) {
                    if (not --stack) {
                        ranges.emplace_back(prev, ++iter);
                        prev = iter;
                    }
                }
            }
            if (stack)
                throw std::runtime_error{"Invalid brackets"};
        }
        if (prev != iter && prev < string.end())
            ranges.emplace_back(prev, iter);
        return ranges;
    }

    std::vector<std::string> advancedSplitter(std::string const& string, char splitter) {
        std::vector<std::string> buf1;
        for (auto const& s1 : splitOnBrackets(string, '(', ')'))
            for (auto const& s2 : splitOnQuotes(s1, '\''))
                buf1.push_back(s2);
        std::vector<std::string> results;
        std::string buffer;
        for (auto const& str : buf1) {
            if (str.front() != '\'' && str.front() != '(') {
                auto split = splitOn(str, splitter);
                if (split.size() > 1) {
                    buffer += std::move(split.front());
                    results.push_back(std::move(buffer));
                    for (size_t i = 1; i < split.size() - 1; ++i)
                        results.push_back(std::move(split[i]));
                    buffer = std::move(split.back());
                } else if (split.size() && str.back() == ';') {
                    buffer += std::move(split.front());
                    results.push_back(std::move(buffer));
                    buffer = std::string{};
                } else
                    buffer += str;
            } else
                buffer += str;
        }
        if (not buffer.empty())
            results.push_back(std::move(buffer));
        return results;
    }

}
