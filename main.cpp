#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <algorithm>
#include <ios>
#include <cstdlib>
#include "RBtree.h"

using namespace std;

template <class T> struct List{
	T item;
	int size;
	List<T> *next;
	List(){ size = 0;}
	int getSize(){ return this->size;}	
};

void showMenu(int sizeList);
void clear();
template <class T> List<T>* add(List<T> *l, T key);
template <class T> void displayList(List<T> *l);
RBtree<string> *processFiles(List<string> *files);
List<string>* selectFiles();
void removeSpecialChars(string *word);
void merge(NODE<string> **&A, int p, int q, int r);
void mergeSort(NODE<string> **&A, int p, int r);
void process_mem_usage(double& vm_usage, double& resident_set);

string def = " ";

int main(){
	clear();
	bool erro = false;
	RBtree<string> *DB;
	Container<string> *sortedNodes;
	List<string> *files2Process= new List<string>;
	showMenu(files2Process->getSize());
	int o;	
	while(1){
		if(erro){
			cerr << "\033[1;31mInvalid Option.\033[0m" << endl;
			cout << endl;			
			erro = false;		
		}
		cout << "> ";
		cin >> o;
		switch(o){
			case 1:
				//The user select the files to be processed
				clear();
				files2Process = selectFiles();
				cout << "Do you want to proceed? (y/n): ";
				char a;	
				cin >> a;
				//If the user continue with the files the DB is loaded else the operation is canceled
				if(a == 'y'){
					clock_t start, finish;
					start = clock();
					DB = processFiles(files2Process); 
					sortedNodes = DB->getSortedList();					
					mergeSort(sortedNodes->array, 0, sortedNodes->count); 	
					finish = clock();
					/*for(int i = 0; i < sortedNodes->count; i++){
						cout << sortedNodes->array[i]->key << " - " << sortedNodes->array[i]->count << endl;
					}*/	
					double time = ((double)(finish - start))/CLOCKS_PER_SEC;
					clear();
					showMenu(files2Process->getSize());				
					cout << "\033[1;34mDB loaded in " << time << "\033[1;34m seconds.\033[0m\n" << endl;					//DB->display();					
					double vm = 0.0, re= 0.0;
					process_mem_usage(vm, re);
					cout << DB->getN() << " palavras unicas.\n" << endl;
					cout << "VM: " << vm << "Kb | RSS: " << re << "Kb"<< endl;
				}else{
					files2Process = new List<string>;
					clear();
					showMenu(files2Process->getSize());
					cerr << "\033[1;31mOperation canceled.\033[0m\n" << endl;
				}
				break;
			case 2:
				if(files2Process->getSize() == 0){
					return 0;
				}else{
					clear();
					cout << "Enter X: " << endl;
					int X;
					cin >> X;
					cout << endl;
					for(int i = sortedNodes->count-1; i > (sortedNodes->count-1 - X); i--){
						cout << sortedNodes->array[i]->key << " - " << sortedNodes->array[i]->count << endl; 
					}
					cout << "\nPress anything and ENTER to go back to menu..." << endl; 
					char a;					
					cin >> a;
					clear();
					showMenu(files2Process->getSize());				
				}
				break;
			case 3:
				if(files2Process->getSize() > 0){				
					
				}else{
					erro = true;
					clear();
					showMenu(files2Process->getSize());
				}
				break;
			case 4:
				if(files2Process->getSize() > 0){			
					clear();	
					for(int i = 0; i < sortedNodes->count; i++){
						if(sortedNodes->array[i]->count == 1)
							cout << sortedNodes->array[i]->key << " - " << sortedNodes->array[i]->count << endl; 
					}
					cout << "\nPress anything and ENTER to go back to menu..." << endl; 
					char a;					
					cin >> a;
					clear();
					showMenu(files2Process->getSize());
				}else{
					erro = true;
					clear();
					showMenu(files2Process->getSize());
				}
				break;
			case 5:
				if(files2Process->getSize() > 0){				
					//delete DB;					
					return 0;
				}else{
					erro = true;
					clear();
					showMenu(files2Process->getSize());
				}
				break;
			default: 
				erro = true;
				clear();
				showMenu(files2Process->getSize());
				break;
		}
	}
}

void showMenu(int sizeList){
	cout << "\t-----------------------------------------------------------------------"<< endl;
	cout << "\t|                                UFJF                                 |" <<endl;
	cout << "\t|                                                                     |" << endl;
	cout << "\t|\t\t\tWord frequency counter V1.0                   |" << endl;
	cout << "\t|                                                                     |" << endl;	
	cout << "\t|                                                                     |" << endl;
	cout << "\t|By: Mateus C. Marim                                                  |" << endl;
	cout << "\t|                                                                     |" << endl;
	cout << "\t-----------------------------------------------------------------------"<< endl;
	cout << endl;
	cout << endl;
	cout << "Options:" << endl;
	cout << endl;
	cout << "1 - Select files to process" << endl;
	if(sizeList > 0){
		cout << "2 - X more frequent words in all DB" << endl;
		cout << "3 - X more frequent words in specific DB" << endl;
		cout << "4 - All words that occur only one time" << endl;
		cout << "5 - Exit program" << endl;
	}else{
		cout << "2 - Exit program" << endl;
	}
	cout << endl;
}

void clear(){
	for(int i = 0; i < 40; i++) cout << endl;
}

template <class T> List<T>* add(List<T> *l, T key){
	List<T> *e = new List<T>;
	e->item = key;
	e->next = l;
	e->size = l->size+1;
	return e;
}

template <class T> void displayList(List<T> *l){
	List<string> *x = l;
	while(x != NULL){
		cout << x->item << endl;
		x = x->next;
	} 
	cout << endl;
}

List<string>* selectFiles(){
	cout << "Inform the txt with the name of the files you want to process. (The files must be in the directory of the program)" << endl;
	List<string> *files = new List<string>;
	string file;
	cout << "\nTXT with the names of the files: " << endl;
	string txt;
	cout << "> ";
	cin >> txt;
	ifstream names(txt.c_str(), ios::in);
	if(!names){
		cerr << "\033[1;31mFile could not be opened.\033[0m\n" << endl;
		exit(1);
	}
	string name;
	while(names >> name){
		files = add(files, name);
	}
	cout << "\n-Selected files:\n";
	displayList(files);
	cout << "\n" << files->size << " files found..." << endl;
	return files; 	
}

RBtree<string> *processFiles(List<string> *files){
	List<string> *x = files;	
	RBtree<string> *temp = new RBtree<string>(def);
	ifstream inFile[files->size];
	int i = 0, tam = x->size;

	//Abre arquivos com os nomes contidos na lista 
	while(x != NULL){
		if(i == tam) break;
		inFile[i].open((x->item).c_str(), ios::in);				

		cout << "Processing " << (x->item).c_str() << "..." << endl;
		if(!inFile[i]){
			cerr << "\033[1;31mFile could not be opened.\033[0m\n" << endl;
			exit(1);
		} 

		string word;		
		//Coloca as palavras do arquivo na estrutura		
		while(inFile[i] >> word){
			//double vm = 0.0, re= 0.0;
			//process_mem_usage(vm, re);
			//cout << "VM: " << vm << "Kb | RSS: " << re << "Kb"<< endl;
			removeSpecialChars(&word);
			if(!word.empty())
				temp->RB_insert(word);
		}
		inFile[i].close();
		inFile[i].clear();
		x = x->next;
		i++;
 	}
	return temp;
}

void removeSpecialChars(string *word){
	word->resize(remove_if(word->begin(), word->end(),[](char x){return !isalnum(x) && !isspace(x);})-word->begin());
}

void merge(NODE<string> **&A, int p, int q, int r){
	int i, j , k;
	NODE<string>** w = new NODE<string>*[r-p];
	for(int n = 0; n < (r-p); n++){
		w[n] = NULL;
	}	
	i = p; j = q;
	k = 0;
	while(i < q && j < r){
		if(A[i]->count <= A[j]->count) w[k++] = A[i++];
		else w[k++] = A[j++];
	}
	while(i < q) w[k++] = A[i++];
	while(j < r) w[k++] = A[j++];
	for(i = p; i < r; ++i) A[i] = w[i - p];
}

void mergeSort(NODE<string> **&A, int p, int r){
	if(p < r){
	//	cout << "a" << endl;
		int q = (p + r)/2;
		mergeSort(A, p, q);
		mergeSort(A, q + 1, r);
		merge(A, p, q, r);
	}
}
//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void process_mem_usage(double& vm_usage, double& resident_set){
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}
