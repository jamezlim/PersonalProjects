    
#include <string> 
#include <utility> // pair
#include <iostream>
#include <unistd.h>
#include <getopt.h> // arguments
#include <thread>
#include <vector>
#include <random>
#include <iomanip> // double precision
#include <cmath> // absolute value 
#include "threadSafeListenerQueue.h"

// variables for resetting coefficients when stuck
int iterationCount = 0; 
int TOLERANCE = 600000;

// function to compute fitness 
std::pair < std::vector <double>, double> fitness (std::vector < std::pair < double, double > > points , std::vector <double> coeff , int DEGREE){
	double distance = 0.0;

	// outer loop for # of points 
	for ( int i = 0 ; i < DEGREE+1; i ++){
		
		// inner loop for # of coefficients 
		double fx = 0.0;
		double power = 1.0;
		for ( int j = 0 ; j < DEGREE+1; j ++){
			 fx += coeff[j] * power;
			 power *= points[i].first; 
		}
		//std:: cout << "fx: " << fx << std::endl;
		//std:: cout << "points[i].second: " << points[i].second << std::endl;

		// sum total distance btwn given point y and f(x)
		double temp =  fabs(fx - points[i].second);
		distance += temp ;
	}

	//std:: cout << "distance: " << distance << std::endl;

	return std::make_pair(coeff, distance);
}


// function for each thread 
void threadFunction(ThreadSafeListenerQueue < std::pair < std::vector <double>, double> > & q1, 
					ThreadSafeListenerQueue < std::pair < std::vector <double>, double> > & q2, 
					std::vector < std::pair < double, double > > points, int DEGREE){
	std::pair < std::vector <double>, double> element;
	std::uniform_real_distribution<double> unif(-100  ,100);
   	std::default_random_engine re(time(0));

	while ( q1.listen(element)){
		if ( iterationCount > TOLERANCE + 5 + q1.length() ) iterationCount = 0;
		double range; 
		if( element.second == -1.0) break;
		

		// set random range for each distance offset 
		
		if ( element.second > 500 ) range = 0.8;
		else range = element.second * 0.005;
		// if for some iteration the fitness has not been updated generate bigger mutations to break out
		if ( iterationCount > TOLERANCE) {
			std::cout << "reached" << std::endl;
			range = 0.5;
		}	
		// generate new DEGREE+1 double coefficients and push to a vector 
		// std::vector coeff will contain at coeff[0] the least significant coefficient
		// ex) ax^2 + bx + c == > coeff = {c , b , a}
		std::vector < double > coeff;
		for (int i = 0 ; i < DEGREE+1 ; i ++){

			double randCoeff = element.first[i] + range * unif(re);
			coeff.push_back( randCoeff); 
		}

		// for (int i = 0 ; i < DEGREE+1 ; i ++){

		// 	double randCoeff = element.first[i] + dRate * unif(re);
		// 	coeff.push_back( randCoeff); 
		// }	


		
		q2.push(fitness(points, coeff, DEGREE));

		//std:: cout << "end of thread#" << iterationCount ++ << std::endl;
		iterationCount ++;
	}
	
}


int main (int argc,char ** argv){

	int option = -1;
	int NUM_OF_THREADS = 0;
	int DEGREE = 0; 
	double EPSILON = 1.0;


	// random DEGREE + 1 (x,y) coordinates 
	std::vector < std::pair < double, double > > points;

	// final answer pair containing vector of coefficients and the sumed distance 
	std::pair < std::vector <double>, double> answer;
	
	// getopt
	while ( (option = getopt( argc, argv, ":n:d:")) != -1 ){
		switch (option )
		{
			case 'n' : NUM_OF_THREADS = std::stoi(optarg);
				break;
			case 'd' : DEGREE = std::stoi(optarg);
				break;	
			case '?' : 
				std::cout << "Unrecognized option !" << std ::endl;
				break;	
		}
	}

	std:: cout << "Number of Threads: "<< NUM_OF_THREADS << std::endl;
	std:: cout << "Finding best fit for degree "<< DEGREE << " polynomial..." << std::endl;
	std:: cout << "------------------------------------------"<< std::endl;

	// generate random DEGREE+1 points in range (-100, 100)
	// push coordinates onto points vector

	std::uniform_real_distribution<double> unif(-5.0,5.0);
   	std::default_random_engine re(time(0));
	for (int i = 0 ; i < DEGREE + 1; i ++ ){
		double x = unif(re);
		double y = unif(re);
		points.push_back( std::make_pair(x,y));
	}

	// initialize queues
	ThreadSafeListenerQueue < std::pair < std::vector <double>, double> > q1;
	ThreadSafeListenerQueue < std::pair < std::vector <double>, double> > q2;

	// generate DEGREE+1 double coefficients and push to a vector 
	// std::vector coeff will contain at coeff[0] the least significant coefficient
	// ex) ax^2 + bx + c == > coeff = {c , b , a}
	std::vector < double > coeff;
	std::uniform_real_distribution<double> uni(-20,20);
	for (int i = 0 ; i < DEGREE+1 ; i ++){
		double randCoeff = uni(re) ;
		coeff.push_back( randCoeff); 
	}

	// constantly compare results of threads with answer 
	answer = fitness(points,coeff, DEGREE);
	for (int i = 0 ; i < NUM_OF_THREADS ; i ++){
		q1.push( answer);
	}


	// create thread pool
	std:: thread threadArray[NUM_OF_THREADS];

	// start threads 
	for (int i=0; i < NUM_OF_THREADS ; i ++ ){
		threadArray[i] = std::thread (threadFunction, std::ref(q1), std::ref(q2),points,DEGREE);
	}

	// repeat while distance is greater than EPSILON 
	while ( answer.second > EPSILON ){


		std::pair < std::vector <double>, double> element;
		// q1 continas tasks waiting 
		// q2 contains results from completed threads 
		q2.listen(element);
		
		if ( element.second < answer.second){
			answer = element;
			iterationCount = 0;
			std:: cout  << answer.second << std::endl;
		}

		// if it is stuck at some fitness push modified coefficients into q1 and start mutaiton from there
		if ( iterationCount >= TOLERANCE && iterationCount < TOLERANCE + 5+ NUM_OF_THREADS ) {
			q1.push( element);
			answer = element;
		}	
		else q1.push(answer);

	}

	for (int i=0; i < NUM_OF_THREADS ; i ++ ){
		q1.push( std:: make_pair(coeff, -1.0));
	}


	// join threads 	
	for (int i=0; i < NUM_OF_THREADS ; i ++ ){
		//std:: cout << " joined # of threads: " << i  << std::endl;
		threadArray[i].join();
	}

	std::cout << "\nBest fit coefficients for points \n";
	for (int i = 0; i < DEGREE+1 ; i ++){
		if ( i == DEGREE) std::cout << std::setprecision (5) << "( " << points[i].first << "," << points[i].second << " ) are" << std::endl;
		else std::cout << "( " << std::setprecision (5) << points[i].first << "," << points[i].second << " ), ";
	}

	//std:: cout << "before joining threads " << std::endl;
	// print coefficients
	for (int i = DEGREE; i >= 0; i --){
		std::cout <<std::setprecision (14) << answer.first[i] << " ";
	}
	std::cout << "\n" ;


	// program termination
	std:: cout << "------------------------------------------" << std::endl;
	std:: cout << "Terminating Program..." << std::endl;

	return 0;
}