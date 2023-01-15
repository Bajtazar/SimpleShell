#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>

namespace shell {

    class ProgramParser : public ParsingEntity {
    public:
        explicit ProgramParser(void) = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;
    private:
        ParsingEntity* statementParser = nullptr;
    };

}
