CXX      = clang++
##CXXFLAGS = -fsanitize=leak -Wall -Wextra -Wpedantic -Wshadow
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -Wshadow

check_version: check_version.cpp
	$(CXX) $(CXXFLAGS) check_version.cpp -o $@

server_test: server.cpp key_value_store.o ./hashtable/hashtable.o
	$(CXX) $(CXXFLAGS) server.cpp key_value_store.o ./hashtable/hashtable.o -o server_test

key_value_store: key_value_store.cpp key_value_store.h
	$(CXX) $(CXXFLAGS) $^ -c

client_test: client_main.cpp client.o
	$(CXX) $(CXXFLAGS) client_main.cpp client.o -o client_test

client: client.h client.cpp
	$(CXX) $(CXXFLAGS) client.cpp client.h -c

hashtable_tests: ./hashtable/hashtable_tests.cpp ./hashtable/hashtable.o
	$(CXX) $(CXXFLAGS) ./hashtable/hashtable_tests.cpp ./hashtable/hashtable.o -o hashtable_tests

hashtable: ./hashtable/hashtable.h ./hashtable/hashtable.cpp
	$(CXX) $(CXXFLAGS) ./hashtable/hashtable.cpp ./hashtable/hashtable.h -c

clean:
	rm *.o hashtable_tests client_test