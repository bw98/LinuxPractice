CXXFLAGS = -Wall -g -std=c++11
test : BTree.o main.o
	g++ $(CXXFLAGS) main.o BTree.o -o test
main.o : main.cpp BTree.h
	g++ $(CXXFLAGS) -c main.cpp 
BTree.o : BTree.cpp BTree.h
	g++ $(CXXFLAGS) -c BTree.cpp
clean :
	rm BTree.o main.o
