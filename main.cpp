#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Word{
	string name; //numele cuvantului
	vector<int> fileNo; //numarul fisierului/fisierelor din care provine cuvantul 

	Word(string _name, vector<int> _fileNo) : name(_name), fileNo(_fileNo) {}

	//vreau ca set-ul de cuvinte sa fie sortat lexicografic, dupa nume
	bool operator <(const Word &wordObj) const{
		return name < wordObj.name;
	}
};

//functie pentru eliminarea caracterelor nedoritate, adica orice nu face parte
//din alfabet sau nu e cratima
bool IsUnwantedCharacter(char c){
	if (isalpha(c) || (c == '-')){
		return false;
	}
	else{
		return true;
	}
}

//functie pentru afisarea unei sugestii
void printSuggestion(set<Word>::iterator it){
	long unsigned int i;

	cout << ((Word)(*it)).name << " : ";
	for (i = 0; i < ((Word)(*it)).fileNo.size(); i++){
		cout << ((Word)(*it)).fileNo[i] << " ";
	}

	cout << "\n";
}

int main(int argc, char* argv[]){
	int i;
	ifstream inputFile; //fisierul de intrare
	string word; //buffer pentru citirea cuvintelor
	set<Word> words; //set-ul in care vor fi stocate cuvintele
	set<Word>::iterator it; //pentru iterarea in setul de cuvinte
	pair<set<Word>::iterator, bool> ret; //auxiliar pentru memorarea rasupunsului inserarii

	//citirea articolelor din parametrii din linia de comanda
	for (i = 1; i < argc; i++){
		//un vector auxiliar, avand numarul fisierului in care este gasit cuvantul
		vector<int> aux;
		aux.push_back(i);

		long unsigned int j;

		inputFile.open(argv[i], ios::in);
		if (!inputFile.is_open()){
			perror("Error opening file!");
			return -1;
		}

		//cat timp sunt cuvinte de citit
		while (inputFile >> word){
			//verific daca apare ', pentru a nu numara si cuvinte in genul Kevin si Kevin's separat
			size_t found = word.find("'");
			if (found != string::npos){
				word.erase(word.begin() + found, word.end());
			}

			//sterg caracterele diferite de cele din alfabet sau '-' si apoi fac conversie tolower
			word.erase(remove_if(word.begin(), word.end(), &IsUnwantedCharacter), word.end());
			transform(word.begin(), word.end(), word.begin(), ::tolower);

			//incerc inserarea cuvantului
			ret = words.insert(Word(word, aux));
			//daca cuvantul exista deja
			if (!ret.second){
				if (((Word)*(ret.first)).fileNo.back() != i){
					vector<int> helper;

					//salvez in vectorul helper continutul anterior al vectorului fileNo, la care
					//adaug si numarul fisierului curent
					for (j = 0; j < ((Word)*(ret.first)).fileNo.size(); j++){
						helper.push_back(((Word)*(ret.first)).fileNo[j]);
					}
					helper.push_back(i);

					//sterg vechiul cuvant, pentru a-l insera pe cel la care s-au facut modificari
					words.erase(ret.first);
					words.insert(Word(word, helper));
				}
				
			}
		}

		inputFile.close();
	}

	//citesc datele de la input
	while (cin >> word){
		int count = 0; //numarul sugestiilor deja gasite

		if (word == "/exit"){
			return 0;
		}

		//ma asigur ca afisez maxim 5 sugestii
		for (it = words.begin(); it != words.end() && count < 5; ++it){
			//caut cuvantul de la stdin in cuvantul curent din set
			size_t found = ((Word)(*it)).name.find(word);

			//daca a fost gasit undeva
			if (found != string::npos){
				//daca a fost gasit incepand cu prima pozitie
				if (found == 0){
					printSuggestion(it);
					count++;
				}

				else{
					string wordAux = "-" + word;
					found = ((Word)(*it)).name.find(wordAux);
					//daca a fost gasita constructia "-" + cuvant
					if (found != string::npos){
						printSuggestion(it);
						count++;
					}
				}
			}
		}


		//daca nu a fost gasit nicio sugestie
		if (count == 0){
			cout << "No suggestions...\n";
		}
		cout << "\n";
	}

    return 0;
}