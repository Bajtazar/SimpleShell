#pragma once

#include <functional>
#include <vector>

namespace shell {

    class ResourceGuard {
    public:
        using Invocable = std::function<void(void)>;
        using Result = std::pair<Invocable, Invocable>;

        explicit ResourceGuard(std::vector<Result>&& invocables);

        ResourceGuard(ResourceGuard const&) = delete;

        ResourceGuard& operator=(ResourceGuard const&) = delete;

        ~ResourceGuard(void);
    private:
        std::vector<Invocable> invocables;
    };

}
