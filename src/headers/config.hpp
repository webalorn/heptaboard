#if !defined(ARGS_HPP)
#define ARGS_HPP

#include <string>
#include <vector>
#include <tclap/CmdLine.h>

#include "util.hpp"

using ull = unsigned long long;

struct CompilerConfig {
    std::string argsGcc = "";
    std::string argsLink = "";
    std::string argsHeptagon = "";

    std::string entryFileCompiled, setupFileCompiled;
    std::string entryPoint = "", setupPoint = "";
    bool entryPointHasMem, setupPointHasMem;

    std::vector<std::string> cFiles;
    std::vector<std::string> heptFiles;
    std::vector<std::string> cFromHeptFiles;
    std::string outputFile = "arduino_reactive_program";
    
    bool listBoards = false;

    bool sendToBoard = false;
    std::string boardDevice = "";
    std::string mmcu = "atmega328p";
    std::string clockSpeed;
    std::string boardConstructor = "arduino";

    uint timerFreq = 0;

    void readCmdArgs(int argc, char** argv);
    void parseEntryPoint(std::string label);
    void checkEntryPoints();
    fs::path getHeptTmpDirectory();
};

#endif // ARGS_HPP