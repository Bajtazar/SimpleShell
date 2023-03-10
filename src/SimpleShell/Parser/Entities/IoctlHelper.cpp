#include "IoctlHelper.hpp"
#include <SimpleShell/Util/Strings.hpp>

#include <algorithm>

namespace shell {

    ArgsAndIoctl getIoctlRange(std::vector<std::string>&& parsed, char escape) {
        auto isIoctl = [escape](std::string const& str) -> bool {
            if (str.front() == escape)
                return false;
            return std::ranges::count(str, '<') || std::ranges::count(str, '>');
        };

        std::vector<std::string> args;
        std::vector<std::string> ioctl;
        for (auto&& string : parsed) {
            if (isIoctl(string))
                ioctl.push_back(trim(string));
            else
                args.push_back(trim(string));
        }
        return { args, ioctl };
    }

    Command::Callbacks generateIoctlCallbacks(std::vector<std::string> const& ioctl, ParsingEntity* ioctlParser) {
        Command::Callbacks callbacks;
        for (auto const& string : ioctl)
            callbacks.push_back(std::any_cast<Command::CBPack>((*ioctlParser)(string)));
        return callbacks;
    }

}
