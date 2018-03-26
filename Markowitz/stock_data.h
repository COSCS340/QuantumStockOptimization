#pragma once

#include <vector>
#include <fstream>

struct stock_data {
	std::vector <float> prc;
	std::vector <float> ret;
	std::vector <std::vector<float> > cov; // Upper Triangular matrix
};

namespace csv_reader {
	// Should this return a struct instead of taking one?
	void get_stock_data (stock_data &, std::ifstream &, std::ifstream &, std::ifstream &, unsigned int);
	void read_csv_row (std::ifstream &, std::vector <float> &);
	void read_csv (std::ifstream &, std::vector <std::vector<float> > &);
};
