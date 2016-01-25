#include "RBtree.h"
#include <iostream>

using namespace std; 

RBtree::RBtree(){
	this->Nil = new NODE;
	this->root = Nil;
	N = 0;
	T = 0;
}

NODE* RBtree::tree_insert(string key){
	NODE *y = this->Nil;
	NODE *x = this->root;

	while(x != Nil){
		y = x;
		if(key < x->key){
			x = x->left;
		}else if(key > x->key){
			x = x->right;			
		}else{
			x->count++;
			T++;
			return NULL;
		}
	}

	N++;
	T++;

	NODE *z = new NODE(key);
	z->p = y;
	z->right = z->left = this->Nil;
	z->ehVermelho = true;

	if(y == Nil){
		this->root = z;
	}else if(key < y->key){
		y->left = z;
	}else{
		y->right = z;
	}
	return z;
}

void RBtree::leftRotate(NODE *x){
	NODE *y;
    y = x->right;
    x->right = y->left;
    y->left->p = x;
    y->p = x->p;
   
    if (x->p == Nil)
        root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
       	x->p->right = y;
    
	y->left = x;
    x->p = y;
}

void RBtree::rightRotate(NODE *x){
	NODE *y;
    y = x->left;
    x->left = y->right;
    y->right->p = x;
    y->p = x->p;
   
    if (x->p == Nil)
        root = y;
    else if (x == x->p->right)
        x->p->right = y;
    else
        x->p->left = y;
    y->right = x;
    x->p = y;
}

void RBtree::RB_insertFixUp(NODE *z){
	NODE *y;
   
    while (z->p->ehVermelho && (z->p != this->root)){
        if (z->p == z->p->p->left){	
            	y = z->p->p->right;           
		    if (y->ehVermelho){
				z->p->ehVermelho = true;
				y->ehVermelho = true;
				z->p->p->ehVermelho = true;
				z = z->p->p;
		    }else{
		        if (z == z->p->right){
		            	z = z->p;
		            	leftRotate(z);
		        }
		       	z->p->ehVermelho = true;
		        z->p->p->ehVermelho = true;
		        rightRotate( z->p->p );
		    }
       	}else{
            y = z->p->p->left;

            if (y->ehVermelho){
				z->p->ehVermelho = true;
				y->ehVermelho = true;
				z->p->p->ehVermelho = true;
				z = z->p->p;
            }else{
				if (z == z->p->left){
					z = z->p;
					rightRotate(z);
				}
				z->p->ehVermelho = true;
				z->p->p->ehVermelho = true;
				leftRotate(z->p->p);
            }
       	}
    }	
    root->ehVermelho = true;
 	y = NULL;
}

void RBtree::RB_insert(string key){
	NODE *z = tree_insert(key);
	if(z == NULL){ 
		return;
	}	
	RB_insertFixUp(z);
}

bool RBtree::search(string val){
	NODE *x = this->root;
	while(x != this->Nil){
		if(val > x->key){
			x = x->right;
		}else if(val < x->key){
			x = x->left;
		}else return true;
	}
	return false;
}

void RBtree::INORDER_TREE_WALK(NODE *root){
	if(root != this->Nil){
		INORDER_TREE_WALK(root->left);
		cout << root->key << "-" << root->count+1 << " ";
		INORDER_TREE_WALK(root->right);
	}
}	

void RBtree::display(){
	INORDER_TREE_WALK(this->root);
	cout << endl;
}

void RBtree::TREE_SORT(NODE *root, Container* array) const{
	if(root != this->Nil){
		TREE_SORT(root->right, array);
		array->push_back(root);		
		TREE_SORT(root->left, array);
	}
}

void RBtree::postOrderDelete(NODE *n){
	if (n != this->Nil){   
        	postOrderDelete(n->left);       
        	postOrderDelete(n->right);
		delete n;
	}
}

Container* RBtree::getSortedList(){
	Container* nodes = new Container(N);
	TREE_SORT(this->root, nodes);
	return nodes;
}

RBtree::~RBtree(){ 
	postOrderDelete(this->root);
	delete this->Nil;
}

