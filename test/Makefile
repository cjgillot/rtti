BUILDDIR=../build/mmethod/
CXXFLAGS+=-std=c++11 -I../include -DNDEBUG -fno-omit-frame-pointer -fno-rtti -g -O2 -Ofast -march=corei7 -mtune=corei7
MMOFLAGS+=-std=c++11 -I../include -DRTTI_MMETHOD_DO_PRINT -Wall -Wextra -O0 -fno-inline
LDFLAGS+=-L../build/src/ -lrtti
EXE=./exe

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
	rm -f mmethod.cc $(MMO) $(OBJ) $(EXE)

%.o: %.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

%.o: %.cc
	g++ -c $< -o $@ $(CXXFLAGS)

%.mmo: %.cpp
	g++ -S $< -o /dev/null $(MMOFLAGS) 2>&1 | $(BUILDDIR)/parse/parse -o $@

mmethod.cc: $(MMO)
	$(BUILDDIR)/mmethod/mmethod -o $@ $^

$(EXE): $(OBJ)
	g++ $^ $(CXXFLAGS) $(LDFLAGS) -o $@

.PRECIOUS: Makefile
.PHONY: clean
