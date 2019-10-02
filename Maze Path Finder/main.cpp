#include <iostream>
#include <getopt.h> // arguments
#include <vector>
#include <ctime> // srand(time(0))
#include <cmath> // abs
#include <mutex>
#include <cstdlib> //std::abort()
#include <thread>

#include "maze.hpp"
#include "threadSafeKVStore.h"
#include "threadSafeListenerQueue.h"

// mutex for incrementing futility counter 
std::mutex mtx;
unsigned long long futilityCounter = 0;


// size of maze is getFinish() + 2
int fitness( Maze maze,std::vector<int> genome, int GLENGTH){
	int bump = 0; 
	// start from (x1,y1) and work towards (x2,y2)
	int x1 = maze.getStart().row;
	int y1 = maze.getStart().col;
	int x2 = maze.getFinish().row;
	int y2 = maze.getFinish().col;
	int direction;
	for (int i = 0; i < GLENGTH; i++){
		direction = genome[i];

		// up 1 
		if ( direction == 1 ){
			//if x1 -1 < 0 is out of bounds || is a wall then increment bump and stay
			if( x1 < 1 || maze.get(x1-1,y1) ) bump ++;
			else x1 --;
		}

		// down 2
		else if ( direction == 2){
			// if x1 +1 > x2 + 1 is out of bounds || is a wall then increment bump and stay 
			if ( x1 > x2 || maze.get(x1+1,y1) ) bump ++;
			else x1 ++;
		}

		// left 3
		else if( direction == 3){
			if ( y1 < 1 || maze.get(x1,y1-1)) bump++;
			else y1 --;
		}
		// right 4
		else if (direction == 4){
			if ( y1 > y2 || maze.get(x1,y1+1)) bump ++;
			else y1 ++;
		}
		// else direction is 0 and do nothing
	}
	// finished moving coordinates following the genome 
	return 2* ( abs(x2-x1) + abs(y2-y1)) + bump;
}


void mixer(ThreadSafeKVStore <int, std::vector<int>> multimap, 
		   ThreadSafeListenerQueue <std::vector <int>> &offspringQ, int GLENGTH,int NUM_OF_THREADS, int THRESHOLD){
	srand(time(0));
	while (futilityCounter < THRESHOLD){
		
		int size = multimap.size();
		// two random numbers to pick from multimap 
		// second random is dependent on random1 to avoid mixing same genome 
		int random1 = rand() % size;
		int random2 = rand() % size;
		if( random1 == random2) random2 = (random1 + size -1) % size;

		std::vector <int> mix1 = multimap[random1].second;
		std::vector <int> mix2 = multimap[random2].second;

		// random splitpoint index zero is okay but index GLENTH - 1 
		// is not suitable as it would just be first mix vector
		int splitPoint = rand() % GLENGTH-1;
		std::vector <int> mixedGenome;

		// create new mixedGenome
		for (int i = 0 ; i < GLENGTH; i++){
			if ( i <= splitPoint) mixedGenome.push_back(mix1[i]);
			else mixedGenome.push_back(mix2[i]);
		}

		offspringQ.push( mixedGenome);
	}



	// possibility that some mutator threads are listening on offspringQ 
	for (int i = 0; i < NUM_OF_THREADS; i ++){
		std:: vector<int> mixedGenome;
		mixedGenome.push_back(-1);
		offspringQ.push(mixedGenome);
	}

	//std::cout << "end of mixer thread" << std::endl;
	
}


void mutator( Maze maze, ThreadSafeKVStore <int, std::vector<int>> &multimap,
			  ThreadSafeListenerQueue <std::vector <int>> &offspringQ, 
			  int NUM_OF_THREADS, int THRESHOLD){
	std:: vector<int> poppedGenome;
	srand(time(0));
	while( offspringQ.listen(poppedGenome)){
		
		// if element of offspringQ is [-1,-1,-1....] then terminate mutator thread
		if ( poppedGenome[0] == -1) break;

		// copy best fitness value 
		int oldBestFitness = multimap[0].first;

		int GLENGTH = poppedGenome.size();

		int random = rand() %10;
		// 40% possibility for mutation and increment futilityCounter if mutated
		if ( random < 4){
			// pick a random element in the genome to mutate 
			int randomIndex = rand() % GLENGTH;
			int newRandomElement = rand() % 5;
			poppedGenome[randomIndex] = newRandomElement;
		}

		//calculate fitness and insert to multimap and truncate to 4* NUM_OF_THREADS
		int poppedGenomeFitness = fitness(maze,poppedGenome,GLENGTH);
		multimap.insert(poppedGenomeFitness,poppedGenome);
		//std::cout << multimap.size() << std::endl;
		multimap.truncate(4*NUM_OF_THREADS);
		

		//compare new best fitness to old bestFitness
		int newBestFitness = multimap[0].first;
		if( newBestFitness < oldBestFitness){
			// thread-safely reset futilityCounter to zero
			mtx.lock();
			futilityCounter = 0;
			mtx.unlock();
		}
		else {
			// thread-safely increment futilityCounter 
			mtx.lock();
			futilityCounter ++;

			if ( futilityCounter == THRESHOLD){
				// print maze
				std::cout << maze << std::endl;

				// print genome
				std::cout << "Current Best Genome: [";
				for (int i =0; i <GLENGTH; i++){
					std::cout << multimap[0].second[i] << " ";
				}
				std::cout << "]" << std::endl;

				//print fitness
				std::cout << "Current Best Fitness: " << multimap[0].first << std::endl;

				mtx.unlock();
				break;
			}

			else if( futilityCounter > THRESHOLD) {
				mtx.unlock();
				break;
			}
			mtx.unlock();	
		}	
	}

	//std::cout << "end of mutator thread" << std::endl;
}

int main (int argc,char ** argv){
	int option = -1;
	int NUM_OF_THREADS = 2;
	int GLENGTH = 7;
	int THRESHOLD = 100;
	int ROWS = 8;
	int COLUMNS = 8;

	// getopt
	while ( (option = getopt( argc, argv, ":n:g:r:c:l:")) != -1 ){
		switch (option )
		{
			case 'n' : NUM_OF_THREADS = std::stoi(optarg);
				break;
			case 'g' : THRESHOLD = std::stoi(optarg);
				break;
			case 'r' : ROWS= std::stoi(optarg);
				break;
			case 'c' : COLUMNS = std::stoi(optarg);
				break;	
			case 'l' : GLENGTH = std::stoi(optarg);
				break;						
			case '?' : 
				std::cout << "Unrecognized option !" << std ::endl;
				break;	
		}
	}
	std:: cout << "Number of Threads: "<< NUM_OF_THREADS << std::endl;
	
	std:: cout << "Finding best route for maze... "<< std::endl;
	std:: cout << "-------------------------------"<< std::endl;

	
	// initialize offspring queue and threadSafeKVStore multimap
	ThreadSafeListenerQueue <std::vector <int>> offspringQ;
	ThreadSafeKVStore <int, std::vector<int>> multimap;

	srand(time(0));
	Maze maze(ROWS,COLUMNS);

	// create genomes of GLENGTH ( 4 * NUM_OF_THREADS) 
	int random;
	for ( int i = 0 ; i < 4 * NUM_OF_THREADS; i ++){
		std::vector <int>  genome;
		for (int j = 0; j < GLENGTH ; j ++){
			random = rand() %5;
			genome.push_back(random);
		}
		// calculate fitness with one genome and add to multimap
		int key = fitness(maze,genome,GLENGTH);
		multimap.insert(key,genome);
	}

	// create thread pool
	std:: thread threadArray[NUM_OF_THREADS];

	// start threads 
	// ideal to start mixer threads first because initialized as empty 
	// while the multimap has 4*NUM_OF_THREADS elements 
	// half of the threads will be mixer threads 
	const int BREAK_POINT = 2;
	int temp =NUM_OF_THREADS -  NUM_OF_THREADS/BREAK_POINT;
	for (int i=0; i < NUM_OF_THREADS ; i ++ ){
		if( i < temp ){
			threadArray[i] = std::thread(mixer, multimap,std::ref(offspringQ),GLENGTH,NUM_OF_THREADS, THRESHOLD);	
		}
		else{
			threadArray[i] = std::thread(mutator, maze, std::ref(multimap), std::ref(offspringQ), NUM_OF_THREADS, THRESHOLD);
		}
	}

	//join threads
	for (int i = 0; i < NUM_OF_THREADS; i ++){
		threadArray[i].join();
		//std::cout << "joined" << std::endl;
	}
}	