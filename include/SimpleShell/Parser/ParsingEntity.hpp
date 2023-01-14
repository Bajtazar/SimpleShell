#pragma once

#include <any>
#include <map>
#include <string>
#include <memory>
#include <functional>

namespace shell {

    struct ParsingEntity {
        using ParsingEntityPtr = std::unique_ptr<ParsingEntity>;
        using ParsingEntityMap = std::map<std::string, ParsingEntityPtr>;

        explicit ParsingEntity(void) noexcept = default;

        ParsingEntity(ParsingEntity&&) = delete;

        ParsingEntity& operator=(ParsingEntity&&) = delete;

        virtual void registerCallbacks(ParsingEntityMap const& parsinMap) = 0;

        virtual std::any operator() (std::string const& command) = 0;

        virtual ~ParsingEntity(void) noexcept = default;
    };

}
