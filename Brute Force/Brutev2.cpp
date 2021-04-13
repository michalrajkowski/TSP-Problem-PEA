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

int TSP(){
	
	//algorym zaczyna sie zawsze w wierzcholku  0
	
	int ver[cnt-1];
	for(int i = 0; i < cnt - 1; i++){ //ZnajdŸ pierwszy cykl hamiltona
		ver[i] = i + 1;
	}
	path = new int[cnt + 1];
	path[0] = 0;
	path[cnt] = 0;
	
	
	int minimum_path = INT_MAX;   //ustaw d³ugoœæ najkrótszej œcie¿ki na nieskoñczonoœæ
	
	while(true){
		int current_path = 0;
		int current_node = 0;
		
		for(int i = 0; i < cnt - 1; i++){               //oblicz d³ugoœæ aktualej œcie¿ki dla badanego cyklu
			current_path += tab[current_node][ver[i]];
			current_node = ver[i];
		}
		current_path += tab[current_node][0];
		
		if(current_path < minimum_path){   //Czy aktualna œcie¿ka jest krótsza od najkrótszej œcie¿ki?
			minimum_path = current_path;   //Ustaw d³ugoœæ najkrótszej œcie¿ki na d³ugoœæ aktualnej œcie¿ki
			
			for(int i = 0; i < cnt-1; i++){ //zaktualizuj wierzcho³ki najkrótszej œcie¿ki
				path[i+1] = ver[i];
			}
		}
		if(!next_permutation(ver, ver + cnt-1)){ //czy istniej¹ jeszcze nierozpatrzone cykle hamiltona? + ZnajdŸ kolejny cykl hamiltona
			break;  
		}
	}

	
	return minimum_path; //Zwróæ wyniki dla najkrótszej œcie¿ki
	
}
 
// Driver Code
int main()
{
	//ofstream myfile;
	//string outFileName = "t1.csv";
    //char readData[100];
    //GetPrivateProfileString(NULL, NULL, NULL, readData, 100, "dane_wejsciowe.ini");
    
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
		
			TSP();
		
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
