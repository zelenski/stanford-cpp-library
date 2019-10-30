
PROGRAMS = delayed_output echo_input exitcode42 infinite_loop infinite_output

all:: $(PROGRAMS)

CXX = g++
CXXFLAGS = -g3 -O0 -Wall
LDFLAGS =
LDLIBS =

$(PROGRAMS): %:%.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean::
	rm -rf $(PROGRAMS) *.o *.dSYM

.PHONY: clean all