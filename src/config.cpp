#include "headers/config.hpp"
#include "headers/util.hpp"

using namespace std;
using namespace TCLAP;

const char* VERSION = "0.0";

void CompilerConfig::readCmdArgs(int argc, char** argv) {
	CmdLine cmd("Heptaboard compiler", ' ', VERSION);

	ValueArg<std::string> argsGccTArg("", "args-gcc", "Arguments passed to avr-gcc", false, this->argsGcc, "string", cmd);
	ValueArg<std::string> argsHeptagonTArg("", "args-hpt", "Arguments passed to heptagon", false, this->argsHeptagon, "string", cmd);
	ValueArg<std::string> outputFileTArg("o", "output", "Name used by the output files", false, this->outputFile, "string", cmd);

	SwitchArg sendToBoardTArg("s", "send", "Send the compiled program to the arduino board", cmd, this->sendToBoard);
	SwitchArg listBoardsTArg("l", "list", "List all connected arduino boards", cmd, this->listBoards);

	ValueArg<std::string> mmcuTArg("m", "mmcu", "mmcu (board type, defaults to " + this->mmcu + ")", false, this->mmcu, "string", cmd);
	ValueArg<std::string> boardDeviceTArg("b", "board", "board device (required if '--send' is set)", false, this->boardDevice, "string", cmd);
	ValueArg<std::string> boardConstructorTArg("c", "constructor", "board constructor (board type, defaults to " + this->boardConstructor + ")", false, this->boardConstructor, "string", cmd);

	UnlabeledMultiArg<string> cFilesTArg("c-files", "C file names", true, "List of paths", false);
	
	// Parse the argv array.
	cmd.add(cFilesTArg);
	cmd.parse( argc, argv );

	// Get the value parsed by each arg.
	this->argsGcc = argsGccTArg.getValue();
	this->argsHeptagon = argsHeptagonTArg.getValue();
	this->outputFile = outputFileTArg.getValue();

	this->cFiles = cFilesTArg.getValue();

	this->sendToBoard = sendToBoardTArg.getValue();
	this->listBoards = listBoardsTArg.getValue();
	this->mmcu = mmcuTArg.getValue();
	this->boardDevice = boardDeviceTArg.getValue();
	this->boardConstructor = boardConstructorTArg.getValue();

	/* Do some checks */
	if (this->sendToBoard) {
		if (this->boardDevice.size() == 0) {
			throw CardDeviceError("If the --send option is used, the --board option must be set");
		}
	}
}