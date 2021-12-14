AVR-CC = avr-gcc -std=gnu99
AVR-CXX = avr-gcc -std=c++17
AVR-CCFLAGS = -Os -DF_CPU=16000000UL -mmcu=atmega328p -ffunction-sections -fdata-sections
LIB-SRC = $(wildcard lib/*.c) $(wildcard lib/*.cpp)
LIB-OBJ-FILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(LIB-SRC)))
LIB_LOCALPATH = lib
LIB_NAME = heptaboard
LIB_FILE = ${LIB_LOCALPATH}/lib${LIB_NAME}.a

LIB_PATH = /usr/local/lib
H_PATH = /usr/local/include/${LIB_NAME}
BIN_PATH = /usr/local/bin

CXX = clang++
CXX-G-FLAGS = -g -fsanitize=address -ggdb3
CXX-FLAGS = -std=c++20 -O2 -Wall -Wextra -Wno-sign-compare -Wshadow $(CXX-G-FLAGS)
INCLUDES = -Isrc/headers
HB-SRC = $(wildcard src/*.c) $(wildcard src/*.cpp)
OBJ-FILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(HB-SRC)))

# This is a minimal set of ANSI/VT100 color codes
_END=\x1b[0m
_RED=\x1b[31m
_GREEN=\x1b[32m
_BLUE=\x1b[34m
_PURPLE=\x1b[35m
_CYAN=\x1b[36m

all: lib heptaboard
	@echo "${_CYAN}You sould install heptaboard with 'make install'${_END}"

install:
	cp $(LIB_FILE) $(LIB_PATH)
	@echo "${_GREEN}Library installed in ${LIB_PATH}${_END}"
	mkdir -p ${H_PATH}
	cp ${LIB_LOCALPATH}/*.h ${H_PATH}
	@echo "${_GREEN}Headers installed in ${H_PATH}${_END}"
	cp heptaboard ${BIN_PATH}
	@echo "${_GREEN}Heptaboard installed in ${BIN_PATH}${_END}"

uninstall:
	rm $(LIB_PATH)/lib${LIB_NAME}.a
	rm -R ${H_PATH}

lib: $(LIB-OBJ-FILES)
	avr-ar -crs $(LIB_FILE) $(LIB-OBJ-FILES)

lib/%.o: lib/%.cpp
	$(AVR-CXX) $(AVR-CCFLAGS) -o $@ -c $<

lib/%.o: lib/%.c
	$(AVR-CC) $(AVR-CCFLAGS) -o $@ -c $<

heptaboard: $(OBJ-FILES)
	$(CXX) $(CXX-FLAGS) -o heptaboard $(OBJ-FILES)

src/%.o: src/%.cpp
	$(CXX) $(CXX-FLAGS) $(INCLUDES) -o $@ -c $<

clean: cleanLib cleanSrc

cleanLib:
	rm $(LIB-OBJ-FILES) $(LIB_FILE)

cleanSrc:
	rm $(OBJ-FILES) heptaboard