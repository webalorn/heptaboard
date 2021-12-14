CC = avr-gcc -std=gnu99
CXX = avr-gcc -std=c++17
CXXFLAGS = -c -Os -DF_CPU=16000000UL -mmcu=atmega328p -ffunction-sections -fdata-sections
SRC = $(wildcard lib/*.c) $(wildcard lib/*.cpp)
OBJ_FILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRC)))
LIB_LOCALPATH = lib
LIB_NAME = heptaboard
LIB_FILE = ${LIB_LOCALPATH}/lib${LIB_NAME}.a

LIB_PATH = /usr/local/lib
H_PATH = /usr/local/include/${LIB_NAME}

# This is a minimal set of ANSI/VT100 color codes
_END=\x1b[0m
_RED=\x1b[31m
_GREEN=\x1b[32m
_BLUE=\x1b[34m
_PURPLE=\x1b[35m
_CYAN=\x1b[36m

all: lib
	@echo "${_CYAN}You can use the library from the lib/ directory, or install it with make install${_END}"
	@echo ${LIB_FILE}

# install:
# 	cp $(LIB_FILE) $(LIB_PATH)
# 	@echo "${_GREEN}Library installed in ${LIB_PATH}${_END}"
# 	mkdir -p ${H_PATH}
# 	cp ${LIB_LOCALPATH}/*.h ${H_PATH}
# 	@echo "${_GREEN}Headers installed in ${LIB_PATH}${_END}"

# uninstall:
# 	rm $(LIB_PATH)/lib${LIB_NAME}.a
# 	rm -R ${H_PATH}

lib: $(OBJ_FILES)
	avr-ar -crs $(LIB_FILE) $(OBJ_FILES)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

%.o: %.c
	$(CC) $(CXXFLAGS) -o $@ -c $<

clean:
	rm $(OBJ_FILES) $(LIB_FILE)