#include <string>
#include <iostream>
#include <algorithm>

#include "headers/config.hpp"
#include "headers/util.hpp"

using namespace std;

int main(int argc, char** argv) {
	
	try {
		// Check if the required programs are installed
		assertCommandExists("heptc", "heptagon");
		assertCommandExists("avr-gcc");
		assertCommandExists("avr-objcopy");
		assertCommandExists("avrdude");

		// Read the command line arguments
		CompilerConfig conf;
		conf.readCmdArgs(argc, argv);

		if (conf.sendToBoard) {
			assertCommandExists("arduino-cli");
		}

		// TODO : find the files
		// TODO : compile
		// TODO : avrdude

	} catch (TCLAP::ArgException& e) {
		cerr << IOMod(FG_RED) << "[ERROR] " << e.error() << " for arg " << e.argId() << EOL;
	} catch (CardDeviceError& e) {
		cerr << e;
		auto devices = listPossibleBoards();
		if (devices.size()) {
			cerr << "If you can't find the arduino device, here are some possible devices:" << endl;
			for (auto device : devices) {
				cerr << IOMod(FG_CYAN) << "- " << device.string() << EOL;
			}
		}
		cerr << "Check if your board is well connected. You can use the command 'arduino-cli board list' to list all connected arduino boards\n";
	} catch (HeptException& e) {
		cerr << e;
	}
}