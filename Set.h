#ifndef Set_H_
#define Set_H_
#include <assert.h>
#include <fstream>
#include <cstring>
#include <vector>
#include <iostream>

using namespace std;

struct NODE{
	string key;
	vector<int> localFreq;
	NODE *right;
	NODE *left;
	NODE *p;
	int globalFreq;
	int pos;
	bool ehVermelho;
	NODE(string key, int nFiles){
		//files = new Hash(nFiles);
		localFreq = vector<int>(nFiles);
		this->key = key;
		ehVermelho = false;
		globalFreq = 1;
	}
	void addFile(string file){
		//this->files->insert(file);
	}
	NODE(int nFiles){
		//files = new Hash(nFiles);
		localFreq = vector<int>(nFiles);
		ehVermelho = false;
		globalFreq = 0;
	}

};

struct Container{
	NODE **array;
	int count;	
	Container(int K){
		count = 0;
		array = new NODE*[K];
		for(int i = 0; i < K; i++){
			array[i] = NULL;
		}
	}
	void push_back(NODE* node){
		array[count] = node;
		count++;
	}
	~Container(){
		for(int i = 0; i < count; i++){
			delete array[i];
		}
	}	
};

class Set{
private:
	NODE *root;
	NODE *Nil;
	vector<string> usedFiles;
	Container* sortedNodes;
	vector<int> fileNWords;
	int nFiles;
	int countF;
	int N;
	long long int T;
	void INORDER_TREE_WALK(NODE *root);
	void RB_insertFixUp(NODE *z);
	NODE* tree_insert(string key, int i);	
	void leftRotate(NODE *x);
	void rightRotate(NODE *x);
	void TREE_SORT(NODE *root, Container *array) const;
	void merge(NODE **arr, int l, int m, int r);
	void merge(NODE **arr, int l, int m, int r, int file, bool sortType);
	void mergeSort(NODE **arr, int l, int r);
	void mergeSort(NODE **arr, int l, int r, int file, bool sortType);
public:
	Set(int nFiles);
	void insertUsedFile(string fileName, int i);
	void RB_insert(string key, int i);
	bool search(string val);
	int getN(){return N;}
	int getTotal(){return T;}
	void display();
	void displayUsedFiles();
	void displayXmoreFrequent(int X);
	void displayXmoreFrequentByFile(string fileName, int X);
	int getNFiles(){return nFiles;}
	Container* result();
	void postOrderDelete(NODE *n);
	int getFileNWords(string file);
	Container* getSortedList();
	~Set();
};

#endif
