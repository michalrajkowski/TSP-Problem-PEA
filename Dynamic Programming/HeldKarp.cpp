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
int* path;
clock_t start, end;
double cpu_time;

ofstream myfile;
string outFileName = "test.csv";
string finalOutFileName = "test_out_name.csv";
string daneWejsciowe = "dane_wejsciowe.ini";

string genName = "testy.txt";

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

void deleteMatrix(){
	for (int i = 0; i < cnt; i++){
		delete[] tab[i];
	}
	delete[] tab;
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

void generateMatrix(int size){
	deleteMatrix();
	createMatrix(size);
	for(int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			tab[i][j] = rand()%500;
		}
	}
}

void display(){
	cout << "Reprezentacja Macierzowa:\n";
	for (int j = 0; j < cnt; j++){
		for(int i = 0; i < cnt; i++){
			cout << tab[j][i] << " ";
		}
		cout << "\n";
	}
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


int TSP_HP(){  //metoda na podstawie pseudokodu z wikipedii https://en.wikipedia.org/wiki/Held%E2%80%93Karp_algorithm
	
	//Utwórz zbiór mo¿liwych stanów C
	int** C;
	int n = cnt;
	
	C = new int*[1<<(n-1)];
	for (int i = 0; i < 1<<(n-1); i++){
		C[i] = new int[n];
		for(int j = 0; j < n; j++){
			C[i][j] = INT_MAX;
		}
	}
	
	//wylosuj kolejny zbiór wierzcho³ków grafu S
	for (int S = 1; S < 1<<(n-1); S++){
		//Wylosuj kolejny wierzcho³ek k
		for(int k = 0; k < n-1; k++){
			//Czy k nale¿y do zbioru?
			if(S & 1 << k){
				//czy k jest jedynym wierzcho³kiem zbioru?
				if(S == 1 << k){
					//C({k}, k) := Graph[n-1][k]
					C[S][k] = tab[n-1][k];
				}else{
					//C(S, k) := min((S\{k}, m) + Graph[m][k])
					int S_bez_k = S - (1<<k);
					for(int m = 0; m < n-1; m++){
						if(S_bez_k & 1 << m){
								C[S][k] = min(
									C[S][k],
									C[S_bez_k][m] + tab[m][k]
								);
						}
						
					}
				}
			}
		}
	}
	
	//opt = min(C((0, ..., n-2), k) + Graph[k][n-1])
	int opt = INT_MAX;
	for(int k = 0; k < n-1; k++){
		opt = min(
			opt,
			C[(1 << (n-1)) -1 ][k] + tab[k][n-1]
		);
	}
	
	//Metoda otrzymywania œcie¿ki
	path = new int[n+1];
	path[0] = n-1;
	path[n] = n-1;
	int iterator = n-1;
	
	int z = n-1;
	int S = (1<< n-1) - 1;
	int min_k, min_path;
	while(S != 0){
		min_k = 0;
		min_path = INT_MAX;
		for(int k = 0; k < n-1; k++){
			if(!(S & (1 << k))){
				continue;
			}
			if(min_path > (C[S][k] + tab[k][z])){
				min_path = (C[S][k] + tab[k][z]);
				min_k = k;
			}
		}
		path[iterator] = min_k;
		
		z = min_k;
		S = S - (1 << z);
		iterator--;
	}
	delete [] C;
	
	//Zwrócenie wyniku opt - optymalna œciezka
	return opt;
	
}
 

int main()
{
	
	//ofstream myfile;
	//string outFileName = "t1.csv";
    //char readData[100];
    //GetPrivateProfileString(NULL, NULL, NULL, readData, 100, "dane_wejsciowe.ini");
    
    
   // string test_graph = "bays29.txt";
    //loadFromFile(test_graph.c_str());

	
	 Matrix();

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
			loadFromFile(temp.c_str());
			inFileMain >> liczba_powtorzen;
			myfile << liczba_powtorzen << " ";
			for (int i = 0; i < cnt+2; i++){
				inFileMain >> temp_num;
				myfile << temp_num << " ";
			}
			myfile << "\n";
			for(int i = 0; i < liczba_powtorzen; i++){
			start = clock();
		
			TSP_HP();
		
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
