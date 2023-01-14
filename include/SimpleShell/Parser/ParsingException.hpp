#pragma once

#include <stdexcept>
#include <string>

namespace shell {

    class ParsingException : public std::exception {
    public:
        explicit ParsingException(std::string const& info) :
            info{"[Parsing Error]" + info} {}

        const char* what(void) const noexcept
            { return info.c_str(); }

    private:
        std::string info;
    };

}
