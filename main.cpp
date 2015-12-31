#include <iostream>
#include <string>
#include <dirent.h>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <algorithm>
#include <ios>
#include <cstdlib>
#include "List.h"
#include "RBtree.h"

using namespace std;

typedef RBtree<string> set;

/*template <class T> struct List{
	T item;
	int size;
	List<T> *next;
	List(){ size = 0;}
	int getSize(){ return this->size;}
};*/

void showMenu(int sizeList);
void clear();
set *processFiles();
void selectFiles();
void removeSpecialChars(string *word);
void merge(NODE<string> **arr, int l, int m, int r);
void mergeSort(NODE<string> **arr, int l, int r);
bool validFile(string file);
void process_mem_usage(double& vm_usage, double& resident_set);

string def = " ";
Container<string> *sortedNodes;	//Container para receber os nos ordenados
List<string> *files2Process;	//Lista para os arquivos a serem processados
set *DB;	//Conjunto para adicionar as palavras (Arvore vermelho-preta)
DIR *dpdf;
struct dirent *epdf;

int main(){
	clear();
	bool erro = false;	
	files2Process = new List<string>("");
	showMenu(files2Process->size());
	int o;	
	while(1){
		if(erro){
			cerr << "\033[1;31mInvalid Option.\033[0m" << endl;
			cout << endl;			
			erro = false;		
		}
		double vm= 0.0, re = 0.0;
		//Mostra o consumo de memória do programa
		process_mem_usage(vm, re);
		cout << "VM: " << vm << "Kb | RSS: " << re << "Kb"<< endl;
		cout << "\n> ";
		cin >> o;
		switch(o){
			case 1:
				clear();
				//O programa faz detecção dos arquivos a serem processados na pasta de entrada				
				selectFiles();
				char a;	
				if(files2Process->size() > 0){
					cout << "Do you want to proceed? (y/n): ";
					cin >> a;
				}else{
					delete files2Process;
					a = 'n';
				}
				//Se o usuário escolhe continuar com os arquivos eles são processados senao a operação é cancelada
				if(a == 'y'){
					//Cria variáveis para contar o tempo de execução das ações
					clock_t start, finish;
					start = clock();
					//Coloca os dados processados no conjunto
					DB = processFiles(); 
					//Retorna uma lista com os nós ordenados lexicograficamente
					sortedNodes = DB->getSortedList();
					//Ordena os nos por frequencia mantendo a ordem em que aparecem					
					mergeSort(sortedNodes->array, 0, sortedNodes->count-1);
					finish = clock();
					/*for(int i = 0; i < sortedNodes->count; i++){
						cout << sortedNodes->array[i]->key << " - " << sortedNodes->array[i]->count << endl;
					}*/	
					double time = ((double)(finish - start))/CLOCKS_PER_SEC;
					clear();
					showMenu(DB->getN());				
					//Dados sobre o carregamento do conjunto								
					cout << "\033[1;34mDB loaded in " << time << "\033[1;34m seconds.\033[0m\n" << endl;					//DB->display();					
					cout << DB->getN() << " palavras unicas.\n" << endl;
				}else{
					files2Process = new List<string>("");
					clear();
					showMenu(DB->getN());
					cerr << "\033[1;31mOperation canceled.\033[0m\n" << endl;
				}
				break;
			case 2:
				if(DB->getN() == 0){
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
					showMenu(DB->getN());				
				}
				break;
			case 3:
				if(DB->getN() > 0){				
					
				}else{
					erro = true;
					clear();
					showMenu(DB->getN());
				}
				break;
			case 4:
				if(DB->getN() > 0){			
					clear();	
					int i  = 0;
					while(sortedNodes->array[i]->count == 1){
						cout << sortedNodes->array[i]->key << " - " << sortedNodes->array[i]->count << endl; 					i++;
					}
					cout << "\nPress anything and ENTER to go back to menu..." << endl; 
					char a;					
					cin >> a;
					clear();
					showMenu(DB->getN());
				}else{
					erro = true;
					clear();
					showMenu(DB->getN());
				}
				break;
			case 5:
				if(DB->getN() > 0){				
					//delete DB;					
					return 0;
				}else{
					erro = true;
					clear();
					showMenu(DB->getN());
				}
				break;
			default: 
				erro = true;
				clear();
				showMenu(DB->getN());
				break;
		}
	}
}
//Mostra o menu principal
void showMenu(int sizeList){
	cout << "\t\033[42;30m-----------------------------------------------------------------------\033[0m"<< endl;
	cout << "\t\033[42;30m|                                UFJF                                 |\033[0m" <<endl;
	cout << "\t\033[42;30m|                                                                     |\033[0m" << endl;
	cout << "\t\033[42;30m|\t\t\tWord frequency counter V1.0                   |\033[0m" << endl;
	cout << "\t\033[42;30m|                                                                     |\033[0m" << endl;	
	cout << "\t\033[42;30m|                                                                     |\033[0m" << endl;
	cout << "\t\033[42;30m|By: Mateus C. Marim                                                  |\033[0m" << endl;
	cout << "\t\033[42;30m|                                                                     |\033[0m" << endl;
	cout << "\t\033[42;30m-----------------------------------------------------------------------\033[0m"<< endl;
	cout << "\t\033[102;30m                                                                       \033[0m" << endl;
	cout << "\t\033[102;30m                                                                       \033[0m" << endl;
	cout << "\t\033[102;30mOptions:                                                               \033[0m" << endl;
	cout << endl;
	cout << "\t1 - Select files to process" << endl;
	if(sizeList > 0){
		cout << "\t2 - X more frequent words in all DB" << endl;
		cout << "\t3 - X more frequent words in specific DB" << endl;
		cout << "\t4 - All words that occur only one time" << endl;
		cout << "\t5 - Exit program" << endl;
	}else{
		cout << "\t2 - Exit program" << endl;
	}
	cout << endl;
}
//Limpa a tela
void clear(){
	for(int i = 0; i < 40; i++) cout << endl;
}

bool validFile(string file){
	if(file.empty() || file.size() < 4)
		return false;
	string test = ".txt";
	int j = 3;
	for(int i = file.size()-1; i > file.size() - 5; i--, j--){
		if(file[i] != test[j])
			return false;
	}
	return true;
}

void selectFiles(){
	cout << "The program will detect the files to be processed in the folder \"Input\", wish to continue? (y/n) " << endl;
	cout << "> ";
	char a;	
	cin >> a;
	if(a == 'n')
		return;
	//Abre o diretório com os arquivos para serem processados
	dpdf = opendir("./Input");
	if(dpdf != NULL){
		//Faz leitura dos arquivos no diretório e os adiciona na lista para processamento
		while(epdf = readdir(dpdf)){
			string file = string(epdf->d_name);
			if(validFile(file)){ 		
				files2Process->push_front(file);
			}
		}
	}
	closedir(dpdf);
	//Mostra ao usuário os arquivos escolhidos para processamento
	cout << "\n-Files detected:\n";
	files2Process->display();
	cout << "\n" << files2Process->size() << " files found...\n" << endl;	
}

set *processFiles(){
	List<string> *x = files2Process;	
	set *temp = new set(def);
	ifstream inFile[files2Process->size()];
	int i = 0, tam = x->size();

	//Abre arquivos contidos na lista
	while(x->front() != ""){
		if(i == tam) break;
		inFile[i].open(string("Input/") + (x->front()).c_str(), ios::in);				

		cout << "Processing " << (x->front()).c_str() << "..." << endl;
		if(!inFile[i]){
			cerr << "\033[1;31mFile could not be opened.\033[0m\n" << endl;
			exit(1);
		} 

		string word;		
		//Coloca as palavras do arquivo na estrutura		
		while(inFile[i] >> word){
			//Remove caracteres especiais das palavras
			removeSpecialChars(&word);
			if(!word.empty())
				temp->RB_insert(word);
		}
		inFile[i].close();
		inFile[i].clear();
		x->pop();
		i++;
 	}
	return temp;
}
//Funcao para remocao de caracteres especiais
void removeSpecialChars(string *word){
	word->resize(remove_if(word->begin(), word->end(),[](char x){return !isalnum(x) && !isspace(x);})-word->begin());
}

void merge(NODE<string> **arr, int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    NODE<string> *L[n1], *R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for(i = 0; i < n1; i++) L[i] = arr[l + i];
    for(j = 0; j < n2; j++) R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; j = 0; k = l;
    while (i < n1 && j < n2){
        if (L[i]->count <= R[j]->count){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there are any */
    while (i < n1){
        arr[k] = L[i];
        i++; k++;
    }
 
    /* Copy the remaining elements of R[], if there are any */
    while (j < n2){
        arr[k] = R[j];
        j++; k++;
    }
}
 
/* l is for left index and r is right index of the sub-array
  of arr to be sorted */
void mergeSort(NODE<string> **arr, int l, int r){
    if (l < r){
        int m = l+(r-l)/2; //Same as (l+r)/2, but avoids overflow for large l and h
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
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
