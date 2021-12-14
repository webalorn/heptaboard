#if !defined(ARGS_HPP)
#define ARGS_HPP

#include <string>
#include <vector>
#include <tclap/CmdLine.h>

struct CompilerConfig {
    std::string argsGcc = "";
    std::string argsHeptagon = "";
    std::vector<std::string> cFiles;
    std::vector<std::string> heptFiles;
    std::string outputFile = "arduino_reactive_program";
    
    bool listBoards = false;

    bool sendToBoard = false;
    std::string mmcu = "atmega328p";
    std::string boardDevice = "";
    std::string boardConstructor = "arduino";

    void readCmdArgs(int argc, char** argv);
};

#endif // ARGS_HPP