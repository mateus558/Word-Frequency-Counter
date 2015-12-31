#ifndef LIST_H_
#define LIST_H_
#include <iostream>

template <typename T> struct LNODE{
	T item;
	LNODE* next;
	LNODE* prev;
	LNODE(){
		next = NULL;
		prev = NULL;
	}
	LNODE(T item){
		this->item = item;
		next = NULL;
		prev = NULL;
	}
	bool operator !=(const LNODE& no){
			//std::cout << no.item != this->item <<std::endl;
			return no.item != this->item;
	}
};

template <class T> class List{
	private:		
		struct LNODE<T> *first;
		struct LNODE<T> *last;
		struct LNODE<T> *nil;
		bool init;
		int N;
	public:
		List(T b);
		void push_front(T key);
		void push_back(T key);
		void pop();
		T front(){return nil->next->item;}
		void remove(T key);
		bool search(T key);
		int size(){return N;}
		void display();		
		~List();
};

#endif
