#include <iostream>
#include <fstream> // read and write from and to file
#include <cstring> // strtok
#include <map>
#include <set>

int main (){

	//input
	//-------------------------------------------------------------------

	std::multimap <int,int > edges;  

	std::ifstream inputFile("inputFront");
	int NUM_OF_VERTICIES;
	std::string s;
	if( inputFile.is_open()){

		std::getline(inputFile,s);
		NUM_OF_VERTICIES = std:: stoi (s);
		std:: cout << "Number of vertices: " << NUM_OF_VERTICIES << std:: endl;
		std:: cout << "----------------------------------" << std:: endl;
		std:: cout << "Creating file > outputFront ..." << std:: endl;

		//convert alphabetical symbols into numbers that represent the key to distinguish which alphabet it maps to 
		//ex) 
		//8
		//A F --> 0 5
		//B A --> 1 0
		//B C --> 1 2
		//B E --> 1 4
	    // ....
		//H G --> 7 6

		// A B C D E F G H
		// 0 1 2 3 4 5 6 7
		std::string nextToken;
  		while (inputFile >> nextToken) {
	    	int key = nextToken[0] - 65;
	    	inputFile >> nextToken;
	    	edges.insert( {key, nextToken[0] - 65});
		}
	}
	else {
		std::cerr << "Failed to open file" << std:: endl;
		exit(1);
	}
	inputFile.close();	

	//----------------------------------------------------------------------

	// generate array of vertex+index symbols [A1,A2,A3 .... A8, B1, B2.... C1, C2 .... H8]
	// index of these arrays correspond to the numeric symbol 
	// ex) A 1, A 2, A 3... A 8, B 1 .....H 1, H 2, H 8
	// A1 = 1 B1 = 9 C1 = 17
	int size = NUM_OF_VERTICIES * NUM_OF_VERTICIES;
	std::string vertexIndexArray [size];
	char c = 65; // 'A'
	char index = 49; //'1'
	for (int i = 0 ; i < size; i++ ){
		std::string newString = "";
		newString += c;
		newString += " ";
		newString += index;
		vertexIndexArray[i] = newString;
		
		index++;
		if ( (index - 48) == NUM_OF_VERTICIES +1 ){
			index = 49;
			c++;
		}
	}

	//----------------------------------------------------------------------

	// generate constraints for Hamiltonian path problem 
	// [A1,A2,A3 .... A8, B1, B2.... ]
	// A = 0 
	// using the key 0 
	// vertexIndexArray[0 * NUM_OF_VERTICIES + index] is A1, A2, ... A8 
	// A1 is represented as 1 which is 0 * NUM_OF_VERTICIES + index + 1

	std:: ofstream outputFile("outputFront");

	// constraint #1 
	for ( int i = 0; i < NUM_OF_VERTICIES; i++){
		std::string line = "";
		for ( int j = 0; j < NUM_OF_VERTICIES; j++){
			int symbol = i * NUM_OF_VERTICIES + j +1;
			line += std::to_string(symbol);
			if ( j != NUM_OF_VERTICIES -1) line += " ";
		}
		outputFile << line << std::endl;
	}

	// constraint #2 
	int key = 0;
	for ( int i = 0; i < size; i++){
		key = i / NUM_OF_VERTICIES;
		for ( int j = 1 ; j < NUM_OF_VERTICIES - key; j++){
			std::string line = "";
			line += std::to_string(-1* (i + 1));
			line += " ";
			line += std::to_string(-1* (i+1 + j* NUM_OF_VERTICIES) );
			outputFile << line << std::endl; 
		}
	}

	// constraint #3
	for( int i = 0 ; i < NUM_OF_VERTICIES; i ++){
		auto range = edges.equal_range(i);
		std::set<int> connectedVerticesSet;
		for ( auto itr = range.first; itr != range.second; ++itr){
			connectedVerticesSet.insert(itr -> second);
		}

		// connectedVertices contains verticies that have an edge with i
		// 1 = [ 0, 2, 4] so process for 3,5,6,7,8
		for ( int j = 0; j < NUM_OF_VERTICIES; j++){
			// if does not include and not itself 
			if( connectedVerticesSet.count(j) == 0 && i != j ){
				// -A1 v -C2 , -A2 v -C3 .... -A7 v -C8
				// loop for 7 times not 8 
				for( int k = 0; k < NUM_OF_VERTICIES -1; k++){
					std::string line = "";
					line += std::to_string( -1 * (i * NUM_OF_VERTICIES + k + 1));
					line += " ";
					line += std::to_string( -1 * (j * NUM_OF_VERTICIES + k + 2));
					outputFile << line << std::endl;
				}
			}
			// else means there is an edge so continue with loop 
		} 
	}

	// extra data transfered to backend
	// 1 A 1
	// 2 A 2 ...
	// 9 B 1 ... 
	outputFile << 0 << std::endl;
	for ( int i = 0; i < size; i ++){
		std::string line = "";
		line += std::to_string(i+1);
		line += " ";
		line += vertexIndexArray[i];
		outputFile << line << std::endl;
	}
	
	outputFile.close();
	return 0;
}