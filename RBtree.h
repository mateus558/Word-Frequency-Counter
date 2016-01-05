#ifndef RBTREE_H_
#define RBTREE_H_
#include <iostream>
#include <assert.h>

using namespace std;
struct NODE{
	string key;
	NODE *right;
	NODE *left;
	NODE *p;
	int count;
	int pos;
	bool ehVermelho;
	
	NODE(string key){
		this->key = key;
		ehVermelho = false;
		count = 1;
	}
	NODE(){
		this->key = "";
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
};

class RBtree{
private:
	NODE *root;
	NODE *Nil;
	int N;
	long long int T;
	void INORDER_TREE_WALK(NODE *root);
	void RB_insertFixUp(NODE *z);
	NODE* tree_insert(string key);	
	void leftRotate(NODE *x);
	void rightRotate(NODE *x);
	void TREE_SORT(NODE *root, Container *array) const;

public:
	RBtree();
	void RB_insert(string key);
	bool search(string val);
	int getN(){return N;}
	int getTotal(){return T;}
	void display();
	void postOrderDelete(NODE *n);
	Container* getSortedList();
	~RBtree();
};

#endif
