#include "headers/compiler.hpp"

using namespace std;

const uintmax_t G_SIZE = 1e9;
const uintmax_t M_SIZE = 1e6;
const uintmax_t K_SIZE = 1e3;

void compileHeptagon(CompilerConfig& conf) {
    fs::path tmpDirectory = fs::absolute(conf.getHeptTmpDirectory());
    if (fs::exists(tmpDirectory)) {
        fs::remove_all(tmpDirectory);
    }

    vector<fs::path> heptFiles;
    for (const string& filePathStr : conf.heptFiles) {
        heptFiles.push_back(fs::relative(fs::absolute(filePathStr), tmpDirectory));
    }

    fs::create_directory(tmpDirectory);
    fs::path prevWorkingPath = std::filesystem::current_path();
    std::filesystem::current_path(prevWorkingPath / tmpDirectory);

    SysCommand heptcCmd("heptc");
    heptcCmd << "-target" << "c" << "-I" << "/usr/local/include/heptaboard";
    for (const auto& filePath : heptFiles) {
        heptcCmd << filePath;
    }
    if (!heptcCmd.exec()) {
        throw CompileError("Can't compile the heptagon program");
    }

    std::filesystem::current_path(prevWorkingPath);

    for(auto& p: fs::recursive_directory_iterator(tmpDirectory)) {
        auto fPath = p.path();
        if (fs::is_regular_file(fPath) && fPath.extension().string() == ".c") {
            conf.cFromHeptFiles.push_back(fs::relative(fPath).string());
        }
    }

    conf.checkEntryPoint();
}

void setGccFlags(CompilerConfig& conf, SysCommand& cmd) {
    cmd << "-Os" << ("-DF_CPU=" + conf.clockSpeed + "UL")
            << ("-mmcu=" + conf.mmcu) << "-ffunction-sections" << "-fdata-sections"
            << "-I/usr/local/include/heptaboard/arduino" << "-I/usr/local/include/heptaboard/hept"
            << conf.argsGcc;
}

string compileC(CompilerConfig& conf) {
    vector<string> cFiles = conf.cFiles;
    cFiles.insert(cFiles.end(), conf.cFromHeptFiles.begin(), conf.cFromHeptFiles.end());
    string hexFile = conf.outputFile + ".hex";
    string elfFile = conf.outputFile + ".elf";

    // Compile individual C files to obj files
    SysCommand gccCmd("avr-gcc");
    vector<fs::path> compiledObjFiles;
    for (const string& filePathStr : cFiles) {
        fs::path filePath(filePathStr);
        fs::path objPath = filePath;
        objPath.replace_extension("o");
        
        setGccFlags(conf, gccCmd);
        gccCmd << "-c" << "-std=gnu99"<< "-o" << objPath << filePath;
        
        if (!gccCmd.exec()) {
            throw CompileError("Can't compile file " + filePathStr);
        }
        compiledObjFiles.push_back(objPath);
    }

    // Compile main file
    setGccFlags(conf, gccCmd);
    auto mainObj = conf.getHeptTmpDirectory() / "main.o";
    fs::path entryHeader = fs::path(conf.entryFileCompiled).replace_extension(".h");
    gccCmd << "-c" << "-std=c++17"<< "-o" << mainObj << "/usr/local/include/heptaboard/main.cpp"
        << ("-DENTRY_HEADER=\"\\\"" + absolute(entryHeader).string() + "\\\"\"")
        << ("-DENTRY=" + conf.entryPoint) << ("-DLOOP_DELAY=" + to_string(conf.loopDelay));
    if (conf.entryPointHasMem) {
        gccCmd << ("-DENTRY_MEM");
    }
    if (!gccCmd.exec()) {
        throw CompileError("Can't compile the main.cpp file");
    }
    compiledObjFiles.push_back(mainObj);
    
    // Link files together
    gccCmd << "-Os" <<  "-ffunction-sections" << "-fdata-sections" << "-Wl,--gc-sections"
        << ("-mmcu=" + conf.mmcu) << conf.argsLink
        << "-o" << elfFile;
    
    for (const fs::path& objFile : compiledObjFiles) {
        gccCmd << objFile;
    }
    gccCmd << "-L/usr/local/lib" << "-lheptaboard";
    if (!gccCmd.exec()) {
        throw CompileError("Can't link obj files");
    }

    // Extracting hex file
    SysCommand objcopyCmd("avr-objcopy");
    objcopyCmd << "-O" << "ihex" << "-R" << ".eeprom"
        << elfFile << hexFile;
    if (!objcopyCmd.exec()) {
        throw CompileError("Can't extract hex file");
    }
    fs::remove(elfFile);

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