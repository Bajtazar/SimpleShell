#include <SimpleShell/Parser/Parser.hpp>

namespace shell {

    Parser::Parser(void) {
        for (auto& [key, entity] : parsingMap)
            entity->registerCallbacks(parsingMap);
    }

    void Parser::registerEntity(
        std::string const& entityName,
        ParsingEntityPtr&& entity)
    {
        parsingMap.emplace(entityName, std::move(entity));
    }

    int32_t Parser::operator()(std::string const& command) {
        (*parsingMap.at("program"))(command);
        return 0;
    }

    Parser::ParsingEntityMap Parser::parsingMap{};

}
