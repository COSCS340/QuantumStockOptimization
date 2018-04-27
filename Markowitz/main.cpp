#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>

#include "markowitz.h"
#include "stock_data.h"

#include "XACC.hpp"
#include "HUBO.hpp"
#include <curl/curl.h>

size_t write_datas(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main(int argc, char **argv) {
	std::ifstream tickerfile;
	std::string tfilename;

	float theta[3] = {.5, .25, .25};
	float budget;
	unsigned int n;
	unsigned int count;
	bool brute = false;

	// Indicate to XACC that we want to take an
	xacc::addCommandLineOption("b",	 "The budget available to the portfolio (debt)");
	xacc::addCommandLineOption("c",	 "The maximum number of one stock to choose");
	xacc::addCommandLineOption("n",	 "The number of the stocks to choose between"); 
	xacc::addCommandLineOption("s",	 "The file containing ticker symbols values"); 
	xacc::addCommandLineOption("t1", "Theta_1 non-negative valued weighted"); 
	xacc::addCommandLineOption("t2", "Theta_2 non-negative valued weighted"); 
	xacc::addCommandLineOption("t3", "Theta_3 non-negative valued weighted"); 
	xacc::addCommandLineOption("classical", "Use Classical brute-force method"); 
	xacc::Initialize(argc, argv);

	auto options = xacc::RuntimeOptions::instance();
	(*options)["compiler"] = "dwave-qmi";
	(*options)["dwave-embedding"] = "dw-sapi";
	(*options)["dwave-solver"] = "DW_2000Q_1";

	if (options->exists("n")) {
		n = std::stoi((*options)["n"]);
	}
	if (options->exists("s")) {
		tfilename = ((*options)["s"]);
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
		budget = std::stof((*options)["b"]);
	}
	if (options->exists("c")) {
		count = std::stoi((*options)["c"]);
	}
	if (options->exists("classical")) {
		brute = true;
	}

    stock_data data(false, false);

    data.print_stats();

	if (!brute) {
		markowitz::model markmodel(data, budget, theta);
		std::vector <int> portfolio = markmodel.portfolio(markowitz::quantum);

		std::ofstream outfile;
		outfile.open("portfolio.csv");
		for (unsigned int i = 0; i < portfolio.size(); i++) {
			std::cout << portfolio[i] << ", ";
			outfile << portfolio[i] << ", ";
		}
		std::cout << std::endl;
		outfile << std::endl;
	}
	else {
		markowitz::model markmodel(data, budget, theta);
		std::vector <int> portfolio = markmodel.portfolio(markowitz::classical);

		std::ofstream outfile;
		outfile.open("portfolio.csv");
		for (unsigned int i = 0; i < portfolio.size(); i++) {
			std::cout << portfolio[i] << ", ";
			outfile << portfolio[i] << ", ";
		}
		std::cout << std::endl;
		outfile << std::endl;
	}

	xacc::Finalize();

	return EXIT_SUCCESS;
}
