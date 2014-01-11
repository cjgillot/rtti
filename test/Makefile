CXX=g++
BUILDDIR=../build/mmethod/
CXXFLAGS+=-std=c++11 -I../include -DNDEBUG -fno-exceptions -fno-rtti -O3 -march=corei7 -mtune=corei7 -pedantic -fomit-frame-pointer -save-temps
MMOFLAGS+=-std=c++11 -I../include -DRTTI_MMETHOD_DO_PRINT -Wall -Wextra -O0 -fno-inline
LDFLAGS+=-L../build/src/ -lrtti -fwhole-program 
EXE=./exe

# CXXFLAGS=-std=c++11 -I../include -O0 -g

SRC=$(wildcard *.cpp)
MMO=$(SRC:.cpp=.mmo)
OBJ=$(SRC:.cpp=.o) mmethod.o

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

%.mmo: %.cpp
# 	$(CXX) -S $< -o /dev/null $(MMOFLAGS) 2>&1 | perl ../mmethod/parse/parse.pl -o $@
	$(CXX) -S $< -o /dev/null $(MMOFLAGS) 2>&1 | $(BUILDDIR)/parse/parse -o $@
# 	/bin/false

mmethod.cc: $(MMO)
	$(BUILDDIR)/mmethod/mmethod -o $@ $^

$(EXE): $(OBJ)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

.PRECIOUS: Makefile
.PHONY: clean
