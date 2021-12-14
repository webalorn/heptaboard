#if !defined(UTIL_HPP)
#define UTIL_HPP

#include <iostream>
#include <exception>
#include <vector>
#include <filesystem>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

// Code and IOMod are adapted from Thomas's code on https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
enum Code {
	FG_RED      = 31,
	FG_GREEN    = 32,
	FG_BLUE     = 34,
	FG_CYAN     = 36,
	FG_DEFAULT  = 39,
	BG_RED      = 41,
	BG_GREEN    = 42,
	BG_BLUE     = 44,
	BG_DEFAULT  = 49
};
class IOMod {
	Code code;
public:
	IOMod(Code pCode);
	friend std::ostream& operator<<(std::ostream& os, const IOMod& mod);
};
std::ostream& operator<<(std::ostream& os, const std::vector<IOMod>& mod);

class EndOfLineObj {
public:
	friend std::ostream& operator<<(std::ostream& os, const EndOfLineObj&);
};
const EndOfLineObj EOL{};


/*
	Exceptions
*/

class HeptException : public std::exception {
public:
	std::string errorMsg;
public:
	HeptException(std::string);
	const char* what() const throw();
	virtual std::vector<IOMod> bashFormat() const;
	const std::string& get() const;
};
std::ostream& operator<<(std::ostream& os, const HeptException& mod);

class CardDeviceError : public HeptException {
public:
	CardDeviceError(std::string);
};

class CompileError : public HeptException {
public:
	CompileError(std::string);
};


/*
	Commands & boards
*/

void assertCommandExists(std::string cmdName, std::string requirement="");
void assertPathExists(const std::string& path, bool assertFile=false);
void assertPathExists(const std::vector<std::string>&, bool assertFile=false);

std::vector<fs::path> listPossibleBoards();

class SysCommand {
private:
	std::string cmdName;
	std::stringstream cmd;
public:
	SysCommand(std::string);
	int exec();

	template<class T>
	friend SysCommand& operator<<(SysCommand& sysCmd, const T& part) {
		sysCmd.cmd << " " << part;
		return sysCmd;
	}
};

#endif // UTIL_HPP
