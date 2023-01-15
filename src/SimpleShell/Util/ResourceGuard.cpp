#include <SimpleShell/Util/ResourceGuard.hpp>

namespace shell {

    ResourceGuard::ResourceGuard(std::vector<Result>&& invocables) {
        for (auto&& [opener, closer] : invocables) {
            opener();
            this->invocables.push_back(std::move(closer));
        }
    }

    ResourceGuard::~ResourceGuard(void) {
        for (auto& closer : this->invocables)
            closer();
    }

}
