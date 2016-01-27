#include "stack.h"
#include "Set.h"
#include <dirent.h>
#include <ctime>
#include <unistd.h>
#include <ios>
#include <sstream>
#include <vector>
#include <cstdlib>

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
stack<string> files2Process;	//Lista para os arquivos a serem processados
Set *DB;	//Conjunto para adicionar as palavras (Arvore vermelho-preta)
Set *file;	
Container *sortedNodesFile;	//Container para receber os nos ordenados
DIR *dpdf;
bool erro;
int nFiles;
struct dirent *epdf;

int main(){
	clear();
	erro = false;	
	showMenu(files2Process.size());
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
				//Carrega dados dos arquivos na Set
				opcao1();
				break;
			case 2:
				//Mostra X palavras mais frequentes no conjunto
				opcao3();
				break;
			case 3:
				//Mostra X palavras mais frequentes em arquivo 
				opcao2();
				break;
			case 4:
				//Mostra todas palavras que aparecem apenas uma vez no conjunto
				opcao4();
				break;
			case 5:
				//Mostra arquivos usados pela DB
				clear();
				DB->displayUsedFiles();
				cout <<"\n" << DB->getNFiles() << " files processed." << endl;
				cout << "\nTo go back to menu press any key and ENTER...\n" << endl;
				char a;
				cin >> a;
				clear();
				showMenu(DB->getN());
				break;
			case 6:
				//Sai do programa
				opcao5();
				break;
			case 7:
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
	cout << "Cleaning last DB, if it existed...\n" << endl;
	delete DB;
	//O programa faz detecção dos arquivos a serem processados na pasta de entrada				
	selectFiles();
	char a;	
	if(files2Process.size() > 0){
		cout << "Do you want to proceed? (y/n): ";
		cin >> a;
	}else{
		while(!files2Process.empty()) files2Process.pop();
		a = 'n';
	}
	//Se o usuário escolhe continuar com os arquivos eles são processados senao a operação é cancelada
	if(a == 'y'){
		//Cria variáveis para contar o tempo de execução das ações
		clock_t start, finish;
		start = clock();
		//Coloca os dados processados no conjunto
		processFiles(); 
		sortedNodes = DB->result();
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
		while(!files2Process.empty()) files2Process.pop();
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
		cout << "Enter X (Total size: " << DB->getN() << "): " << endl;
		int X;
		cin >> X;
		cout << endl;
		DB->displayXmoreFrequent(X);
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

		cout << "Type the name of the file:" << endl;
		cout << "> ";
		cin >> selFile;
			
		cout << "\nEnter X (Total size: " << DB->getN() << "): " << endl;
		int X;
		cin >> X;
		cout << endl;
		DB->displayXmoreFrequentByFile(selFile, X);
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
		ofstream output("Output/opt4.txt", ios::out);	//Abre arquivo para saída
		if(!output){
			clear();
			showMenu(DB->getN());
			cerr << "\033[1;31mFile could not be opened.\033[0m\n" << endl;
			return;	
		}
		int i  = sortedNodes->count;	
		while(i-- && sortedNodes->array[i]->count == 1);	//Leva o i na posição em que a frequencia 1 começa 
		i++;
		if(i < sortedNodes->count){	//Imprime os nós de frequencia 1 se existirem
			do{
				cout << sortedNodes->array[i]->key << " - " << sortedNodes->array[i]->count << " occurrence"<< endl;
				output << sortedNodes->array[i]->key << " " << sortedNodes->array[i]->count << endl;
			}while(++i && i < sortedNodes->count);
		}else{
			cout << "Nao existem palavras que ocorrem apenas uma vez!" << endl;
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
	cout << "\t\033[42;30m|\t\tWord frequency counter V3.1415926...	             \t|\033[0m" << endl;
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
	cout << "\t2 - X more frequent words in specific DB" << endl;
	if(sizeList > 0){
		cout << "\t3 - X more frequent words in all DB" << endl;
		cout << "\t4 - All words that occur only one time" << endl;
		cout << "\t5 - Display the name of the files processed" << endl;
		cout << "\t6 - Exit program" << endl;
	}else{
		cout << "\t3 - Exit program" << endl;
	}
	cout << endl;
}
//Limpa a tela
void clear(){
	for(int i = 0; i < 50; i++) cout << endl;
}

string formatStr(string str){	
	string result;
	//Retira os caracteres especiais e joga as letras maiusculas para minusculas
	for(unsigned int i = 0; i < str.size(); i++){
		if((str[i] >= 'a' && str[i] <= 'z')){
			result.push_back(str[i]);
		}else if(str[i] >= 'A' && str[i] <= 'Z'){
			result.push_back((char)((int)(str[i]) + 32));
		}  
	}
	return result;
}

bool validFile(string file){	//Verifica se o arquivo é válido
	//Verifica se o nome do arquivo não está vazio ou se o tamanho é menor que 3
	if(file.empty() || file.size() < 4)
		return false;
	//Verifica se o arquivo é do tipo txt
	string test = ".txt";
	int j = 3;
	for(unsigned int i = file.size()-1; i > file.size() - 5; i--, j--){
		if(file[i] != test[j])
			return false;
	}
	//Se todas condições anteriores são verdadeiras, o arquivo é válido
	return true;
}

void selectFiles(){
	cout << "The program will detect the files to be processed in the folder \"Input\", wish to continue? (y/n) " << endl;
	cout << "> ";
	char a;	
	cin >> a;
	if(a == 'n')
		return;
	while(!files2Process.empty()){files2Process.pop();}
	//Abre o diretório com os arquivos para serem processados
	dpdf = opendir("./Input");
	if(dpdf != NULL){
		//Faz leitura dos arquivos no diretório e os adiciona na lista para processamento
		while((epdf = readdir(dpdf))){
			string file = string(epdf->d_name);
			if(validFile(file) && !file.empty()){ 		
				files2Process.push(file);
			}
		}
	}
	closedir(dpdf);
	//Mostra ao usuário os arquivos escolhidos para processamento
	cout << "\n-Files detected:\n";
	stack<string> temp;
	while(!files2Process.empty()){
		cout << files2Process.top() << endl;
		temp.push(files2Process.top());		
		files2Process.pop();
	}
	files2Process = temp;
	cout << "\n" << files2Process.size() << " files found...\n" << endl;	
	nFiles = files2Process.size();
}

void processFiles(){
	stack<string> temp = files2Process;	
	ifstream inFile[files2Process.size()];	
	DB = new Set(nFiles);
	int i = 0;

	//Abre arquivos contidos na lista
	while(!temp.empty()){ 
		string file2open = string("Input/") + string((temp.top()).c_str());
		inFile[i].open(file2open, ios::in);				
		cout << "Processing " << (temp.top()).c_str() << "..." << endl;
		if(!inFile[i]){			
			cerr << "\033[1;31mFile could not be opened.\033[0m\n" << endl;
			return;
		} 
		string word;		
		//Coloca as palavras do arquivo na estrutura		
		while(inFile[i] >> word){
			//Remove caracteres especiais das palavra			
			word = formatStr(word);
			if(!word.empty()){
				DB->RB_insert(word, temp.top());
			}
		}
		inFile[i].close();
		inFile[i].clear();
		i++;
		DB->insertUsedFile(temp.top());
		temp.pop();
 	}
}

//Análise de desempenho automático
void runTimeAnal(){
	vector<string> *temp = new vector<string>;	
	dpdf = opendir("./Input");
	//Faz leitura dos arquivos no diretório e os adiciona na lista para processamento	
	if(dpdf != NULL){
		while((epdf = readdir(dpdf))){
			string file = string(epdf->d_name);
			if(validFile(file)){ 		
				temp->push_back(file);
			}
		}
	}
	closedir(dpdf);
	//Abre arquivo de saída para escrita	
	ofstream analysis("Output/runa.csv",ios::out);
	analysis << "N,Palavras Unicas,T(N)" << endl;
	//Realiza os experimentos aumentando o número de arquivos processados em 1 a cada iteração do for principal
	for(unsigned int i = 0; i <= temp->size(); i++){
		DB = new Set(i);
		clock_t start, finish;
		start = clock();		
		for(unsigned int j = 0; j < i; j++){
			//Abre arquivo para processar as palavras
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
					DB->RB_insert(word, name);
			}
			inFile[j].close();
			inFile[j].clear();
		}
		sortedNodes = DB->result();
		finish = clock();	
		double time = ((double)(finish - start))/CLOCKS_PER_SEC;
		//Substitui o . no tempo double por ,
		ostringstream s;
		s << time;
		string t = s.str();
		for(unsigned int n = 0; n < t.size(); n++)
			if(t[n] == '.')
				t[n] = ','; 
		//Manda os resultados do experimento para o arquivo de saída
		analysis << DB->getTotal() << "," << DB->getN() << "," << "\"" << t << "\"" << endl;
		cout << DB->getTotal() << " " << DB->getN() << " " << " " << t << endl;
		delete DB;
	}
	analysis.close();
	analysis.clear();
}

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent key for a process' virtual memory
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
