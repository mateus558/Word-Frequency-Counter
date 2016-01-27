#include "Hash.h"

Hash::Hash(int m){

	this->m = m;
	N = 0;
	table = new ListH*[m];
	for(int i = 0; i < m; i++){
		table[i] = NULL;
	}
}

int Hash::hashFunc(string data){
	unsigned long hash = 5381;
    int c, i=0;

    while ((c = data[i++]))
        hash = (((hash << 5) + hash) + c); /* hash * 33 + c */

    return hash%m;
}

void Hash::insert(string data){
	if(find(data)) return ;
	int p = hashFunc(data);
	if(table[p] == NULL){
		table[p] = new ListH;
		table[p]->data = new Data(data);
		table[p]->prox = NULL;
		N++;
	}else{
		ListH* e = new ListH;
		e->data = new Data(data);
		e->prox = table[p]; 
		table[p] = e;
		N++;
	} 
}

bool Hash::find(string data){
	int p = hashFunc(data);
	
	if(table[p] != NULL && table[p]->data->key == data){
		table[p]->data->freq++;
		N++;		
		return true;
	}else{
		ListH* itr = table[p];
		while(itr != NULL){
			if(itr->data->key == data){
				itr->data->freq++;
				N++;
				return true;
			}
			itr = itr->prox;
		}
	}
	return false;
}

int Hash::getFreq(string data){
	int p = hashFunc(data);

	if(table[p] != NULL && table[p]->data->key == data){
		return table[p]->data->freq;
	}else{
		ListH* itr = table[p];
		while(itr != NULL){
			if(itr->data->key == data){
				return itr->data->freq;
			}
			itr = itr->prox;
		}
	}
	return -1;
}

void Hash::display(){
	bool linha = false;
	for(int i = 0; i < m; i++){
		ListH* itr = table[i];		
		while(itr != NULL){			
			if(itr->data->key.size() > 5){				
				cout << itr->data->key << " | ";
				linha = true;
			}
			itr = itr->prox;
		}	
		if(linha){				
			cout << endl;
			linha = false;
		}
	}
	cout << endl;
}

Hash::~Hash(){
	for(int i = 0; i < m; i++){
		delete table[i];
	}
}
