#ifndef STACK_H
#define STACK_H
#include <cassert>
#include <iostream>

template<class T> class stack{
	T* pilha;
	int N;
	int tam;
	int topo;
public:
	stack(int tam);
	stack();
	bool empty();
	void push(T data);
	void pop();	
	T top();
	int size(){ return N;}	
	//~stack();
};

#endif
