#ifndef RBTREE_H
#define RBTREE_H

template <class T> struct NODE{
	T key;
	NODE<T> *dir;
	NODE<T> *esq;
	NODE<T> *pai;
	int count;
	bool ehVermelho;
	bool operator ==(const NODE& node){		
		if(this->key == node.key && this->ehVermelho == node.ehVermelho)
			return true;
		return false;
	}
	bool operator !=(const NODE& node){		
		if(this->key == node.key && this->ehVermelho == node.ehVermelho)
			return false;
		return true;
	}
	
	NODE(){
		count = 0;
		ehVermelho = false;
	}	
};

template <class T> class RBtree{
private:
	NODE<T> *root;
	NODE<T> *nill;
	int n;
	void INORDER_TREE_WALK(NODE<T> *root);
	void RB_insertFixUp(NODE<T> *&z);
	NODE<T>* tree_insert(T key);	
	void leftRotate(NODE<T> *&x);
	void rightRotate(NODE<T> *&y);
	void aux_delete(NODE<T> *&root);

public:
	RBtree();
	void RB_insert(T key);
	bool search(T val);
	void display();
	int getN(){return this->n;}
	~RBtree();
};

#endif
