#include <SimpleShell/IO/Console.hpp>

#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <cstring>

extern "C" {
    #include <unistd.h>
}

namespace shell {

    Console::Console(void) :
        user{}, directory{}
    {
        actualizeUser();
        actualizeDirectory();
        if (user.empty() || directory.empty())
            throw std::runtime_error{"Unknown error during console init"};
    }

    void Console::printPrompt(void) {
        std::printf("%s@%s$", user.c_str(), directory.c_str());
    }

    void Console::actualizeUser(void) {
        if (char* user = getenv("USER"))
            this->user = std::string{user, user + std::strlen(user)};
        else
            this->user.clear();
    }

    void Console::actualizeDirectory(void) {
        char buffer[1024];
        if (buffer == getcwd(buffer, 1024))
            formatDirectoryStr({buffer, std::strlen(buffer)});
    }

    void Console::formatDirectoryStr(std::string_view directory) {
        auto home = getHome();
        auto const& [iter1, iter2] = std::ranges::mismatch(home, directory);
        if (iter2 != directory.end())
            this->directory = '~' + std::string{iter2, directory.end()};
        else if (iter2 == directory.end())
            this->directory = '~';
        else
            this->directory = directory;
    }

    std::string_view Console::getHome(void) {
        if (char* home = getenv("HOME"))
            return { home, std::strlen(home) };
        return {};
    }

}
