#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>

#include "stock_data.h"
#include "markowitz.h"

#include "XACC.hpp"
#include "HUBO.hpp"

int main(int argc, char **argv) {
	std::ifstream prc_file;
	std::ifstream ret_file;
	std::ifstream cov_file;
	
	prc_file.open(argv[1]);
	ret_file.open(argv[2]);
	cov_file.open(argv[3]);
	float theta[3] = {1, 1, 1};
	int budget;
	int n;

	// Indicate to XACC that we want to take an
	xacc::addCommandLineOption("b",	 "The budget available to the portfolio (debt)");
	xacc::addCommandLineOption("n",	 "The number of the stocks to choose between"); 
	xacc::addCommandLineOption("t1", "Theta_1 non-negative valued weighted"); 
	xacc::addCommandLineOption("t2", "Theta_2 non-negative valued weighted"); 
	xacc::addCommandLineOption("t3", "Theta_3 non-negative valued weighted"); 
	xacc::Initialize(argc, argv);

	auto options = xacc::RuntimeOptions::instance();
	(*options)["compiler"] = "dwave-qmi";
	(*options)["dwave-embedding"] = "dw-sapi";
	(*options)["dwave-solver"] = "DW_2000Q_1";

	if (options->exists("n")) {
		n = std::stoi((*options)["n"]);
	}
	if (options->exists("t1")) {
		theta[0] = std::stof((*options)["t1"]);
	}
	if (options->exists("t2")) {
		theta[1] = std::stof((*options)["t2"]);
	}
	if (options->exists("t3")) {
		theta[2] = std::stof((*options)["t3"]);
	}
	if (options->exists("b")) {
		budget = std::stoi((*options)["b"]);
	}

	stock_data my_data = csv_reader::get_stock_data (prc_file, ret_file, cov_file, n);

	markowitz::model classic(my_data, budget, theta);

	std::vector <int> soln = classic.portfolio(markowitz::classical);

	for (int i = 0; i < soln.size(); i++) {
		printf ("%d", soln[i]);
	}
	printf ("\n");

	std::vector <int> qsoln = classic.portfolio(markowitz::quantum);

	for (int i = 0; i < qsoln.size(); i++) {
		printf ("%d", qsoln[i]);
	}
	printf ("\n");

//	markowitz::quantum qclassic(my_data, 100, theta);

	xacc::Finalize();
	return 0;
}
