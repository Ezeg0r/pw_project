CXX = g++
CXXFLAGS = -O3 -std=c++17

all: slow fast

slow: slow.cpp
	$(CXX) $(CXXFLAGS) slow.cpp -o slow

fast: fast.cpp
	$(CXX) $(CXXFLAGS) -pthread fast.cpp -o fast

tests/gen: tests/gen.cpp
	$(CXX) $(CXXFLAGS) tests/gen.cpp -o tests/gen

benchmark: all tests/gen
	cd tests && ./gen
	python3 benchmark.py

clean:
	rm -f slow fast tests/gen

.PHONY: all benchmark clean