#include "headers/config.hpp"

using namespace std;
using namespace TCLAP;

const char* VERSION = "0.0";

fs::path CompilerConfig::getHeptTmpDirectory() {
	return outputFile + ".hptc.tmp";
}

void CompilerConfig::parseEntryPoint(string label) {
	size_t separator = label.find("::");
	string file, func;

	if (separator == std::string::npos) {
		this->entryFile = this->heptFiles[0];
		file = fs::path(this->entryFile).stem().string();
		func = label;
	} else {
		file = label.substr(0, separator);
		func = label.substr(separator+2, string::npos);

		for (const string& pathStr : this->heptFiles) {
			if (fs::path(pathStr).stem().string() == file) {
				this->entryFile = pathStr;
				break;
			}
		}
	}
	if (this->entryFile.size() == 0) {
		throw HeptException("The file " + file + ".ept doesn't exists");
	}
	file[0] = toupper(file[0]);
	this->entryPoint = file + "__" + func;
}

void CompilerConfig::checkEntryPoint() {
	fs::path cFileName = fs::path(this->entryFile).filename().replace_extension("c");
	fs::path cFilePath = this->getHeptTmpDirectory()
		/ (fs::path(this->entryFile).stem().string() + "_c") / cFileName;

	if (!isStringInFile(cFilePath, this->entryPoint + "_step")) {
		throw HeptException("The entry point " + this->entryPoint + " doesn't exists in the file " + cFilePath.string());
	}
	this->entryPointHasMem = isStringInFile(cFilePath, this->entryPoint + "_mem*");
	this->entryFileCompiled = cFilePath.string();
}

void CompilerConfig::readCmdArgs(int argc, char** argv) {
	CmdLine cmd("Heptaboard compiler", ' ', VERSION);

	ValueArg<string> argsGccTArg("", "args-gcc", "Arguments passed to avr-gcc", false, this->argsGcc, "arv-gcc args", cmd);
	ValueArg<string> argsLinkTArg("", "args-link", "Arguments passed to avr-gcc when used as a linker", false, this->argsLink, "linker args", cmd);
	ValueArg<string> argsHeptagonTArg("", "args-hpt", "Arguments passed to heptagon", false, this->argsHeptagon, "heptagon args", cmd);
	ValueArg<string> outputFileTArg("o", "output", "Name used by the output files", false, this->outputFile, "output-name", cmd);
	ValueArg<string> entryPointTArg("e", "entry", "Entry point of the heptagon program, in the form filename::function (h1::main for the main function in h1.ept). If the file is not given, the first heptagon file is used", true, this->outputFile, "filename::function", cmd);

	SwitchArg sendToBoardTArg("s", "send", "Send the compiled program to the arduino board", cmd, this->sendToBoard);
	// SwitchArg listBoardsTArg("l", "list", "List all connected arduino boards", cmd, this->listBoards);

	ValueArg<string> mmcuTArg("m", "mmcu", "mmcu (board type, defaults to " + this->mmcu + ")", false, this->mmcu, "board-mmcu", cmd);
	ValueArg<string> boardDeviceTArg("b", "board", "board device (file descriptor path ; required if '--send' is set)", false, this->boardDevice, "device path", cmd);
	ValueArg<string> boardConstructorTArg("c", "constructor", "board constructor (board type, defaults to " + this->boardConstructor + ")", false, this->boardConstructor, "arduino/...", cmd);
	ValueArg<long long> clockSpeedTArg("", "cpu-clock", "Speed of the clock of the board", false, 16000000UL, "clock speed", cmd);

	MultiArg<string> cFilesTArg("f", "c-file", "C file compiled with the program", true, "C file", cmd);
	UnlabeledMultiArg<string> heptFilesTArg("heptagon-files", "heptagon file names", true, "List of paths", false);
	
	// Parse the argv array.
	cmd.add(heptFilesTArg);
	cmd.parse( argc, argv );

	// Get the value parsed by each arg.
	this->argsGcc = argsGccTArg.getValue();
	this->argsLink = argsLinkTArg.getValue();
	this->argsHeptagon = argsHeptagonTArg.getValue();
	this->outputFile = outputFileTArg.getValue();

	this->cFiles = cFilesTArg.getValue();
	this->heptFiles = heptFilesTArg.getValue();
	parseEntryPoint(entryPointTArg.getValue());

	this->sendToBoard = sendToBoardTArg.getValue();
	// this->listBoards = listBoardsTArg.getValue();
	this->mmcu = mmcuTArg.getValue();
	this->boardDevice = boardDeviceTArg.getValue();
	this->boardConstructor = boardConstructorTArg.getValue();
	this->clockSpeed = to_string(clockSpeedTArg.getValue());

	/* Do some checks */
	if (this->sendToBoard) {
		if (!boardDeviceTArg.isSet()) {
			auto devices = listPossibleBoards();
			if (devices.size() == 1) {
				this->boardDevice = devices[0];
				cout << IOMod(FG_CYAN) << "[INFO] The board device is not set, trying with "
					<< this->boardDevice << EOL;
			} else {
				throw CardDeviceError("If the --send option is used, the --board option must be set");
			}
		}
	}

	assertPathExists(this->cFiles, true);
	assertPathExists(this->heptFiles, true);
}