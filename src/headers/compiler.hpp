#if !defined(COMPILER_HPP)
#define COMPILER_HPP

#include "config.hpp"
#include "util.hpp"

void compileHeptagon(CompilerConfig& conf);
std::string compileC(CompilerConfig& conf);
void sendProgToBoard(CompilerConfig& conf, std::string hexFile);

#endif // COMPILER_HPP