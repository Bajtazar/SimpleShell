#pragma once

#include <stdexcept>

namespace shell {

    template <std::invocable Invocable>
    ChildProcess::ChildProcess(Invocable invocable) :
        childID{fork()}
    {
        if (childID > 0)
            return;
        if (not childID) {
            try {
                invocable();
            } catch (std::exception const& except) {
                std::puts(except.what());
            }
            exit(0);
        }
        throw std::runtime_error{"A new child process cannot be forked"};
    }

}
