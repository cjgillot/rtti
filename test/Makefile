CXX=g++
BUILDDIR=../build/mmethod/
CXXFLAGS+=-g -std=c++11 -I../include -DNDEBUG -fno-exceptions -fno-rtti -O3 -march=corei7 -mtune=corei7 -pedantic -fomit-frame-pointer -O3 -fexpensive-optimizations
LDFLAGS+=-L../build/src/ -lrtti -fwhole-program 
EXE=./exe

# CXXFLAGS=-std=c++11 -I../include -O0 -g -Wall -Wextra

SRC=$(wildcard *.cpp)
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
	rm -f $(EXE)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.s: %.cpp
	$(CXX) -S $< -o $@ $(CXXFLAGS) -g0

asm: $(ASM)

$(EXE): $(OBJ)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

.PRECIOUS: Makefile
.PHONY: clean
