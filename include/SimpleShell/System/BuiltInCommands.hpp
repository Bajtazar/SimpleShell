#pragma once

#include <functional>
#include <string>
#include <vector>
#include <optional>
#include <map>

namespace shell {

    class BuiltInCommands {
    public:
        using Args = std::vector<std::string>;
        using Invocable = std::function<void(Args const&)>;

        [[nodiscard]] std::optional<Invocable> operator()(
            std::string const& name) const;

    private:
        static std::map<std::string, Invocable> builtIns;
    };

    inline constexpr BuiltInCommands builtins;

}
