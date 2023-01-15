#pragma once

#include <SimpleShell/Parser/ParsingEntity.hpp>
#include <SimpleShell/System/Command.hpp>

namespace shell {

    using ArgsAndIoctl = std::pair<std::vector<std::string>, std::vector<std::string>>;

    ArgsAndIoctl getIoctlRange(std::vector<std::string>&& parsed, char escape);

    Command::Callbacks generateIoctlCallbacks(
        std::vector<std::string> const& ioctl, ParsingEntity* ioctlParser);
}
