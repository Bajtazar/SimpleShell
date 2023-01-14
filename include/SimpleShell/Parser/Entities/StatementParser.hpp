#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>

namespace shell {

    class StatementParser : public ParsingEntity {
    public:
        using Result = std::function<void(void)>;

        explicit StatementParser(void) noexcept = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;

    private:
        ParsingEntity* commandParser = nullptr;
        ParsingEntity* variableParser = nullptr;
        ParsingEntity* stringParser = nullptr;
    };

}
