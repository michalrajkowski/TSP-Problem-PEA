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
	
	
	double smiec, x, y;
	int something;
	float punkty[rozmiar][2];
	for(int i = 0; i < rozmiar; i++){
		inFile >> something >> x >> y;
		punkty[i][0] = x*100;
		punkty[i][1] = y*100;
	}
	
	
	for(int i = 0; i < rozmiar; i++){
		for(int j = i+1; j < rozmiar; j++){
			double distX = punkty[i][0] - punkty[j][0];
			if(distX < 0.0)
				distX*= -1.0;
			distX = distX * distX;
			
			double distY = punkty[i][1] - punkty[j][1];
			if(distY < 0.0){
				distY *= -1.0;
			}
			distY = distY*distY;
			smiec = distX + distY;
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


/////////////////////////
//Algorithm global space
//////////////////////////

int max_iterations = 50000;
int population_size = 100;
int elite_size = 10;
float mating_pool_percent = 0.5;
float mutation_chance = 0.05;
float recombination_probability = 1.0;

double bestFitness = 1;
int bestPathLenght = -1;
int* bestPath;

double* fitness;
int** pathList;
int** matingPool;
double* probability;

int pathCost(int* path){
	int cost = 0;
	for(int i = 0; i < cnt - 1; i++){
		cost+= tab[path[i]][path[i+1]];
	}
	cost+= tab[path[cnt-1]][path[0]];
	return cost;
}

int randomPath(int *path){
	vector<int> temp;
	for(int i = 0; i < cnt; i++){
		temp.push_back(path[i]);
	}
	random_shuffle( temp.begin(), temp.end());
	for(int i = 0; i < cnt; i++){
		path[i] = temp[i];
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

void calculateFitness(int index){
	int* current_path = pathList[index];
	int cost = pathCost(current_path);
	fitness[index] = 1/( (double) cost);
}


void GA_TSP(){
	
	int matingPool_size = mating_pool_percent*population_size;
	
	//tworzenie tabel
//	cout << "\nGA_TSP begin\n";
	bestFitness = 0.0;
	
	
	fitness = new double[population_size];
	probability = new double[population_size];
	bestPath = new int[cnt];
	
	pathList = new int*[population_size];
	for (int i = 0; i < population_size; i++){
		pathList[i] = new int[cnt];
		for(int j = 0; j < cnt; j++){
			pathList[i][j] = 0;
		}
	}
	
	matingPool = new int*[matingPool_size];
	for (int i = 0; i < matingPool_size; i++){
		matingPool[i] = new int[cnt];
		for(int j = 0; j < cnt; j++){
			matingPool[i][j] = 0;
		}
	}
	
	///

	//generowanie pierwszej populacji
	int test_path[cnt];
	for(int i = 0; i < cnt; i++){
		test_path[i] = i;
	}

	
	for(int i = 0; i < population_size; i++){
		randomPath(test_path);
		
		for(int j = 0; j < cnt; j++){
			pathList[i][j] = test_path[j];
		}
	}
	
	///
	
	///Pêtla g³ówna
	for(int current_iteration = 0; current_iteration < max_iterations; current_iteration++){
		
	
	
	
		//obliczanie fitness
		for(int i = 0; i < population_size; i++){
			calculateFitness(i);
			if(fitness[i] >= bestFitness){
				bestFitness = fitness[i];
				bestPathLenght = pathCost(pathList[i]);
				for(int j = 0; j < cnt; j++){
					bestPath[j] = pathList[i][j];
				}
			}
		}
		
		//Select mating pool
		//List sorted by fitness
		
		//elites
		vector<pair<double, int> > ranking(population_size);
		
		for(int i = 0; i < population_size; i++){
			ranking[i] = make_pair(fitness[i], i);
		}
		sort(ranking.begin(), ranking.end());
		
		for(int i = 0; i < elite_size; i++){
			for(int j = 0; j < cnt; j++){
				matingPool[matingPool_size - elite_size + i][j] = pathList[ranking[population_size - 1 - i].second][j];	
			}
		}
		
		
		
		//Get from the list based on fitness
		//Roulette Wheel
		double totalFitness = 0; 
		
		double averageOldFitness = 0;
		
		for(int i = 0; i < population_size; i++){
			totalFitness += fitness[i];
		}
		
		averageOldFitness = totalFitness/population_size;
		
		for(int i = 0; i < population_size; i++){
			probability[i] = fitness[i]/totalFitness;
		}
		
		for(int i = 0; i < matingPool_size - elite_size; i++){
			double rndNumber = rand() / (double) RAND_MAX;
			double offset = 0.0;
			int pick = 0; 
			
			for(int j = 0; j < population_size; j++){
				offset += probability[j];
				if(rndNumber < offset){
					pick = j;
					break;
				}
			}
			
			//insert picked
			
			for(int j = 0; j < cnt; j++){
				matingPool[i][j] = pathList[pick][j];
			}
		}
		
		
		/*
		for(int i = 0; i < matingPool_size; i++){
			for(int j = 0; j < cnt+1; j++){
				cout << matingPool[i][j];
			}
			cout << "\n";
		}
		*/
		
		//Create Children
		
		for(int current_child_number = 0; current_child_number < population_size; current_child_number++){
			
			//getParents
			int rng1 = rand() % matingPool_size;
			int rng2 = rand() % matingPool_size;
			
			int* parent1 = matingPool[rng1];
			int* parent2 = matingPool[rng2];
			
			int child[cnt];
			for(int i = 0; i < cnt; i++){
				child[i] = -1;
			}
			
			double recombination_rng = rand() / (double) RAND_MAX;
			if(recombination_rng < recombination_probability){
			
				//breed
				int geneA = rand() % (cnt);
				int geneB = rand() % (cnt);
				
				int startGene = min(geneA, geneB);
				int endGene = max(geneA, geneB);
				
				
				bool isInParent[cnt];
				for(int i = 0; i < cnt; i++){
					isInParent[i] = false;
				}
				
				for(int i = startGene; i <= endGene; i++){
					isInParent[parent1[i]] = true;
					child[i] = parent1[i];
				}
				
				int p = 0;
				for(int i = 0; i < cnt; i++){
					if(child[i] == -1){
						while(isInParent[parent2[p]]){
							p++;
						}
						isInParent[parent2[p]] = true;
						child[i] = parent2[p];
					}
				}
				
				for(int i = 0; i < cnt; i++){
					pathList[current_child_number][i] = child[i];
				}
			}else{
				for(int i = 0; i < cnt; i++){
					pathList[current_child_number][i] = parent1[i];
				}
			}
		}
		
		
		//Mutate
		double mutation_rng;
		for(int i = 0; i < population_size; i++){
			mutation_rng = rand() / (double) RAND_MAX;
			if(mutation_rng < mutation_chance){
				//mutate
				int geneA = rand() % (cnt);
				int geneB = rand() % (cnt);
				swap(pathList[i][geneA], pathList[i][geneB]);
			}
		}
		
		//Repeat
	}
	
	//cout << "Best Fitness: " << bestFitness << "\n";
	//cout << "Best path: " << bestPathLenght << "\n";
	
	//koniec dzia³ania algo i usuwanie tabel
	for (int i = 0; i < population_size; i++){
		delete[] pathList[i];
	}
	delete[] pathList;
	
	for (int i = 0; i < matingPool_size; i++){
		delete[] matingPool[i];
	}
	delete[] matingPool;
	
	delete[] fitness;
	delete[] probability;
//	cout << "\nGA_TSP end\n";
	
}
                         

int main()
{
	
	//ofstream myfile;
	//string outFileName = "t1.csv";
    //char readData[100];
    //GetPrivateProfileString(NULL, NULL, NULL, readData, 100, "dane_wejsciowe.ini");
    
    Matrix();	
    
    
    //loadFromFileGR("gr120.txt");
    //display();

    //GA_TSP();
    int maksymalne_iteracje[5] = {1000, 5000, 10000, 25000, 50000};
	int population_sizes[5] = {50, 100, 200, 500, 1000};
	float mutation_chances[6] = {0.0, 0.01, 0.02, 0.05, 0.1, 0.5};
	float recombination_probabilitys[5] = {1.0, 0.95, 0.9, 0.8, 0.5};
    
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
			}else if(typ_pliku == "gr"){
				loadFromFileGR(temp.c_str());
			}
			
			cout << temp << "\n";
			
			inFileMain >> liczba_powtorzen;
			myfile << liczba_powtorzen << " ";
			
			inFileMain >> optymalny_wynik;
			myfile << optymalny_wynik << " ";
			
			
			myfile << "\n";
			
		
			for(int z = 0; z < 1; z++){
				//max_iterations = maksymalne_iteracje[z];
				//population_size = population_sizes[z];
				//elite_size = population_size/10;
				//mutation_chance = mutation_chances[z];
			//	recombination_probability = recombination_probabilitys[z];
				//myfile << "Parametr: " << 
				//maksymalne_iteracje[z] << "\n";
			
			double srednia = 0;
				for(int i = 0; i < liczba_powtorzen; i++){
					cout << "Test: " << i << "\n";
					start = clock();
				
					GA_TSP();
				
					end = clock();
					cpu_time = ((double) (end - start)) / (CLOCKS_PER_SEC/1000);
					//myfile << cpu_time<<"\n";
					double czass = cpu_time;
					int wynik = bestPathLenght;
					float blad = bestPathLenght - optymalny_wynik;
					blad /= optymalny_wynik;
				//	myfile << blad << "\n";	
					myfile << wynik << " " << blad << " " << czass << "\n";
					
					srednia += wynik;	
					
				}
			}
			//srednia = srednia/10.0;
			//myfile << "œrednia:\n" << srednia << "\n";
		

		}
	}
	
    finalOutFileName = temp;
    
    inFileMain.close();
    myfile.close();
   
    remove(finalOutFileName.c_str());
    rename(outFileName.c_str(), finalOutFileName.c_str());
    
    
    
	return 0;
	
}
