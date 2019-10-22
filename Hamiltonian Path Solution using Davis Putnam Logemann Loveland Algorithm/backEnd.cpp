#include <iostream>
#include <fstream> 
#include <sstream>	// parse tokens 
#include <cmath>	// std:: sqrt
#include <queue>

int main (){

	std::queue <int> trueAtoms; // queue to contain symbols that are only true 

	std::ifstream inputFile("outputDpll");
	int symbol = 1;
	if( inputFile.is_open()){

		std::string line;
		while ( getline (inputFile, line)) {
			// while reading file if 0 is reached signifies end of symbol assignment table 
  			if ( line == "0") {
  				break;
  			}
  			// if a symbol is true add on to queue
  			if ( line.back() == 'T'){
  				trueAtoms.push(symbol);
  			}
  			else if (line.back() == '!'){
  				std::cout << "No Solution!" << std::endl;
  				return 1;
  			}
  			symbol ++;
		}
		trueAtoms.push(-1);
	}
	else {
		std::cerr << "Failed to open file" << std:: endl;
		exit(1);
	}


	// number of symbols = number of atoms * number of atoms
	int NUM_OF_ATOMS = std:: sqrt(symbol);
	// using patterns of symbols sorting can be done with out a sorting algorithm
	char answer [NUM_OF_ATOMS];

	std::string line;
	int trueAtom = trueAtoms.front(); // front of the queue
	int lineNumber = 1; 
	while (getline (inputFile, line)){
		if( lineNumber == trueAtom){
			char vertex = 'A' + ((trueAtom-1) / NUM_OF_ATOMS);
			int index = (trueAtom -1) % NUM_OF_ATOMS;
			answer[index] = vertex;
			trueAtoms.pop();
			trueAtom = trueAtoms.front();
		}
		lineNumber++;
	}
	inputFile.close();

	// print answer 
	std::cout << "Solution Path: " ;
	for (int i = 0 ; i < NUM_OF_ATOMS; i++){
		if( i == NUM_OF_ATOMS -1) std::cout << answer[i] << std::endl;
		else std::cout << answer[i] << " -> ";
	}

	return 0;
}