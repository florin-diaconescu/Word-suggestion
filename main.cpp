#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Word{
	string name;
	vector<int> fileNo; //numarul fisierului/fisierelor din care provine cuvantul 

	Word(string _name, vector<int> _fileNo) : name(_name), fileNo(_fileNo) {}
};

//cauta un cuvant ce trebuie inserat in lista de cuvinte deja existenta
int findWord(vector<Word> words, string word){
	int i;
	for (i = 0; i < words.size(); i++){
		if (words[i].name == word){
			return i;
		}
	}

	return -1;
}

bool IsUnwantedCharacter(char c){
	if (isalpha(c) || (c == '-')){
		return false;
	}
	else{
		return true;
	}
}

bool cmp_by_name(struct Word a, struct Word b){
	return (a.name < b.name);
}

int main(int argc, char* argv[]){
	int i, index;
	ifstream inputFile;
	string word, ignore ="'s"; //pentru stocarea cuvintelor citite
	vector<Word> words;

	//citirea articolelor din parametrii din linia de comanda
	for (i = 1; i < argc; i++){
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

			index = findWord(words, word);
			if(index != -1){
				if (words[index].fileNo.back() != i){
					words[index].fileNo.push_back(i);	
				}				
			}
			else{
				vector<int> aux;
				aux.push_back(i);
				words.push_back(Word(word, aux));
			}
		}

		inputFile.close();
	}

	//sortez vectorul de cuvinte lexicografic
	sort(words.begin(), words.end(), cmp_by_name);

	/*for (i = 0; i < words.size(); i++){
		cout << words[i].name << " ";
	}
	
	cout<< "\n\n\n";*/

	//citesc datele de la input
	while (cin >> word){
		int i, j, count = 0;
		if (word == "/exit"){
			return 0;
		}

		for (i = 0; i < words.size() && count < 5; i++){
			size_t found = words[i].name.find(word);

			//daca s-a gasit stringul
			if (found != string::npos){
				//daca s-a gasit incepand cu prima pozitie
				if (found == 0){
					cout << words[i].name << " : ";
					for (j = 0; j < words[i].fileNo.size(); j++){
						cout << words[i].fileNo[j] << " ";
					}

					cout << "\n";
					count++;
				}

				//altfel, incerc sa caut in cuvintele compuse, construind
				//un cuvant auxiliar
				else{
					string wordAux = "-" + word;
					found = words[i].name.find(wordAux);
					if (found != string::npos){
						cout << words[i].name << " : ";
						for (j = 0; j < words[i].fileNo.size(); j++){
							cout << words[i].fileNo[j] << " ";
						}

						cout << "\n";
						count++;
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