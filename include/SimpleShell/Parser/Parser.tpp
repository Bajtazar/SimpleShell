#pragma once

namespace shell {

    template <std::derived_from<ParsingEntity> Tp>
    void registerEntity(std::string const& name) {
        Parser::registerEntity(name, std::make_unique<Tp>());
    }

}
