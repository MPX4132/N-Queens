//
//  main.cpp
//  N-Queens
//
//  Created by Matias Barcenas on 3/16/16.
//  Copyright © 2016 Matias Barcenas. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "Evolution.hpp"
#include <iomanip>


int main(int argc, const char * argv[]) {

	// Get command line arguments.
	const int geneLength = (argc > 1)? std::atoi(argv[1]) : 10;
	const int roundCount = (argc > 2)? std::atoi(argv[2]) : 25;
	const std::string fName = (argc > 3)? argv[3] : "results.txt";


	// Attempt opening a file in the local directory.
	std::fstream output(fName, std::fstream::out | std::fstream::app);
	if (!output.is_open()) std::cout << "Error attempting to open output file..." << std::endl;


	// Attempt resolving each round.
	for (int i = 0; i < roundCount; i++) {
		Evolution nQueens(geneLength);

		std::clock_t sTime = clock();
		nQueens.resolve();
		std::clock_t eTime = clock();

		// Output the time and result to standard output.
		std::cout << "[" << std::setw(6) << std::setprecision(2) << (static_cast<float>(eTime-sTime) / CLOCKS_PER_SEC) << "s] ";
		std::cout << nQueens << std::endl;

		// Output the result to the file.
		output << nQueens << std::endl;
	}

	output << std::endl;
	output.close();

    return 0;
}
