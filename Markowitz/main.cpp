#include <iostream>
#include <fstream>
#include <cstdlib>
#include "stock_data.h"
#include "markowitz.h"

int main(int argc, char **argv) {
	std::ifstream prc_file;
	std::ifstream ret_file;
	std::ifstream cov_file;
	
	prc_file.open(argv[1]);
	ret_file.open(argv[2]);
	cov_file.open(argv[3]);
	int n = atoi(argv[4]);
	float theta[3] = {.25, .25, .5};

	stock_data my_data = csv_reader::get_stock_data (prc_file, ret_file, cov_file, n);
	markowitz::classical model(my_data, 100, theta);
	markowitz::quantum qmodel(my_data, 100, theta);

	return 0;
}
