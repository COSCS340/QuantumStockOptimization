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

	stock_data my_data;

	csv_reader::get_stock_data (my_data, prc_file, ret_file, cov_file, n);
	markowitz::classical test(&my_data, 100, 0.5f, 0.0f, 0.5f);
	test.solve();

	return 0;
}
