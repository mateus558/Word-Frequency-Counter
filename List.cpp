#include "List.h"

using namespace std;

template <class T> List<T>::List(T b){
	this->first = new LNODE<T>;	
	this->last = new LNODE<T>;	
	this->nil = new LNODE<T>;
	this->nil->item = b;
	this->first->prev = this->nil;
	this->nil->next = this->first;
	this->last->next = this->nil;
	this->nil->prev = this->last; 
	this->N = 0;
}

template <class T> void List<T>::push_front(T key){
	LNODE<T> *e = new LNODE<T>(key);
	e->next = nil->next;
	nil->next->prev = e;
	nil->next = e;
	e->prev = nil;
	N++;
}

template <class T> void List<T>::pop(){
	nil->next = nil->next->next;
	nil->next->prev = nil->next->next;	
	N--;
}

template <class T> void List<T>::remove(T key){
	LNODE<T> *x = nil->next;
	while(x->item != key){
		x = x->next;
	}
	x->prev->next = x->next;
	x->next->prev = x->prev;
}

template <class T> bool List<T>::search(T key){
	LNODE<T> *x = nil->next;
	while(*x != *nil){
		if(x->item == key)
			return true;
		x = x->next;
	}
	return false;
}

template <class T> void List<T>::display(){
	LNODE<T> *x = nil->next;
	
	while(*x != *nil){
		cout << x->item << endl;
		x = x->next;
	}
	
	cout << endl;
}

template <class T> List<T>::~List(){
	LNODE<T> *tmp;
	while(nil != NULL){
		tmp = nil;
		nil = nil->next;
		delete tmp;
	}
}

template class List<int>;
template class List<string>;
template class List<bool>;
template class List<double>;
template class List<float>;
