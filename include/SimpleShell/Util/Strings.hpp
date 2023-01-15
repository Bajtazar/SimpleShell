#pragma once

#include <string>
#include <vector>

namespace shell {

    std::string ltrim(std::string const& string);

    std::string rtrim(std::string const& string);

    std::string trim(std::string const& string);

    std::vector<std::string> split(std::string string);

    std::vector<std::string> splitOn(std::string string, char term);

    std::vector<std::string> splitOnQuotes(std::string string, char quote);

    std::vector<std::string> splitOnBrackets(std::string string, char open, char close);

    std::vector<std::string> advancedSplitter(std::string const& string, char splitter);

}
