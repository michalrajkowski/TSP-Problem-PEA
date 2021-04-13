#include <iostream>
#include <limits.h>
#include <algorithm>
#include <vector>
#include<conio.h>
#include<string>
#include<fstream>
#include<cstdlib>
#include <time.h>
#include <new>
#include <vector>
#include <queue>

using namespace std;

int** tab;
int cnt;
clock_t start, end;
double cpu_time;

ofstream myfile;
string outFileName = "test.csv";
string finalOutFileName = "test_out_name.csv";
string daneWejsciowe = "dane_wejsciowe.ini";

Matrix(){
	cnt = 1;
	tab = new int*[cnt];
	for (int i = 0; i < cnt; i++){
		tab[i] = new int[cnt];
		for(int j = 0; j < cnt; j++){
			tab[i][j] = 0;
		}
	}
	cnt = 0;
}

void createMatrix(int _cnt){
	cnt = _cnt;
	tab = new int*[cnt];
	for (int i = 0; i < cnt; i++){
		tab[i] = new int[cnt];
		for(int j = 0; j < cnt; j++){
			tab[i][j] = 0;
		}
	}
}

void display(){
	cout << "Reprezentacja Macierzowa:\n";
	cout << "  ";
	for (int i = 0; i < cnt; i++){
		cout << i << " ";
	}
	cout << "\n";
	for (int j = 0; j < cnt; j++){
		cout << j << " ";
		for(int i = 0; i < cnt; i++){
			cout << tab[j][i] << " ";
		}
		cout << "\n";
	}
}

void deleteMatrix(){
	for (int i = 0; i < cnt; i++){
		delete[] tab[i];
	}
	delete[] tab;
}

int loadFromFile(string FileName){
	int rozmiar;
    ifstream inFile;
    inFile.open(FileName.c_str());
    if (!inFile) {
        cout << "Blad zwiazany z otwarciem pliku\n";
    }else{
    	inFile >> rozmiar;
	}
	deleteMatrix();
	createMatrix(rozmiar);
	
	
	int w;
	for(int i = 0; i < rozmiar; i++){
		for(int j = 0; j < rozmiar; j++){
			inFile >> w;
			tab[i][j] = w;
		}
	}
    
    inFile.close();
}

int loadFromGr(string FileName){
	int rozmiar;
    ifstream inFile;
    inFile.open(FileName.c_str());
    if (!inFile) {
        cout << "Blad zwiazany z otwarciem pliku\n";
    }else{
    	inFile >> rozmiar;
	}
	deleteMatrix();
	createMatrix(rozmiar);
	
	
	int w;
	for(int i = 0; i < rozmiar; i++){
		for(int j = 0; j < i+1; j++){
			inFile >> w;
			if(j == i){
				tab[i][j] = 0;
			}else{
				tab[i][j] = w;
				tab[j][i] = w;
			}
		}
	}
    
    inFile.close();
}

class Node{
	public:
		int Value;
		Node* ChildNodes;
		bool Selected;
};



//Algorithm global space

int *path;

int *path_final;

bool *visited;

int opt;


int findEnter(int node){
	int min = INT_MAX;
	for(int i = 0; i < cnt; i++){
		if(i == node) 
			continue; 
		if(tab[node][i] < min)
			min = tab[node][i];
	}
	return min;
}
int findLeave(int node){
	int min = INT_MAX;
	for(int i = 0; i < cnt; i++){
		if(i == node) 
			continue; 
		if(tab[i][node] < min)
			min = tab[node][i];
	}
	return min;
}


//Algorithm global space end

void Recursion(int LE, int MIL, int depth){
	if(depth == cnt){
		
		int result = MIL + tab[path[depth-1]][0];
		
		if(result < opt){
			opt = result; 
			for(int i = 1; i < cnt; i++){
				path_final[i] = path[i];
			}
		}
		
		return;
	}
	
	for(int i = 0; i < cnt; i++){
		if(i == path[depth - 1] || visited[i] == true)
		continue;
		
		int new_MIL = MIL;
		int new_LE = LE; 
		new_MIL += tab[path[depth - 1]][i];
		new_LE -= (findEnter(i) + findLeave(path[depth - 1])) /2;
		
		if(new_LE + new_MIL < opt){
			path[depth] = i;
			visited[i] = true;
			
			Recursion(new_LE, new_MIL, depth + 1);
		}
		visited[i] = false;
		
		
	}	
	
}

int TSP_BnB(){  

	path = new int[cnt];
	
	opt = INT_MAX;
	int MIL = 0;
	int depth = 0;
	
	path_final = new int[cnt + 1];

	visited = new bool[cnt];	
	
	int LE = 0;
	
	for(int i = 0; i < cnt; i++){
		visited[i] = false;
	}
	visited[0] = true;
	
	for(int i = 0; i < cnt; i++){
		LE += findEnter(i);
		LE += findLeave(i);
	}
	LE = LE / 2;

	path[0] = 0;
	path[cnt] = 0;
	
	Recursion(LE, MIL, depth + 1);
	
}
                         

int main()
{
	
	//ofstream myfile;
	//string outFileName = "t1.csv";
    //char readData[100];
    //GetPrivateProfileString(NULL, NULL, NULL, readData, 100, "dane_wejsciowe.ini");
    
    Matrix();
    
    /*
	loadFromFile("br17.txt");

	TSP_BnB();
	
	cout << opt;
	*/
    
    
    ifstream inFileMain;
    string temp;
    int temp_num;
    int liczba_powtorzen;
    inFileMain.open(daneWejsciowe.c_str());
    myfile.open (outFileName.c_str(), ios::app);
    if (!inFileMain) {
        cout << "Blad zwiazany z otwarciem pliku\n";
    }else{
    	while(!inFileMain.eof()){
    		inFileMain >> temp;
    		myfile << temp << " ";
    		if(inFileMain.eof()){
				break;
			}
			
			
			string first_two = temp.substr(0, 2);
			if(first_two == "gr"){
				loadFromGr(temp.c_str());
				
			}else{
				loadFromFile(temp.c_str());
				
			}		
			inFileMain >> liczba_powtorzen;
			myfile << liczba_powtorzen << " ";
			for (int i = 0; i < cnt+2; i++){
				inFileMain >> temp_num;
				myfile << temp_num << " ";
			}
			myfile << "\n";
			for(int i = 0; i < liczba_powtorzen; i++){
			start = clock();
		
			TSP_BnB();
		
			end = clock();
			cpu_time = ((double) (end - start)) / (CLOCKS_PER_SEC/1000);
			myfile << cpu_time<<"\n";		
	}
		}
	}
	
    finalOutFileName = temp;
    
    inFileMain.close();
    myfile.close();
   
    remove(finalOutFileName.c_str());
    rename(outFileName.c_str(), finalOutFileName.c_str());
    
    

    
	return 0;
	
}
