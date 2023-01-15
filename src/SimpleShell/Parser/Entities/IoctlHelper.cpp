#include "IoctlHelper.hpp"

#include <algorithm>

namespace shell {

    ArgsAndIoctl getIoctlRange(std::vector<std::string>&& parsed) {
        static constexpr auto isIoctl = [](std::string const& str) -> bool {
            if (str.front() == '\'')
                return false;
            return std::ranges::count(str, '<') || std::ranges::count(str, '>');
        };

        std::vector<std::string> args;
        std::vector<std::string> ioctl;
        for (auto&& string : parsed) {
            if (isIoctl(string))
                ioctl.push_back(std::move(string));
            else
                args.push_back(std::move(string));
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
