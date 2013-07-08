
bins = example1 example2 example3 example4

#################################################

FLAGS = -g3 -ggdb -Wall
INCLUDES = -I..
LIBS = -lstdc++

CXXFLAGS= $(FLAGS) $(INCLUDES) 

sources = $(bins:=.cpp)

all: $(bins)

$(bins): 
	$(CXX) $(FLAGS) $(INCLUDES) $(LIBS)  $< -o $@ 

clean: 
	rm -rf *~ *.d *.o $(bins)

%.d: %.cpp
	set -e; $(CXX) $(INCLUDES) -MM $(CPPFLAGS) $< \
		| sed 's/\($*\)\.o[ :]*/\1 $@ : /g' > $@; \
	        [ -s $@ ] || rm -f $@

include $(sources:.cpp=.d)
