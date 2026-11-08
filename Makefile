CXX = g++
CXXFLAGS = -std=c++17 -O3

all: triangles generator

triangles: triangles.cpp
	$(CXX) $(CXXFLAGS) -pthread triangles.cpp -o triangles

generator: generator.cpp
	$(CXX) $(CXXFLAGS) generator.cpp -o generator

clean:
	rm -f triangles generator

.PHONY: all clean
