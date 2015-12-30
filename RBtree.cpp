#include "RBtree.h"
#include <iostream>

using namespace std; 

template <class T> bool operator <(const T& key, const T& key1){return key < key1;}

template <class T> RBtree<T>::RBtree(T def_val){
	this->nill = new NODE<T>;
	nill->right = nill->left = nill->p = nill;
	nill->ehVermelho = false;
	nill->key = def_val;
	this->root = new NODE<T>;
	root->right = root->left = root->p = nill;
	root->ehVermelho = false;
	root->key = def_val;
	N = 0;
}

template <class T> NODE<T>* RBtree<T>::tree_insert(T key){
	NODE<T> *y = this->nill;
	NODE<T> *x = this->root;
	N++;
	while(*x != *nill){
		y = x;
		if(key < x->key){
			x = x->left;
		}else if(key == x->key){
			x->count++;
			return NULL;
		}else{
			x = x->right;			
		}
	}
	NODE<T> *z = new NODE<T>;
	z->key = key;
	z->p = y;
	z->right = this->nill;
	z->left = this->nill;
	if(*y == *nill){
		this->root = z;
	}else if(key < y->key){
		y->left = z;
	}else{
		y->right = z;
	}
	return z;
}

template <class T> void RBtree<T>::leftRotate(NODE<T> *&x){
	NODE<T> *y = x->right;
	x->right = y->left;
	if(*y->left != *this->nill){
		y->left->p = y;
	}
	y->p = x->p;
	if(*y->p == *this->nill){
		this->root = x;
	}else if(*y == *y->p->right){
		y->p->right = x;
	}else y->p->left = x;
	x->right = y;
	y->p = x;
}

template <class T> void RBtree<T>::rightRotate(NODE<T> *&y){
	NODE<T> *x = y->left;
	y->left = x->right;
	if(*x->right != *nill){
		x->right->p = x;
	}
	x->p = y->p;
	if(*x->p == *nill){
		this->root = y;
	}else if(*x == *x->p->left){
		x->p->left = y;
	}else x->p->right = y;
	y->left = x;
	x->p = y;
}

template <class T> void RBtree<T>::RB_insertFixUp(NODE<T> *&z){
	while(z->p->ehVermelho && (*z != *this->root)){
		if(*z->p == *z->p->p->left){
			//Pai de z eh no leftuerdo, entao o tio eh righteito			
			NODE<T> *tio = z->p->p->right;
			if(tio->ehVermelho){
			//Caso 1 - Tio eh vermelho, entao troca cores				
				z->p->ehVermelho = false;
				tio->ehVermelho = false;
				z->p->p->ehVermelho = true;
				z = z->p->p;
			}else if(*z == *z->p->right){
				//Caso 2 - Tio eh preto, entao faz rotacao 
				z = z->p;
				leftRotate(z);
			}
			//Caso 3
			z->p->ehVermelho = false;
			z->p->p->ehVermelho = true;
			rightRotate(z->p->p);
		}else{
			//Simetrico ao if, trocando left por right
			NODE<T> *tio = z->p->p->left;
			if(tio->ehVermelho){
				z->p->ehVermelho = false;
				tio->ehVermelho = false;
				z->p->p->ehVermelho = true;
				z = z->p->p;
			}else if(*z == *z->p->left){
				z = z->p;
				rightRotate(z);
			}
			z->p->ehVermelho = false;
			z->p->p->ehVermelho = true;
			leftRotate(z->p->p);
		}
	}
	this->root->ehVermelho = false;
}

template <class T> void RBtree<T>::RB_insert(T key){
	NODE<T> *z = tree_insert(key);
	if(z == NULL){ 
		return;
	}	
	//cout << z->key << " " << z->p->key << endl;
	z->ehVermelho = true;	
	//RB_insertFixUp(z);
}

template <class T> bool RBtree<T>::search(T val){
	NODE<T> *x = this->root;
	while(*x != *this->nill){
		if(val < x->key){
			x = x->right;
		}else if(val > x->key){
			x = x->left;
		}else return true;
	}
	return false;
}

template <class T> void RBtree<T>::INORDER_TREE_WALK(NODE<T> *root){
	if(*root != *this->nill){
		INORDER_TREE_WALK(root->left);
		cout << root->key << "-" << root->count+1 << " ";
		INORDER_TREE_WALK(root->right);
	}
}	

template <class T> void RBtree<T>::display(){
	INORDER_TREE_WALK(this->root);
	cout << endl;
}

template <class T> void RBtree<T>::TREE_SORT(NODE<T> *root, Container<T>* array, int i){
	if(*root != *this->nill){
		TREE_SORT(root->left, array, i);
		array->push_back(root);		
		TREE_SORT(root->right, array, i);
	}
}

template <class T> Container<T>* RBtree<T>::getSortedList(){
		Container<T>* nodes = new Container<T>(N);
		TREE_SORT(this->root, nodes, 0);
		return nodes;
}

template <class T> void RBtree<T>::aux_delete(NODE<T> *&root){
	if(root != NULL){		
		aux_delete(root->right);
		aux_delete(root->left);
		delete root;
	}
}

template <class T> RBtree<T>::~RBtree(){ 
	aux_delete(this->root);	
}

template class RBtree<int>;
template class RBtree<string>;
template class RBtree<float>;
template class RBtree<double>;
