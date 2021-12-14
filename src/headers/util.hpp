#if !defined(UTIL_HPP)
#define UTIL_HPP

#include <iostream>
#include <exception>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

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


/*
	Commands & boards
*/

void assertCommandExists(std::string cmdName, std::string requirement="");

std::vector<fs::path> listPossibleBoards();

#endif // UTIL_HPP
