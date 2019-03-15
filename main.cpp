#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Word{
	string name;
	mutable vector<int> fileNo; //numarul fisierului/fisierelor din care provine cuvantul 

	Word(string _name, vector<int> _fileNo) : name(_name), fileNo(_fileNo) {}

	bool operator <(const Word &wordObj) const{
		return name < wordObj.name;
	}
};

bool IsUnwantedCharacter(char c){
	if (isalpha(c) || (c == '-')){
		return false;
	}
	else{
		return true;
	}
}

int main(int argc, char* argv[]){
	int i;
	ifstream inputFile;
	string word, ignore ="'"; //pentru stocarea cuvintelor citite
	set<Word> words;
	set<Word>::iterator it;
	pair<set<Word>::iterator, bool> ret;

	//citirea articolelor din parametrii din linia de comanda
	for (i = 1; i < argc; i++){
		vector<int> aux;
		aux.push_back(i);

		inputFile.open(argv[i], ios::in);
		if (!inputFile.is_open()){
			perror("Error opening file!");
			return -1;
		}

		while (inputFile >> word){
			//verific daca apare constructia ignore, adica "'s"
			size_t found = word.find(ignore);
			if (found != string::npos){
				word.erase(word.begin() + found, word.end());
			}

			//sterg caracterele diferite de cele din alfabet sau '-' si apoi fac conversie tolower
			word.erase(remove_if(word.begin(), word.end(), &IsUnwantedCharacter), word.end());
			transform(word.begin(), word.end(), word.begin(), ::tolower);

			//cuvantul exista deja
			ret = words.insert(Word(word, aux));
			if (!ret.second){
				if (((Word)*(ret.first)).fileNo.back() != i){
					vector<int> helper;

					for (long unsigned int j = 0; j < ((Word)*(ret.first)).fileNo.size(); j++){
						helper.push_back(((Word)*(ret.first)).fileNo[j]);
					}
					helper.push_back(i);
					words.erase(ret.first);
					words.insert(Word(word, helper));
				}
				
			}
		}

		inputFile.close();
	}

	//citesc datele de la input
	while (cin >> word){
		int count = 0;
		if (word == "/exit"){
			return 0;
		}

		for (it = words.begin(); it != words.end() && count < 5; ++it){
			size_t found = ((Word)(*it)).name.find(word);

			if (found != string::npos){
				if (found == 0){
					cout << ((Word)(*it)).name << " : ";
					for (long unsigned int i = 0; i < ((Word)(*it)).fileNo.size(); i++){
						cout << ((Word)(*it)).fileNo[i] << " ";
					}

					cout << "\n";
					count ++;
				}

				else{
					string wordAux = "-" + word;
					found = ((Word)(*it)).name.find(wordAux);
					if (found != string::npos){
						cout << ((Word)(*it)).name << " : ";
						for (long unsigned int i = 0; i < ((Word)(*it)).fileNo.size(); i++){
							cout << ((Word)(*it)).fileNo[i] << " ";
						}

						cout << "\n";
						count ++;
					}
				}
			}
		}


		//daca nu a fost gasit nicaieri
		if (count == 0){
			cout << "No suggestions...\n";
		}
		cout << "\n";
	}

    return 0;
}