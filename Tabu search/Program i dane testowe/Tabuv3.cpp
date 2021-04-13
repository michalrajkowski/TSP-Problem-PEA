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
#include <math.h>

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

int loadFromFileGR(string FileName){
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
		for(int j = 0; j < i + 1; j++){
			inFile >> w;
			tab[i][j] = w;
			tab[j][i] = w;
		}
	}
    
    inFile.close();
}

int loadFromFileKROA(string FileName){
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
	
	
	int smiec, x, y;
	int punkty[rozmiar][2];
	for(int i = 0; i < rozmiar; i++){
		inFile >> smiec >> x >> y;
		punkty[i][0] = x;
		punkty[i][1] = y;
	}
	
	
	for(int i = 0; i < rozmiar; i++){
		for(int j = i+1; j < rozmiar; j++){
			smiec = (punkty[i][0] - punkty[j][0])*(punkty[i][0] - punkty[j][0]) + 
			(punkty[j][1] - punkty[i][1])*(punkty[j][1] - punkty[i][1]);
			smiec = sqrt(smiec);
			tab[i][j] = smiec;
			tab[j][i] = smiec;
		}
	}
    
    inFile.close();
}

int loadFromFileKROAplus(string FileName){
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
	
	
	int smiec, x, y;
	int punkty[rozmiar][2];
	for(int i = 0; i < rozmiar; i++){
		inFile >> smiec >> x >> y;
		x *= 100;
		y *= 100;
		punkty[i][0] = x;
		punkty[i][1] = y;
	}
	
	
	for(int i = 0; i < rozmiar; i++){
		for(int j = i+1; j < rozmiar; j++){
			smiec = (punkty[i][0] - punkty[j][0])*(punkty[i][0] - punkty[j][0]) + 
			(punkty[j][1] - punkty[i][1])*(punkty[j][1] - punkty[i][1]);
			smiec = sqrt(smiec);
			tab[i][j] = smiec;
			tab[j][i] = smiec;
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

int* best;
int bestCost;
int* bestCandidate;
int* tabuList1;
int* tabuList2;
float tabu_size_percent = 1.0;

int max_iterations = 2500;

int reset_time = 500;

int pathCost(int* path){
	int cost = 0;
	for(int i = 0; i < cnt; i++){
		cost+= tab[path[i]][path[i+1]];
	}
	return cost;
}

int randomPath(int *path){
	vector<int> temp;
	for(int i = 1; i < cnt; i++){
		temp.push_back(path[i]);
	}
	random_shuffle( temp.begin(), temp.end());
	for(int i = 0; i < cnt-1; i++){
		path[i+1] = temp[i];
	}  
	
}

void greedy(int* path){
	bool visited[cnt];
	for(int i = 0; i < cnt; i++)
		visited[i] = false;
	
	int wierzcholek = 0;
	for(int c = 1; c < cnt; c++){
		int mini = INT_MAX;
		int mini_node;
		visited[wierzcholek] = true;
		for(int i = 0; i < cnt; i++){
			if(visited[i] == false){
				int droga = tab[wierzcholek][i];
				if(droga < mini){
					mini = droga;
					mini_node = i;
				}
			}
		}
		path[c] = mini_node;
		wierzcholek = mini_node;
	}
}


void TabuTSP(){
 
 //INITIALIZING
 //1sBest ‹ s0
 //2bestCandidate ‹ s0
 //3tabuList ‹ []
 //4tabuList.push(s0)
 best = new int[cnt + 1];
 bestCandidate = new int[cnt + 1];
 bestCost = INT_MAX;
 int bestCandidateCost = INT_MAX;
 int tabuSize = cnt * tabu_size_percent;
 tabuList1 = new int[tabuSize];
 tabuList2 = new int[tabuSize];
 
 int resetTimer = 0;
 
 //INITIAL SOLUTION
 
 //1. autism aproach
 for(int i = 0; i < cnt; i++){
 	bestCandidate[i] = i;	
 }
 bestCandidate[cnt] = 0;
 
 //2. Greedy aproach
 greedy(bestCandidate);
 
 //3. closestNeighbour
 //alternatywna opcja initial solution
 
 
 //NADPISANIE best I best_CANDIDATE
 bestCandidateCost = pathCost(bestCandidate);
 bestCost = bestCandidateCost; 
 
 
 for(int iteration = 0; iteration < max_iterations; iteration++){ //WARUNEK STOPU
 	
 	//CHECK NEIGHBORHOOD
 	
 	int a, b, newCost = 0; 
 	int newCostMin = INT_MAX;
 	bool checkTabu = false;
 	
 	for (int i = 1; i < cnt; i++){
 		for(int j = i + 1; j < cnt; j++){
 			//oblicz d³ugoœæ dla swapniêtych i oraz j
 			
 			/*
			 newCost = bestCandidateCost;
			 
			 */
			 swap(bestCandidate[i], bestCandidate[j]);
			 newCost = pathCost(bestCandidate);
			 swap(bestCandidate[i], bestCandidate[j]);
			 
 			
			 //
			 if(newCost < newCostMin){ //szukamy najlepszej zamiany
			 	
				 
				 if(newCost < bestCost){ //warunek obejœcia tabu - najlepszy wynik z napotkanych
			 		bestCost = newCost;
			 		newCostMin = newCost;
			 		a = i;
			 		b = j;
				 }else{
				 	for(int k = 0; k < tabuSize; k++){
				 		if((tabuList1[k] == i && tabuList2[k] == j )||(tabuList1[k] == j && tabuList2[k] == i)){
				 			checkTabu = true;
						 }
					 }
					 if(checkTabu == false){
					 	newCostMin = newCost;
					 	a = i;
			 			b = j;
					 }
				 }
				 checkTabu = false;
			 }
 			
		 }
	 }
	 
	 //a, b - wierzcholki do zamiany
	 //newCostMin - nowy koszt trasy.
	 
	 swap(bestCandidate[a], bestCandidate[b]);
	 tabuList1[iteration % tabuSize] = a;
	 tabuList2[iteration % tabuSize] = b;
	 
	 bestCandidateCost = newCostMin;
	 
	 if(bestCandidateCost < bestCost){
	 	bestCost = bestCandidateCost;
	 	for(int i = 0; i < cnt+1; i++){
	 		best[i] = bestCandidate[i];
		 }
	 }
	 
 		
 }
	delete[] bestCandidate;
	delete[] tabuList1;
	delete[] tabuList2;
 
 	return;
 
}
                         

int main()
{
	
	//ofstream myfile;
	//string outFileName = "t1.csv";
    //char readData[100];
    //GetPrivateProfileString(NULL, NULL, NULL, readData, 100, "dane_wejsciowe.ini");
    
    Matrix();
    float tabu_percent_list[8] = {0.3, 0.4, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0};
    int max_iterations_list[5] = {100, 500, 1000, 2500, 10000};
	
    
    /*
    loadFromFileKROA("kroA100.txt");
    //display();
    for(float f = 0.3; f < 0.5; f+=0.01){
    	tabu_size_percent = f;
    	TabuTSP();
    	cout << tabu_size_percent << ": ";
    	cout << bestCost << "\n";
	}
	*/
    
    
    ifstream inFileMain;
    string temp;
    int temp_num;
    string typ_pliku;
    int liczba_powtorzen;
    int optymalny_wynik;
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
			
			inFileMain >> typ_pliku;
			
			if(typ_pliku == "full"){
				loadFromFile(temp.c_str());
			}else if(typ_pliku == "half"){
				loadFromFileGR(temp.c_str());
			}else if(typ_pliku == "points"){
				loadFromFileKROA(temp.c_str());
			}
			
			inFileMain >> liczba_powtorzen;
			myfile << liczba_powtorzen << " ";
			
			inFileMain >> optymalny_wynik;
			myfile << optymalny_wynik << " ";
			
			
			myfile << "\n";
			
		
			
			double srednia = 0;
			for(int i = 0; i < liczba_powtorzen; i++){
				start = clock();
			
				TabuTSP();
			
				end = clock();
				cpu_time = ((double) (end - start)) / (CLOCKS_PER_SEC/1000);
				//myfile << cpu_time<<"\n";
				float czas = bestCost;
				//float blad = bestCost - optymalny_wynik;
				//blad /= optymalny_wynik;
			//	myfile << blad << "\n";	
				myfile << czas << "\n";
				srednia += czas;	
				
			}
			srednia = srednia/10.0;
			myfile << "œrednia:\n" << srednia << "\n";
		

		}
	}
	
    finalOutFileName = temp;
    
    inFileMain.close();
    myfile.close();
   
    remove(finalOutFileName.c_str());
    rename(outFileName.c_str(), finalOutFileName.c_str());
    
    

    
	return 0;
	
}
