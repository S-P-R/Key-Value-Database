CXX      = clang++
CXXFLAGS = -fsanitize=leak -Wall -Wextra -Wpedantic -Wshadow


hashtable_tests: hashtable_tests.cpp hashtable.o
	$(CXX) $(CXXFLAGS) hashtable_tests.cpp hashtable.o -o hashtable_tests

hashtable: hashtable.h hashtable.cpp
	$(CXX) $(CXXFLAGS) hashtable.cpp hashtable.h -c

clean:
	rm *.o hashtable_tests