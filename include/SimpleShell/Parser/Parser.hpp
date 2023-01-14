#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>

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

}
