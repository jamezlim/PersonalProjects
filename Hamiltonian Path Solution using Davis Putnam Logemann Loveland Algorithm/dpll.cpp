#include <iostream>
#include <fstream>
#include <map> // map used to contain assignment of each symbol  
#include <set> // set for fast checking pureLiterals
#include <string>
#include <vector>
#include <sstream> // string stream parsing
#include <algorithm> // std::count


void propagate (int l, std::map <int, std::vector < int>> &cnf,  std::map<int, std::string> &v){
	// if literal l is negative search v for -1 * l
	std::string assignedValue ;
	if( l < 0) assignedValue = v.find(-1*l) -> second;
	else assignedValue =  v.find(l) -> second;

	for ( auto clause : cnf){
		std::vector <int > newVec; 
		for( auto literal : clause.second){

			// atom l is in clause 
			if ( literal == -1 * l || literal == l ){
				if( (literal < 0 && assignedValue == "false") || (literal > 0 && assignedValue == "true")) {
					cnf.erase(clause.first);
				}	
			}
			// if not in clause push all remainders
			else newVec.push_back(literal);
		}
		// only link clause if key still exists 
		auto itr = cnf.find(clause.first); 
		if (itr != cnf.end()) {
			itr -> second = newVec;
		}
	}
}


void obviousAssign(int l, std::map<int, std::string> &v ){
	if( l < 0) v.find(-1* l) -> second = "false";
	else v.find(l) -> second = "true";
}


bool isPureLiteral(int l, std::map <int, std::vector < int>> cnf ){
	// if l comes across -1 * l then it is not a pure literal 
	for ( auto element : cnf){
		for ( auto literal : element.second){
			if ( literal * -1 == l) return false; 
		}
	}
	return true;
}

 
std::map<int, std::string> dpll(std::vector<int> atoms, std::map <int, std::vector < int>> cnf, std::map<int, std::string> v){
	// easy cases 
	//--------------------------------------------------------------------------------------------------------------------------
	bool easyCase = true;
	while ( easyCase){
		easyCase = false;
		
		// contain all pure literals of this iteration  
		std:: set <int>  pureLiterals;
		bool pureLiteralExists = false; 
		bool emptyClauseExists = false; 
		for ( auto clause : cnf){
			if ( (clause.second).empty()) emptyClauseExists = true;
			for (auto literal: clause.second){
				// if the pure literal set already contains the literal then skip for faster search 
				if( !pureLiterals.count(literal) &&  isPureLiteral(literal, cnf)){
					pureLiterals.insert(literal);
					pureLiteralExists = true;
				} 
			}
		}

		// all clauses are satisfied so assign any left over unbound as either true or false
		if ( cnf.empty()){
			for ( auto atoms : v){
				if ( atoms.second == "unbound") atoms.second = "true";
			}
			return v;
		}

		// if some clause in cnf is .empty() means unsatisfiable
		else if (emptyClauseExists){
			std::map <int, std::string> nilMap = {{-1,"NIL"}};
			return nilMap;
		} 
		
		else if (pureLiteralExists){
			// if pureliterals existed then while loop again 
			easyCase = true;
			// for all pure literals in the set 
			for ( auto eachPureLiteral : pureLiterals){
				obviousAssign(eachPureLiteral,v);

				// delete every clause that contains literal;
				// have to erase whole key --> vector pair 
				for ( auto clause : cnf){
					if( std::count ( clause.second.begin(),clause.second.end(),eachPureLiteral)){
						cnf.erase(clause.first);
					}
				}
			}
		}

		// if no other cases satisfy check for single literals 
		else {
			for ( auto clause : cnf){
				if (clause.second.size() == 1){
					//other single literals still exist so oop again 
					easyCase = true; 
					
					int singleLiteral = clause.second[0];
					obviousAssign(singleLiteral, v);
					propagate(singleLiteral, cnf, v);
					break;
				}
			}
		}
	}// while loop

	// hard cases 
	//--------------------------------------------------------------------------------------------------------------------------
	int pickedAtom;
	for ( auto atom : v){
		if(atom.second == "unbound") {
			pickedAtom = atom.first;
			break;
		}	
	}
	
	// branch to true
	v.find(pickedAtom) -> second = "true";
	std::map <int, std::vector < int>> cnfCopy = cnf;
	propagate(pickedAtom, cnfCopy, v);
	auto vNew = dpll(atoms,cnfCopy,v);
	// have an answer !!!
	if( vNew.find(-1) == vNew.end()) return vNew;

	// branch to false 
	else{
		v.find(pickedAtom) -> second = "false";
		propagate(pickedAtom, cnf, v);
		return dpll(atoms, cnf, v);
	}
}// end of dpll



int main (){
	//input 
	//-------------------------------------------------------------------
	// take input from the output file created on the front end and parse them into containers 
	// to pass into the dpll algorithm

	std::vector <int> atoms; // 1 ~ 64 
	std::map <int, std::vector < int>> cnf; // vec [1 2 3 4 5 6 7 8] <-- one formula 
	std::map <int, std::string > v; // {atom , true/false/unbound}
	std::vector <std::string> keepCopy; // 1 A 1, 2 A 2 .... 9 B 1, 10 B 2 .... 64 H 8 

	std::ifstream inputFile("outputFront");	
	if( inputFile.is_open()){

		// input cnf formulas into set cnf
		std::string line;
		int key = 0;
  		while ( getline (inputFile, line)) {
  			if ( line == "0") {
  				break;
  			}

  			std::vector < int > oneFormula; 
  			std:: istringstream iss(line);
  			int token;
			while (iss >> token){
				oneFormula.push_back(token);
    		}
  			cnf.insert({key,oneFormula});
  			key ++;
		}
	}
	else {
		std::cerr << "Failed to open file" << std:: endl;
		exit(1);
	}

	// copy from 1 A 1, 2 A 2 .... 9 B 1
	std::string line;
	int numOfSymbols = 0;
	while ( getline (inputFile, line)) {
		numOfSymbols ++;

		// initialize v all to unbound 
		v.insert({numOfSymbols,"unbound"});

		// set of propositional atoms 
		atoms.push_back(numOfSymbols);

		// keep copy for writing to file later on 
		keepCopy.push_back(line);
	}	

	// numOfSymbols 64 from this point 
	inputFile.close();

	// v contains answers to proposition 
	std::cout << "Creating file > outputDpll ..." << std::endl;
	v = dpll(atoms, cnf,v);

	//out put 
	//-------------------------------------------------------------------
	std:: ofstream outputFile("outputDpll");
	for ( auto element : v ){
		if( element.second == "true") outputFile << element.first << " T" << std::endl;
		else if( element.second == "false") outputFile << element.first << " F" << std::endl;
		else outputFile << "No Solution!" << std::endl;
	}	

	// border symbol to divide output of assignment and table for numeric representations of symbols 
	outputFile << 0 << std::endl;

	for( auto line : keepCopy){
		outputFile << line << std::endl;
	}

	outputFile.close();

	return 0;
}
