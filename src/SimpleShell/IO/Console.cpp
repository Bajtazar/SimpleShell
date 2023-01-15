#include <SimpleShell/IO/Console.hpp>

#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <iostream>

extern "C" {
    #include <unistd.h>
}

namespace shell {

    Console::Console(void) :
        user{}, directory{}, host{getHostname()}
    {
        actualizeUser();
        actualizeDirectory();
        if (user.empty() || directory.empty())
            throw std::runtime_error{"Unknown error during console init"};
    }

    std::string Console::getHostname(void) {
        char hostname[1024];
        gethostname(hostname, 1024);
        return { hostname, std::strlen(hostname) };
    }

    [[nodiscard]] std::string Console::getCommand(void) {
        printPrompt();
        return getLineFromStdin();
    }

    void Console::printPrompt(void) {
        actualizeUser();
        actualizeDirectory();
        std::printf("[%s@%s]%s$ ", user.c_str(), host.c_str(),
                    directory.c_str());
    }

    [[nodiscard]] std::string Console::getLineFromStdin(void) {
        std::string command;
        std::getline(std::cin, command);
        if (command.back() == '\n')
            command.pop_back();
        return command;
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
        if (iter2 != directory.end() && iter1 == home.end())
            this->directory = '~' + std::string{iter2, directory.end()};
        else if (iter2 == directory.end() && iter1 == home.end())
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
