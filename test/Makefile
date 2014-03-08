CXX=g++
BUILDDIR=../build/mmethod/
CXXFLAGS+=-g -std=c++11 -I../include -DNDEBUG -fno-exceptions -fno-rtti -O3 -march=corei7 -mtune=corei7 -pedantic -fomit-frame-pointer
MMOFLAGS+= -I../include -DRTTI_MMETHOD_DO_PRINT -Wall -Wextra -O0 -fno-inline
LDFLAGS+=-L../build/src/ -lrtti -fwhole-program 
EXE=./exe

# CXXFLAGS=-std=c++11 -I../include -O0 -g -Wall -Wextra

SRC=$(wildcard *.cpp)
MMO=$(SRC:.cpp=.mmo)
ASM=$(SRC:.cpp=.s)
OBJ=$(SRC:.cpp=.o)

all: $(EXE)

test: $(EXE)
	$(EXE)

bench: $(EXE)
	time $(EXE) 3000000000

clean:
	find -name '*~' -delete
	find -name '*.o' -delete
	find -name '*.mmo' -delete
	rm -f mmethod.cc $(EXE)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.o: %.cc
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.s: %.cpp
	$(CXX) -S $< -o $@ $(CXXFLAGS) -g0

asm: $(ASM)

$(EXE): $(OBJ)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

.PRECIOUS: Makefile
.PHONY: clean
