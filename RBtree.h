#ifndef RBTREE_H_
#define RBTREE_H_

template <typename T> struct NODE{
	T key;
	NODE *right;
	NODE *left;
	NODE *p;
	int count;
	bool ehVermelho;

	/*      ----------
		Operadores 
		----------     		*/
	//Retorna true se os nodes sao iguais	
	bool operator ==(const NODE& node){		
		return this->key == node.key;
	}
	//Retorna true se os nodes sao diferentes
	bool operator !=(const NODE& node){		
		return this->key != node.key;
	}
};

template <class T> class RBtree{
private:
	NODE<T> *root;
	NODE<T> *nill;
	int N;
	void INORDER_TREE_WALK(NODE<T> *root);
	void RB_insertFixUp(NODE<T> *&z);
	NODE<T>* tree_insert(T key);	
	void leftRotate(NODE<T> *&x);
	void rightRotate(NODE<T> *&y);
	void aux_delete(NODE<T> *&root);

public:
	RBtree(T def_val);
	void RB_insert(T key);
	bool search(T val);
	int getN(){return N;}
	void display();
	~RBtree();
};

#endif
