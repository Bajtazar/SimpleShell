#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>
#include <concepts>

namespace shell {

    class Parser {
    public:
        using ParsingEntityPtr = typename ParsingEntity::ParsingEntityPtr;

        explicit Parser(void);

        static void registerEntity(
            std::string const& entityName,
            ParsingEntityPtr&& entity);

        int32_t operator()(std::string const& command);

    private:
        using ParsingEntityMap = typename ParsingEntity::ParsingEntityMap;

        static ParsingEntityMap parsingMap;
    };

    template <std::derived_from<ParsingEntity> Tp>
    void registerEntity(std::string const& name);

}

#include <SimpleShell/Parser/Parser.tpp>
