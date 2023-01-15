#include <SimpleShell/Parser/Entities/IoctlParser.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>

#include <algorithm>

extern "C" {
    #include <fcntl.h>
}

namespace shell {

    void IoctlParser::registerCallbacks(ParsingEntityMap const& parsinMap) {
        stringParser = parsinMap.at("string").get();
    }

    std::any IoctlParser::operator() (std::string const& command) {
        if (not isFileRedirection(command))
            return parseFileRedirection(command);
        throw ParsingException{"Given command is not a valid io redirection"};
    }

    bool IoctlParser::isFileRedirection(std::string const& command) const {
        return (std::ranges::count(command, '<') == 1) ^ (std::ranges::count(command, '>') == 1);
    }

    DeferredIoctl IoctlParser::parseFileRedirection(std::string const& command) const {
        auto isCtl = [](char c) -> bool {
            return c == '>' || c == '<';
        };
        auto iter = std::ranges::find_if(command, isCtl);
        std::string left{command.begin(), iter};
        std::string right{std::next(iter), command.end()};
        if (*iter == '>')
            return getCtl(left, right);
        return getCtl(right, left);
    }

    DeferredIoctl IoctlParser::getCtl(std::string const& left, std::string const& right) const {
        if (right.empty())
            return fileToProc(left);
        if (left.empty())
            return procToFile(right);
        if (left.empty() && right.empty())
            throw ParsingException{
                "The > and < tokens can be only used inside an explicit string "
                "or in io redirection context"};
        return streamToStream(std::stoi(left), std::stoi(right));
    }

    DeferredIoctl IoctlParser::fileToProc(std::string const& fileName) const {
        auto file = open(fileName.c_str(), O_RDONLY);
        if (file == -1)
            throw std::runtime_error{"\"" + fileName + "\" File could not be open"};
        return DeferredIoctl{{{0, file}}, [file]() { close(file); }};
    }

    DeferredIoctl IoctlParser::procToFile(std::string const& fileName) const {
        auto file = open(fileName.c_str(), O_WRONLY);
        if (file == -1)
            throw std::runtime_error{"\"" + fileName + "\" File could not be open"};
        return DeferredIoctl{{{1, file}}, [file]() { close(file); }};
    }

    DeferredIoctl IoctlParser::streamToStream(descriptor_t left, descriptor_t right) const {
        return DeferredIoctl{{{left, right}}};
    }

}
