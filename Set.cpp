#include "Set.h"

using namespace std; 

Set::Set(int nFiles){
	this->Nil = new NODE(nFiles);
	this->nFiles = nFiles;
	this->usedFiles = new Hash(nFiles);
	this->root = Nil;
	N = 0;
	T = 0;
}

NODE* Set::tree_insert(string key, string file){	//Inserção de arvore binária
	NODE *y = this->Nil;
	NODE *x = this->root;

	while(x != Nil){
		y = x;
		if(key < x->key){
			x = x->left;
		}else if(key > x->key){
			x = x->right;			
		}else{
			x->addFile(file);
			x->count++;
			T++;
			return NULL;
		}
	}
	
	N++;
	T++;
	
	NODE *z = new NODE(nFiles);
	z->addFile(file);
	z->key = key;
	z->p = y;
	z->count++;
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

void Set::insertUsedFile(string fileName){
	usedFiles->insert(fileName);
}

void Set::RB_insert(string key, string file){
	NODE *z = tree_insert(key, file);
	if(z == NULL){ 
		return;
	}	
	RB_insertFixUp(z);
}

void Set::leftRotate(NODE *x){	//Rotação esquerda
	NODE *y;
    y = x->right;
    x->right = y->left;
	if(y->left != Nil)    	
		y->left->p = x;
    y->p = x->p;
   
    if (x->p == Nil)
        root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else x->p->right = y;
    
	y->left = x;
    x->p = y;
}

void Set::rightRotate(NODE *x){	//Rotação direita
	NODE *y;
    y = x->left;
    x->left = y->right;
	if(y->right != Nil)	
	    y->right->p = x;
    y->p = x->p;
   
    if (x->p == Nil)
        root = y;
    else if (x == x->p->right)
        x->p->right = y;
    else x->p->left = y;
    
	y->right = x;
    x->p = y;
}

void Set::RB_insertFixUp(NODE *z){
	NODE *y;
   
    while (z->p->ehVermelho && (z->p != this->root)){	//Executa enquanto o pai for vermelho
        if (z->p == z->p->p->left){	//Tio é esquerdo		
            	y = z->p->p->right;           
		    if (y->ehVermelho){	//Caso 1: Tio é vermelho, então troca as cores do pai, avô e do tio
				z->p->ehVermelho = true;
				y->ehVermelho = true;
				z->p->p->ehVermelho = true;
				z = z->p->p;
		    }else{	
		        if (z == z->p->right){	//Caso 2: Tio preto e nó z direito, então faz rotação direita no pai de z	
		            	z = z->p;
		            	leftRotate(z);
		        }
		       	z->p->ehVermelho = true;	//Caso 3: Tio preto e nó z esquerdo, então faz rotação esquerda no pai e direita no avo de z
		        z->p->p->ehVermelho = true;
		        rightRotate(z->p->p);
		    }
       	}else{	//Análogo
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

bool Set::search(string val){
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

void Set::INORDER_TREE_WALK(NODE *root){	//Percorre a arvore in ordem
	if(root != this->Nil){
		INORDER_TREE_WALK(root->left);
		cout << root->key << "-" << root->count+1 << " ";
		INORDER_TREE_WALK(root->right);
	}
}	

void Set::display(){	//imprime elementos da arvore ordenados
	INORDER_TREE_WALK(this->root);
	cout << endl;
}

void Set::TREE_SORT(NODE *root, Container* array) const{	//Coloca os nós da arvore ordenas em um Container
	if(root != this->Nil){
		TREE_SORT(root->right, array);
		array->push_back(root);		
		TREE_SORT(root->left, array);
	}
}

void Set::postOrderDelete(NODE *n){	//Deleta os nós em pos ordem
	if (n != this->Nil){   
        postOrderDelete(n->left);       
        postOrderDelete(n->right);
		delete n->files;		
		delete n;
	}
}

Container* Set::getSortedList(){	//Retorna container ordenado
	Container* nodes = new Container(N);
	TREE_SORT(this->root, nodes);
	return nodes;
}

Container* Set::result(){
	//Retorna uma lista com os nós ordenados lexicograficamente
	sortedNodes = this->getSortedList();
	//Ordena os nos por frequencia mantendo a ordem em que aparecem					
	mergeSort(sortedNodes->array, 0, sortedNodes->count-1);
	return sortedNodes;
}

void Set::merge(NODE **arr, int l, int m, int r){
	int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* Cria arrays temporários */
    NODE *L[n1], *R[n2];
 
    /* Copia dados para arrays temporários L e R */
    for(i = 0; i < n1; i++) L[i] = arr[l + i];
    for(j = 0; j < n2; j++) R[j] = arr[m + 1+ j];
 
    /* Intercala os arrays temporários em arr[l..r]*/
    i = 0; j = 0; k = l;
    while (i < n1 && j < n2){
        if (L[i]->files->getFreqT() > R[j]->files->getFreqT()){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copia os elementos restantes em L[], se existe algum */
    while (i < n1){
        arr[k] = L[i];
        i++; k++;
    }
 
    /* Copia os elementos restantes em R[], se existe algum */
    while (j < n2){
        arr[k] = R[j];
        j++; k++;
    }
}

void Set::mergeSort(NODE **arr, int l, int r){
	if (l < r){
        int m = l+(r-l)/2; //O mesmo que (l+r)/2, mas evita overflow para valores grandes de l e h
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

void Set::displayUsedFiles(){
	usedFiles->display();
}

void Set::displayXmoreFrequent(int X){
	ofstream output("Output/opt2.txt", ios::out);	//Abre o arquivo de saída
	if(!output){
		cerr << "Error opening the file" << endl;
		return;
	}	
	if(X <= N){
		for(int i = 0; i < X; i++){	//Imprime os X nós mais frequentes	
				cout << sortedNodes->array[i]->key << " - " << sortedNodes->array[i]->count << " occurrences" << endl; 
				output << sortedNodes->array[i]->key << " " << sortedNodes->array[i]->count << endl;	
		}
	}else cerr << "Size exceeded!" << endl;
	output.close();
	output.clear();
}

void Set::displayXmoreFrequentByFile(string fileName, int X){
		ofstream output("Output/opt3.txt", ios::out);		//Abre arquivo de saída
		output << fileName + "\n" << endl;
		if(X <= N){			
			for(int i = 0; i < X; i++){
				int freq = sortedNodes->array[i]->files->getFreq(fileName);				
				if(freq > 0){				
					cout << sortedNodes->array[i]->key << " - " << freq << " occurrences" << endl;
					output << sortedNodes->array[i]->key << " " << freq << endl;
				}else i--;
			}
		}else{
			cerr << "Size exceeded!" << endl;
		}
		output.close();
		output.clear();
}

Set::~Set(){ //Chama a deleção pos ordem 
	postOrderDelete(this->root);
	delete this->Nil;
}

