CXX=g++
CXXFLAGS=-std=c++11
SOURCES=gridmaket.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=gridmaket

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
clean:
	rm -rf *.o $(EXECUTABLE)
