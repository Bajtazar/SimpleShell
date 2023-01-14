#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>

#include <optional>

namespace shell {

    class StringParser : public ParsingEntity {
    public:
        explicit StringParser(void) = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;
    private:
        using Iter = typename std::string::const_iterator;

        bool isQuotedString(std::string const& quoted) const;

        bool isInvalidQuotedString(std::string const& quoted) const;

        std::string praseQuotedString(Iter iter, Iter const& sent) const;

        std::string parseConsoleString(std::string const& string) const;

        std::optional<char> parseAdvancedChar(char nextChar) const;
    };

}
