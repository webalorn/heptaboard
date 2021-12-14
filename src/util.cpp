// TODO : exceptions
// TODO : IO

#include "headers/util.hpp"
#include <stdlib.h>

using namespace std;

IOMod::IOMod(Code pCode) : code(pCode) {}

ostream& operator<<(ostream& os, const IOMod& mod) {
	return os << "\033[" << mod.code << "m";
}
ostream& operator<<(ostream& os, const EndOfLineObj&) {
	return os << "\033[0m" << endl;
}
ostream& operator<<(ostream& os, const vector<IOMod>& modList) {
	for (const auto& mod : modList) {
		os << mod;
	}
	return os;
}


/*
	Exceptions
*/

const char* HeptException::what() const throw() {
	return this->errorMsg.c_str();
}
const string& HeptException::get() const {
	return this->errorMsg;
}
vector<IOMod> HeptException::bashFormat() const {
	return {IOMod(FG_RED)};
}
HeptException::HeptException(string msg) : errorMsg("[ERROR] " + msg) {}

ostream& operator<<(ostream& os, const HeptException& e) {
	return os << e.bashFormat() << e.get() << EOL;
}

CardDeviceError::CardDeviceError(string msg) : HeptException(msg) {}


/*
	Commands & boards
*/

void assertCommandExists(string cmdName, string requirement) {
	string whichCmd = "which " + cmdName + " > /dev/null 2>&1";
	if (system(whichCmd.c_str())) {
		if (requirement.size()) {
			cmdName = cmdName + " (" + requirement + ")";
		}
		throw HeptException("The command " + cmdName + " is required but doesn't exists. Please install check the heptaboard requirements");
	}
}

std::vector<fs::path> listPossibleBoards() {
	std::vector<fs::path> bordPaths;
	std::string devicesPath = "/dev";
	for (const auto& entry : fs::directory_iterator(devicesPath)) {
		auto path = entry.path();
		if (path.filename().string().rfind("cu.", 0) == 0) {
			if (path.filename().string().find("usb") != string::npos) {
				bordPaths.push_back(path);
			}
		}
	}
	return bordPaths;
}