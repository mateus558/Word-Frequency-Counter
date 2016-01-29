all:
	g++ --std=c++11 -Wall -g main.cpp Set.cpp Set.h stack.h stack.cpp -o wlc
	./wlc
