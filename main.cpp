#include <iostream>
#include <string>
#include <dirent.h>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <ios>
#include <sstream>
#include <vector>
#include <cstdlib>
#include "List.h"
#include "RBtree.h"

using namespace std;

void opcao1();
void opcao2();
void opcao3();
void opcao4();
void opcao5();
void showMenu(int sizeList);
void clear();
string formatStr(string str);
void processFiles();
void selectFiles();
void merge(NODE **arr, int l, int m, int r);
void mergeSort(NODE **arr, int l, int r);
bool validFile(string file);
void runTimeAnal();
void process_mem_usage(double& vm_usage, double& resident_set);

string def = " ";
Container *sortedNodes;	//Container para receber os nos ordenados
List<string> *files2Process;	//Lista para os arquivos a serem processados
RBtree *DB;	//Conjunto para adicionar as palavras (Arvore vermelho-preta)
RBtree *file;	
Container *sortedNodesFile;	//Container para receber os nos ordenados
DIR *dpdf;
bool erro;
struct dirent *epdf;

int main(){
	clear();
	erro = false;	
	files2Process = new List<string>("");
	DB = new RBtree;
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
				//Carrega dados dos arquivos na RBtree
				opcao1();
				break;
			case 2:
				//Mostra X palavras mais frequentes no conjunto
				opcao2();
				break;
			case 3:
				//Mostra X palavras mais frequentes em arquivo 
				opcao3();
				break;
			case 4:
				//Mostra todas palavras que aparecem apenas uma vez no conjunto
				opcao4();
				break;
			case 5:
				//Sai do programa
				opcao5();
				break;
			case 6:
				runTimeAnal();
				break;
			default: 
				//Caso seja informada uma opcao inválida o programa volta no menu
				erro = true;
				clear();
				showMenu(DB->getN());
				break;
		}
	}
}

//Salva arquivos em Container em um arquivo
void save2File(string fileName, Container *array){
	string path = "Output/" + fileName;	
	ofstream output(path, ios::out);
	
	if(!output){
		cerr << "\033[1;31mFile could not be opened.\033[0m\n" << endl;
		exit(3);
	}
	for(int i = 0; i < array->count; i++){
			output << array->array[i]->key << " " << array->array[i]->count << endl; 
	}

	output.close();
	output.clear();
}	

void opcao1(){
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
		processFiles(); 
		//Retorna uma lista com os nós ordenados lexicograficamente
		sortedNodes = DB->getSortedList();
		//Ordena os nos por frequencia mantendo a ordem em que aparecem					
		mergeSort(sortedNodes->array, 0, sortedNodes->count-1);
		finish = clock();	
		double time = ((double)(finish - start))/CLOCKS_PER_SEC;
		save2File("opt1.txt", sortedNodes);		
		clear();
		showMenu(DB->getN());				
		//Dados sobre o carregamento do conjunto								
		cout << "\033[1;34mDB loaded in " << time << "\033[1;34m seconds.\033[0m\n" << endl;		
		//DB->display();					
		cout << DB->getN() << " palavras unicas e total de " << DB->getTotal() << " palavras.\n" << endl;
	}else{
		files2Process = new List<string>("");
		clear();
		showMenu(DB->getN());
		cerr << "\033[1;31mOperation canceled.\033[0m\n" << endl;
	}
}

void opcao2(){
	if(DB->getN() == 0){
		exit(0);
	}else{
		clear();
		cout << "Enter X: " << endl;
		int X;
		cin >> X;
		cout << endl;
		ofstream output("Output/opt2.txt", ios::out);
		if(!output){
			cerr << "Error opening the file" << endl;
		}	
		for(int i = 0; i < X; i++){
			cout << sortedNodes->array[i]->key << " - " << sortedNodes->array[i]->count << " occurrences" << endl; 
			output << sortedNodes->array[i]->key << " " << sortedNodes->array[i]->count << endl;	
		}
		output.close();
		output.clear();
		cout << "\nPress anything and ENTER to go back to menu..." << endl; 
		char a;					
		cin >> a;
		clear();
		showMenu(DB->getN());				
	}
}

void opcao3(){
	if(DB->getN() > 0){				
		clear();
		string selFile;
		ifstream inFile;
		ofstream output("Output/opt3.txt", ios::out);		
		file = new RBtree;		

		cout << "Type the name of the file:" << endl;
		cout << "> ";
		cin >> selFile;
		output << selFile + "\n" << endl;
		inFile.open(string("Input/") + selFile.c_str(), ios::in);
		if(!inFile){
			cerr << "\033[1;31mFile could not be opened.\033[0m\n" << endl;
			exit(1);
		} 
		string word;
		while(inFile >> word){
			//removeSpecialChars(&word);
			word = formatStr(word);			
			if(!word.empty()){
				file->RB_insert(word);
			}				
		}
		inFile.close();
		sortedNodesFile = file->getSortedList();
		//Ordena os nos por frequencia mantendo a ordem em que aparecem					
		mergeSort(sortedNodesFile->array, 0, sortedNodesFile->count-1);
		cout << "\nEnter X: " << endl;
		int X;
		cin >> X;
		cout << endl;
		for(int i = sortedNodesFile->count-1; i > (sortedNodesFile->count-1 - X); i--){
			cout << sortedNodesFile->array[i]->key << " - " << sortedNodesFile->array[i]->count << " occurrences" << endl;
			output << sortedNodesFile->array[i]->key << " " << sortedNodesFile->array[i]->count << endl;
		}
		output.close();
		output.clear();
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
}

void opcao4(){
	if(DB->getN() > 0){			
		clear();	
		int i  = 0;
		ofstream output("Output/opt4.txt", ios::out);
		if(!output){
			cerr << "Error opening the file!" << endl;
		}
		while(sortedNodes->array[i]->count == 1 && i++){
			cout << sortedNodes->array[i]->key << " - " << sortedNodes->array[i]->count << " occurrences"<< endl;
			output << sortedNodes->array[i]->key << " " << sortedNodes->array[i]->count << endl; 				
		}
		output.close();
		output.clear();

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
}

void opcao5(){
	if(DB->getN() > 0){
		delete DB;				
		exit(2);
	}else{
		erro = true;
		clear();
		showMenu(DB->getN());
	}
}

//Mostra o menu principal
void showMenu(int sizeList){
	cout << "\t\033[42;30m-----------------------------------------------------------------------\033[0m"<< endl;
	cout << "\t\033[42;30m|                                UFJF                                 |\033[0m" <<endl;
	cout << "\t\033[42;30m|                                                                     |\033[0m" << endl;
	cout << "\t\t\t\033[42;30m|Word frequency counter V1.0                   |\033[0m" << endl;
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

string formatStr(string str){	
	string result;
	for(int i = 0; i < str.size(); i++){
		if((str[i] >= 'a' && str[i] <= 'z')){
			result.push_back(str[i]);
		}else if(str[i] >= 'A' && str[i] <= 'Z'){
			result.push_back((char)((int)(str[i]) + 32));
		}  
	}
	return result;
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

void processFiles(){
	List<string> *x = files2Process;	
	ifstream inFile[files2Process->size()];
	DB = new RBtree;
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
			word = formatStr(word);
			if(!word.empty())
				DB->RB_insert(word);
		}
		inFile[i].close();
		inFile[i].clear();
		x->pop();
		i++;
 	}
}

void merge(NODE **arr, int l, int m, int r){
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
        if (L[i]->count > R[j]->count){
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
 
/* l é para o indice da esquerda e r é o indice da direita do sub-array para ser ordenado */
void mergeSort(NODE **arr, int l, int r){
    if (l < r){
        int m = l+(r-l)/2; //O mesmo que (l+r)/2, mas evita overflow para valores grandes de l e h
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

//Análise de desempenho automático
void runTimeAnal(){
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
	vector<string> *temp = new vector<string>;
	while(files2Process->front() != ""){
		temp->push_back(files2Process->front());
		files2Process->pop();
	} 
	ofstream analysis("Output/runa.csv",ios::out);
	analysis << "N,Palavras Unicas,T(N)" << endl;
	for(int i = 0; i <= temp->size(); i++){
		DB = new RBtree;
		clock_t start, finish;
		start = clock();		
		for(int j = 0; j < i; j++){
			//Coloca os dados processados no conjunto
			ifstream inFile[i];
			string name = temp->at(j);
			inFile[j].open(string("Input/") + name.c_str(), ios::in);				
			if(!inFile[j]){
				cerr << "\033[1;31mFile could not be opened.\033[0m\n" << endl;
				exit(1);
			} 
			string word;		
			//Coloca as palavras do arquivo na estrutura		
			while(inFile[j] >> word){
			//Remove caracteres especiais das palavras
				word = formatStr(word);
				if(!word.empty())
					DB->RB_insert(word);
			}
			inFile[j].close();
			inFile[j].clear();
		}
		//Retorna uma lista com os nós ordenados lexicograficamente
		sortedNodes = DB->getSortedList();
		//Ordena os nos por frequencia mantendo a ordem em que aparecem					
		mergeSort(sortedNodes->array, 0, sortedNodes->count-1);
		finish = clock();	
		double time = ((double)(finish - start))/CLOCKS_PER_SEC;
		ostringstream s;
		s << time;
		string t = s.str();
		for(int n = 0; n < t.size(); n++)
			if(t[n] == '.')
				t[n] = ','; 
		analysis << DB->getTotal() << "," << DB->getN() << "," << "\"" << t << "\"" << endl;
		cout << DB->getTotal() << " " << DB->getN() << " " << " " << t << endl;
	}
	analysis.close();
	analysis.clear();
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
