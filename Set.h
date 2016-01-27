#ifndef Set_H_
#define Set_H_
#include <iostream>
#include <assert.h>
#include <string>
#include <fstream>
#include "Hash.h" 

using namespace std;

struct NODE{
	string key;
	Hash *files;
	NODE *right;
	NODE *left;
	NODE *p;
	int count;
	int pos;
	bool ehVermelho;
	NODE(string key, int nFiles){
		files = new Hash(nFiles);
		this->key = key;
		ehVermelho = false;
		count = 1;
	}
	void addFile(string file){
		this->files->insert(file);
	}
	NODE(int nFiles){
		files = new Hash(nFiles);
		ehVermelho = false;
		count = 0;
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
	Hash *usedFiles;
	Container* sortedNodes;
	int nFiles;
	int N;
	long long int T;
	void INORDER_TREE_WALK(NODE *root);
	void RB_insertFixUp(NODE *z);
	NODE* tree_insert(string key, string file);	
	void leftRotate(NODE *x);
	void rightRotate(NODE *x);
	void TREE_SORT(NODE *root, Container *array) const;
	void merge(NODE **arr, int l, int m, int r);
	void mergeSort(NODE **arr, int l, int r);
public:
	Set(int nFiles);
	void insertUsedFile(string fileName);
	void RB_insert(string key, string file);
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
	Container* getSortedList();
	~Set();
};

#endif
