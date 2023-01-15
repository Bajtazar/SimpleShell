#include <SimpleShell/Parser/Entities/IoctlParser.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>

#include <algorithm>

extern "C" {
    #include <fcntl.h>
}

namespace shell {

    IoctlParser::IoctlParser(void) :
        stdIn{dup(STDIN_FILENO)},
        stdOut{dup(STDOUT_FILENO)},
        stdErr{dup(STDERR_FILENO)}
    {}

    void IoctlParser::registerCallbacks(ParsingEntityMap const& parsinMap) {
        stringParser = parsinMap.at("string").get();
    }

    std::any IoctlParser::operator() (std::string const& command) {
        if (isFileRedirection(command))
            return parseRedirection(command);
        throw ParsingException{"Given command is not a valid io redirection"};
    }

    IoctlParser::~IoctlParser(void) {
        close(stdIn);
        close(stdOut);
        close(stdErr);
    }

    bool IoctlParser::isFileRedirection(std::string const& command) const {
        return (std::ranges::count(command, '<') == 1) ^ (std::ranges::count(command, '>') == 1);
    }

    IoctlParser::Result IoctlParser::parseRedirection(std::string const& command) const {
        static constexpr auto isCtl = [](char c) -> bool {
            return c == '>' || c == '<';
        };

        auto iter = std::ranges::find_if(command, isCtl);
        std::string left{command.begin(), iter};
        std::string right{std::next(iter), command.end()};
        // if only for better readibility
        return getCtl(right, left, *iter == '>' ? OUT : IN);
    }

    IoctlParser::Result IoctlParser::getCtl(std::string const& left, std::string const& right, bool direction) const {
        if (right.empty())
            return fileAndProc(left, direction);
        if (left.empty())
            return fileAndProc(right, not direction);
        throw ParsingException{
            "The > and < tokens can be only used inside an explicit string "
            "or in io redirection context"};
    }

    IoctlParser::Result IoctlParser::fileAndProc(std::string const& fileName, bool direction) const {
        static constexpr descriptor_t ERROR = 0xFFFFFFFF;

        auto link = std::make_shared<descriptor_t>(0);
        return {
            [link, fileName, direction]() {
                *link = open(fileName.c_str(), direction == OUT ? O_WRONLY | O_CREAT | O_TRUNC : O_RDONLY, 0600);
                if (*link == -1) {
                    *link = ERROR;
                    return;
                }
                dup2(*link, direction == OUT ? STDOUT_FILENO : STDIN_FILENO);
            },
            [link, stdIn=this->stdIn, stdOut=this->stdOut, direction]() {
                if (*link == ERROR)
                    return;
                std::fflush(direction == OUT ? stdout : stdin);
                if (direction == OUT)
                    dup2(stdOut, STDOUT_FILENO);
                else
                    dup2(stdIn, STDIN_FILENO);
                close(*link);
            }
        };
    }

    // DeferredIoctl IoctlParser::getCtl(std::string const& left, std::string const& right) const {
    //     if (right.empty())
    //         return fileToProc(left);
    //     if (left.empty())
    //         return procToFile(right);
    //     if (left.empty() && right.empty())
    //         throw ParsingException{
    //             "The > and < tokens can be only used inside an explicit string "
    //             "or in io redirection context"};
    //     return streamToStream(std::stoi(left), std::stoi(right));
    // }

    // DeferredIoctl IoctlParser::fileToProc(std::string const& fileName) const {
    //     auto file = open(fileName.c_str(), O_RDONLY);
    //     if (file == -1)
    //         throw std::runtime_error{"\"" + fileName + "\" File could not be open"};
    //     return DeferredIoctl{{{0, file}}, [file]() { close(file); }};
    // }

    // DeferredIoctl IoctlParser::procToFile(std::string const& fileName) const {
    //     auto file = open(fileName.c_str(), O_WRONLY);
    //     if (file == -1)
    //         throw std::runtime_error{"\"" + fileName + "\" File could not be open"};
    //     return DeferredIoctl{{{1, file}}, [file]() { close(file); }};
    // }

    // DeferredIoctl IoctlParser::streamToStream(descriptor_t left, descriptor_t right) const {
    //     return DeferredIoctl{{{left, right}}};
    // }

}
