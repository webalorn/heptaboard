#include "headers/config.hpp"

using namespace std;
using namespace TCLAP;

const char* VERSION = "0.0";
const uint MAX_TIMER_FREQ = 62500;

fs::path CompilerConfig::getHeptTmpDirectory() {
	return outputFile + ".hptc.tmp";
}

void CompilerConfig::checkEntryPoints() {
	for (auto entryFile : this->heptFiles) {
		fs::path cFileName = fs::path(entryFile).filename().replace_extension("c");
		fs::path cFilePath = this->getHeptTmpDirectory()
			/ (fs::path(entryFile).stem().string() + "_c") / cFileName;
		string fileIdentifier = cFileName.stem().string();
		fileIdentifier[0] = toupper(fileIdentifier[0]);

		string mainEntryPoint = fileIdentifier + "__main";
		string setupEntryPoint = fileIdentifier + "__setup";
		
		if (this->entryPoint == "" && isStringInFile(cFilePath, mainEntryPoint + "_step")) {
			this->entryPointHasMem = isStringInFile(cFilePath, mainEntryPoint + "_mem*");
			this->entryFileCompiled = cFilePath.string();
			this->entryPoint = mainEntryPoint;
		}
		if (this->setupPoint == "" && isStringInFile(cFilePath, setupEntryPoint + "_step")) {
			this->setupPointHasMem = isStringInFile(cFilePath, setupEntryPoint + "_mem*");
			this->setupFileCompiled = cFilePath.string();
			this->setupPoint = setupEntryPoint;
		}
	}
	if (this->entryPoint == "") {
		throw HeptException("Can't find the node 'main'");
	}
}

void CompilerConfig::readCmdArgs(int argc, char** argv) {
	CmdLine cmd("Heptaboard compiler", ' ', VERSION);

	ValueArg<string> argsGccTArg("", "args-gcc", "Arguments passed to avr-gcc", false, this->argsGcc, "arv-gcc args", cmd);
	ValueArg<string> argsLinkTArg("", "args-link", "Arguments passed to avr-gcc when used as a linker", false, this->argsLink, "linker args", cmd);
	ValueArg<string> argsHeptagonTArg("", "args-hpt", "Arguments passed to heptagon", false, this->argsHeptagon, "heptagon args", cmd);
	ValueArg<string> outputFileTArg("o", "output", "Name used by the output files", false, this->outputFile, "output-name", cmd);

	SwitchArg sendToBoardTArg("s", "send", "Send the compiled program to the arduino board", cmd, this->sendToBoard);
	// SwitchArg listBoardsTArg("l", "list", "List all connected arduino boards", cmd, this->listBoards);

	ValueArg<string> mmcuTArg("m", "mmcu", "mmcu (board type, defaults to " + this->mmcu + ")", false, this->mmcu, "board-mmcu", cmd);
	ValueArg<string> boardDeviceTArg("b", "board", "board device (file descriptor path ; required if '--send' is set)", false, this->boardDevice, "device path", cmd);
	ValueArg<string> boardConstructorTArg("", "constructor", "board constructor (board type, defaults to " + this->boardConstructor + ")", false, this->boardConstructor, "arduino/...", cmd);

	ValueArg<ull> clockSpeedTArg("", "cpu-clock", "Speed of the clock of the board", false, 16000000UL, "clock speed", cmd);
	ValueArg<uint> freqTArg("f", "freq", "Number of iterations of the main loop each second, should be <= " + to_string(MAX_TIMER_FREQ), false, 0, "frequency", cmd);
	ValueArg<uint> loopDelayTArg("d", "delay", "Time between two iteration of the main loop (in milliseconds)", false, 0, "time interval", cmd);

	MultiArg<string> cFilesTArg("c", "c-file", "C file compiled with the program", false, "C file", cmd);
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

	this->sendToBoard = sendToBoardTArg.getValue();
	// this->listBoards = listBoardsTArg.getValue();
	this->mmcu = mmcuTArg.getValue();
	this->boardDevice = boardDeviceTArg.getValue();
	this->boardConstructor = boardConstructorTArg.getValue();
	this->clockSpeed = to_string(clockSpeedTArg.getValue());

	this->timerFreq = freqTArg.getValue();

	if (freqTArg.isSet() && loopDelayTArg.isSet()) {
		throw CardDeviceError("Can't set the --delay option and the --freq option together");
	} else if (loopDelayTArg.isSet()) {
		if (loopDelayTArg.getValue() < 1) {
			throw CardDeviceError("Can't set a loop delay lower than 1ms");
		}
		if (loopDelayTArg.getValue() > 1000) {
			throw CardDeviceError("Can't set a loop delay greater than 1000ms");
		}
		this->timerFreq = 1000 / loopDelayTArg.getValue();
	}

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

	if (this->timerFreq == 0) {
		cout << IOMod(FG_CYAN) << "[INFO] The time frequency is set to 0, the program will run as fast as possible" << EOL;
	}
	if (this->timerFreq > MAX_TIMER_FREQ) {
		cout << IOMod(FG_CYAN) << "[INFO] Can't set a frequency greater than " << MAX_TIMER_FREQ << EOL;
	}

	assertPathExists(this->cFiles, true);
	assertPathExists(this->heptFiles, true);
}