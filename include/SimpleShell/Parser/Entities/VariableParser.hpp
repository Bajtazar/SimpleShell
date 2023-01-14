#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>

#include <optional>

namespace shell {

    class VariableParser : public ParsingEntity {
    public:
        explicit VariableParser(void) = default;

        void registerCallbacks(ParsingEntityMap const& parsinMap) override;

        std::any operator() (std::string const& command) override;

        static bool isVariable(std::string const& command) noexcept;
    private:
        std::optional<std::string> getLocalVariable(std::string const& variable) const;

        std::optional<std::string> getEnviromentalVariable(std::string const& variable) const;
    };

}
