CXX = g++
CXXFLAGS = -std=c++11 -O3
INCLUDES = -Iinclude

OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp src/*.cpp))

.PHONY: all
all: main

main: $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o main $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

.PHONY: clean
clean:
	-rm main
	-rm *.o
	-rm src/*.o