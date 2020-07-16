CXX = clang++
CFLAGS = -Wall -Wextra -Werror -std=c++17 -O2

HDR = src/pid.hpp src/util.hpp src/fan.hpp src/config.hpp
OBJ = obj/pid.o obj/util.o obj/fan.o obj/config.o
LINK = -lpthread -lsmcpp
FRMWK = -framework IOKit
DEFS =

## Colours
COL_OBJ = $(shell tput setaf 3 2>/dev/null)
COL_EXE = $(shell tput setaf 4 2>/dev/null)
COL_SOO = $(shell tput setaf 5 2>/dev/null)
COL_RST = $(shell tput sgr0 2>/dev/null)
COL_BLD = $(shell tput bold 2>/dev/null)

## install dir
PREFIX = /usr/local

.SECONDARY:

obj/%.o: src/%.cpp $(HDR)
	@printf "$(COL_OBJ)ASSEMBLING OBJECT $@$(COL_RST)\n"
	@mkdir -p `dirname $@`
	@$(CXX) $(CFLAGS) $(DEFS) $< -c -o $@

fanpid: $(OBJ) obj/main.o $(HDR)
	@printf "$(COL_OBJ)LINKING OBJECTS TO EXECUTABLE $@$(COL_RST)\n"
	@$(CXX) $(CFLAGS) $(FRMWK) $(DEFS) $(CLIENT) $(OBJ) obj/main.o -o $@ $(LINK)

tests: $(OBJ) obj/tests.o $(HDR)
	@printf "$(COL_OBJ)LINKING OBJECTS TO EXECUTABLE $@$(COL_RST)\n"
	@$(CXX) $(CFLAGS) $(FRMWK) $(DEFS) $(CLIENT) $(OBJ) obj/tests.o -o $@ $(LINK)

options:
	@printf "Compiler: $(COL_BLD)$(CXX)$(COL_RST)\n"

clean:
	rm -rf obj smctool *.so
