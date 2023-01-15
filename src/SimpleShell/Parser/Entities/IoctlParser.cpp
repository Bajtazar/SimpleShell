#include <SimpleShell/Parser/Entities/IoctlParser.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>

#include <algorithm>

extern "C" {
    #include <fcntl.h>
}

namespace shell {

    IoctlParser::Descrpitors::Descrpitors(void) :
        stdIn{dup(STDIN_FILENO)},
        stdOut{dup(STDOUT_FILENO)},
        stdErr{dup(STDERR_FILENO)} {}

    IoctlParser::Descrpitors::~Descrpitors(void) {
        close(stdIn);
        close(stdOut);
        close(stdErr);
    }

    void IoctlParser::registerCallbacks(ParsingEntityMap const& parsinMap) {
        stringParser = parsinMap.at("string").get();
    }

    std::any IoctlParser::operator() (std::string const& command) {
        if (isFileRedirection(command))
            return parseRedirection(command);
        throw ParsingException{"Given command is not a valid io redirection"};
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
        if (Stream::isStream(left) && Stream::isStream(right))
            return streamToStream(left, right, direction);
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
            [link, desc=descriptors, direction]() {
                if (*link == ERROR)
                    return;
                std::fflush(direction == OUT ? stdout : stdin);
                if (direction == OUT)
                    dup2(desc.stdOut, STDOUT_FILENO);
                else
                    dup2(desc.stdIn, STDIN_FILENO);
                close(*link);
            }
        };
    }

    IoctlParser::Result IoctlParser::streamToStream(std::string const& left,
                                                    std::string const& right,
                                                    bool direction) const
    {
        Stream leftDesc{left};
        Stream rightDesc{right};
        if (direction == OUT)
            std::swap(leftDesc, rightDesc);
        return {
            [leftDesc, rightDesc, desc=descriptors]() {
                dup2(leftDesc.get(desc), rightDesc.set(desc));
            },
            [rightDesc, desc=descriptors] {
                dup2(rightDesc.get(desc), rightDesc.set(desc));
            }
        };
    }

    IoctlParser::Stream::Stream(std::string const& stream) :
        isReference{stream.front() == '&'}
    {
        descriptor = std::stoi(
            isReference ? std::string{std::next(stream.begin()), stream.end()}
                        : stream);
        if (descriptor < 0 || descriptor > 2)
            throw ParsingException{"The stream descriptor has to be in the range from 0 to 2"};
    }

    [[nodiscard]] IoctlParser::descriptor_t IoctlParser::Stream::get(
        Descrpitors const& desc) const noexcept
    {
        if (isReference)
            return descriptor;
        switch (descriptor) {
            case STDIN_FILENO:
                return desc.stdIn;
            case STDOUT_FILENO:
                return desc.stdOut;
            case STDERR_FILENO:
                return desc.stdErr;
            default:
                break;
        }
        return 0;
    }

    [[nodiscard]] IoctlParser::descriptor_t IoctlParser::Stream::set(
        Descrpitors const& desc) const noexcept
    {
        return descriptor;
    }


    [[nodiscard]] bool IoctlParser::Stream::isStream(std::string const& stream) noexcept {
        if (stream.front() == '&')
            return stream.size() == 2 && std::isdigit(stream.back());
        return stream.size() == 1 && std::isdigit(stream.front());
    }

}
