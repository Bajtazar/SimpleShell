#pragma once

#include <string>
#include <vector>

namespace shell {

    std::string ltrim(std::string const& string);

    std::string rtrim(std::string const& string);

    std::vector<std::string> split(std::string string);

}
