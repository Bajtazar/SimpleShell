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

}
