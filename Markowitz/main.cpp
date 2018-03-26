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

	stock_data my_data;

	csv_reader::get_stock_data (my_data, prc_file, ret_file, cov_file, n);
	markowitz::classical test(my_data, 1000, theta);
	test.solve();
	std::vector <int> result= test.get_solution();

	std::cout << test.get_minimum();
	for (unsigned int i = 0; i < result.size(); i++)
		std::cout << result[i] << std::endl;


	return 0;
}
