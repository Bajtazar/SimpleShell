#include <SimpleShell/Parser/Entities/PipeParser.hpp>
#include <SimpleShell/Parser/ParsingException.hpp>
#include <SimpleShell/Util/Strings.hpp>
#include <SimpleShell/System/ChildProcess.hpp>

#include <algorithm>

extern "C" {
    #include <fcntl.h>
}

#include <iostream>

namespace shell {

    void PipeParser::registerCallbacks(ParsingEntityMap const& parsinMap) {
        expressionParser = parsinMap.at("expression").get();
    }

    std::any PipeParser::operator() (std::string const& command) {
        auto subcommands = splitOn(command, '|');
        if (subcommands.size() == 1)
            return (*expressionParser)(subcommands.front());
        std::vector<Command> commands;
        for (auto const& subcommand : subcommands)
            commands.push_back(std::any_cast<Command>((*expressionParser)(subcommand)));
        createPipes(commands);
        return Command{ [cmds=std::move(commands)]([[maybe_unused]] Command::Args const& args) mutable {
            std::vector<ChildProcess> processess;
            for (auto& invocable : cmds)
                processess.emplace_back(invocable);
            for (size_t i = 0;i < cmds.size(); ++i)
                if (processess[i].waitFor())
                    cmds[i].closePipeDesc();
        }};
    }

    void PipeParser::createPipes(std::vector<Command>& commands) const {
        std::vector<PipeCB> out;
        std::vector<PipeCB> in;
        for (std::size_t i = 1; i < commands.size(); ++i) {
            auto const& [l1, l2] = createPipeContext();
            out.push_back(l1);
            in.push_back(l2);
        }
        commands.front().setPipeCallbacks({ out.front() });
        for (std::size_t i = 1; i < commands.size() - 1; ++i) {
            commands[i].setPipeCallbacks({ out[i], in[i - 1] });
        }
        commands.back().setPipeCallbacks({ in.back() });
    }

    PipeParser::PipeContext PipeParser::createPipeContext(void) const {
        static constexpr auto NOFUC = []() {};

        int fd[2]{};
        if (pipe2(fd, O_CLOEXEC)) throw std::runtime_error{"Cannot create a pipe"};
        return {{[desc = fd[1]]() { dup2(desc, STDOUT_FILENO); }, [desc=fd[1]](){ close(desc); }},
                {[desc = fd[0]]() { dup2(desc, STDIN_FILENO); }, [desc=fd[0]](){ close(desc); }}};
    }

}
