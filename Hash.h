#ifndef HASH_H
#define HASH_H
#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class Data{
	string key;
	int freq;
	Data(string key){
		this->key = key;
		freq = 1;
	}
friend class Hash;
friend class ListH;
};

class ListH{
	Data* data;
	ListH* prox;

	~ListH(){
		delete data;
		delete prox;
	}
friend class Hash;
};

class Hash{
	ListH **table;
	int m;
	int N;
public:
	Hash(int m);
	int hashFunc(string data);
	void insert(string data);
	int getFreq(string data);
	int getFreqT(){return N;}
	bool find(string data);
	void display();
	~Hash();
};

#endif
