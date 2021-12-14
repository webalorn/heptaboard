#include "headers/compiler.hpp"

using namespace std;

const uintmax_t G_SIZE = 1e9;
const uintmax_t M_SIZE = 1e6;
const uintmax_t K_SIZE = 1e3;

string compileC(CompilerConfig& conf) {
    vector<string>& cFiles = conf.cFiles;

    // Compile individual files
    vector<fs::path> compiledObjFiles;
    for (const string& filePathStr : cFiles) {
        fs::path filePath(filePathStr);
        fs::path objPath = filePath;
        objPath.replace_extension("o");
        
        SysCommand gccCmd("avr-gcc");
        gccCmd << "-c" << "-std=gnu99" << "-Os" << ("-DF_CPU=" + conf.clockSpeed + "UL")
            << ("-mmcu=" + conf.mmcu) << "-ffunction-sections" << "-fdata-sections"
            << "-I/usr/local/include/heptaboard" << conf.argsGcc
            << "-o" << objPath << filePath;
        
        if (!gccCmd.exec()) {
            throw CompileError("Can't compile file " + filePathStr);
        }
        compiledObjFiles.push_back(objPath);
    }
    
    // Link files together
    SysCommand gccCmd("avr-gcc");
    gccCmd << "-Os" <<  "-ffunction-sections" << "-fdata-sections" << "-Wl,--gc-sections"
        << ("-mmcu=" + conf.mmcu) << conf.argsLink
        << "-o" << (conf.outputFile + ".elf");
    
    for (const fs::path& objFile : compiledObjFiles) {
        gccCmd << objFile;
    }
    gccCmd << "-L/usr/local/lib" << "-lheptaboard";
    if (!gccCmd.exec()) {
        throw CompileError("Can't link obj files");
    }

    // Extracting hex file
    string hexFile = conf.outputFile + ".hex";
    SysCommand objcopyCmd("avr-objcopy");
    objcopyCmd << "-O" << "ihex" << "-R" << ".eeprom"
        << (conf.outputFile + ".elf") << hexFile;
    if (!objcopyCmd.exec()) {
        throw CompileError("Can't extract hex file");
    }

    // Printing the size of the hex file
    uintmax_t hexSize = fs::file_size(conf.outputFile + ".hex");
    cout << IOMod(FG_GREEN) << "Size of the program: ";
    if (hexSize >= G_SIZE) {
        cout << (hexSize / G_SIZE) << "." << ((hexSize * 10 / G_SIZE)%10) << "Gb";
    } else if (hexSize >= M_SIZE) {
        cout << (hexSize / M_SIZE) << "." << ((hexSize * 10 / M_SIZE)%10) << "Mb";
    } else if (hexSize >= K_SIZE) {
        cout << (hexSize / K_SIZE) << "." << ((hexSize * 10 / K_SIZE)%10) << "Kb";
    } else {
        cout << hexSize << "b";
    }
    cout << EOL << endl;

    return hexFile;
}

void sendProgToBoard(CompilerConfig& conf, std::string hexFile) {
    SysCommand avrdudeCmd("avrdude");
    avrdudeCmd << "-c" << conf.boardConstructor << "-p" << conf.mmcu
        << "-P" << conf.boardDevice
        << "-U" << ("flash:w:\"" + hexFile + "\":i");

    if (!avrdudeCmd.exec()) {
        throw CompileError("Can't send the program to the board");
    }
}