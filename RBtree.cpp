#include "RBtree.h"
#include <iostream>

using namespace std; 

template <class T> bool operator <(const T& key, const T& key1){return key < key1;}

template <class T> RBtree<T>::RBtree(T def_val){
	this->nill = new NODE<T>;
	nill->dir = nill->esq = nill->pai = nill;
	nill->ehVermelho = false;
	nill->key = def_val;
	this->root = new NODE<T>;
	root->dir = root->esq = root->pai = nill;
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
			x = x->esq;
		}else if(key == x->key){
			x->count++;
			return NULL;
		}else{
			x = x->dir;			
		}
	}
	NODE<T> *z = new NODE<T>;
	z->key = key;
	z->pai = y;
	z->dir = this->nill;
	z->esq = this->nill;
	if(*y == *nill){
		this->root = z;
	}else if(key < y->key){
		y->esq = z;
	}else y->dir = z;
	return z;
}

template <class T> void RBtree<T>::leftRotate(NODE<T> *&x){
	NODE<T> *y = x->dir;
	x->dir = y->esq;
	if(*y->esq != *this->nill){
		y->esq->pai = y;
	}
	y->pai = x->pai;
	if(*y->pai == *this->nill){
		this->root = x;
	}else if(*y == *y->pai->dir){
		y->pai->dir = x;
	}else y->pai->esq = x;
	x->dir = y;
	y->pai = x;
}

template <class T> void RBtree<T>::rightRotate(NODE<T> *&y){
	NODE<T> *x = y->esq;
	y->esq = x->dir;
	if(*x->dir != *nill){
		x->dir->pai = x;
	}
	x->pai = y->pai;
	if(*x->pai == *nill){
		this->root = y;
	}else if(*x == *x->pai->esq){
		x->pai->esq = y;
	}else x->pai->dir = y;
	y->esq = x;
	x->pai = y;
}

template <class T> void RBtree<T>::RB_insertFixUp(NODE<T> *&z){
	while(z->pai->ehVermelho && (*z != *this->root)){
		if(*z->pai == *z->pai->pai->esq){
			//Pai de z eh no esquerdo, entao o tio eh direito			
			NODE<T> *tio = z->pai->pai->dir;
			if(tio->ehVermelho){
			//Caso 1 - Tio eh vermelho, entao troca cores				
				z->pai->ehVermelho = false;
				tio->ehVermelho = false;
				z->pai->pai->ehVermelho = true;
				z = z->pai->pai;
			}else if(*z == *z->pai->dir){
				//Caso 2 - Tio eh preto, entao faz rotacao 
				z = z->pai;
				leftRotate(z);
			}
			//Caso 3
			z->pai->ehVermelho = false;
			z->pai->pai->ehVermelho = true;
			rightRotate(z->pai->pai);
		}else{
			//Simetrico ao if, trocando esq por dir
			NODE<T> *tio = z->pai->pai->esq;
			if(tio->ehVermelho){
				z->pai->ehVermelho = false;
				tio->ehVermelho = false;
				z->pai->pai->ehVermelho = true;
				z = z->pai->pai;
			}else if(*z == *z->pai->esq){
				z = z->pai;
				rightRotate(z);
			}
			z->pai->ehVermelho = false;
			z->pai->pai->ehVermelho = true;
			leftRotate(z->pai->pai);
		}
	}
	this->root->ehVermelho = false;
}

template <class T> void RBtree<T>::RB_insert(T key){
	NODE<T> *z = tree_insert(key);
	if(z == NULL){ 
		return;
	}	
	//cout << z->key << " " << z->pai->key << endl;
	z->ehVermelho = true;	
	//RB_insertFixUp(z);
}

template <class T> bool RBtree<T>::search(T val){
	NODE<T> *x = this->root;
	while(*x != *this->nill){
		if(val < x->key){
			x = x->dir;
		}else if(val > x->key){
			x = x->esq;
		}else return true;
	}
	return false;
}

template <class T> void RBtree<T>::INORDER_TREE_WALK(NODE<T> *root){
	if(*root != *this->nill){
		INORDER_TREE_WALK(root->esq);
		cout << root->key << "-" << root->count+1 << " ";
		INORDER_TREE_WALK(root->dir);
	}
}	

template <class T> void RBtree<T>::display(){
	INORDER_TREE_WALK(this->root);
	cout << endl;
}

template <class T> void RBtree<T>::aux_delete(NODE<T> *&root){
	if(root != NULL){		
		aux_delete(root->dir);
		aux_delete(root->esq);
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
