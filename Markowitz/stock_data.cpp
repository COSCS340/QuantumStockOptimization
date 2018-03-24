#include "stock_data.h"
#include <vector>
#include <fstream>
#include <sstream>

#include <iostream>


void csv_reader::get_stock_data (stock_data &data,
                                 std::ifstream &prc_file,
                                 std::ifstream &ret_file,
                                 std::ifstream &cov_file,
                                 unsigned int n)
{
	data.prc.resize(n);
	data.ret.resize(n);

	data.cov.resize(n);
	for (unsigned int i = 0; i < n; i++) data.cov[i].resize(n);

	read_csv_row(prc_file, data.prc);
	read_csv_row(ret_file, data.ret);
	read_csv(cov_file, data.cov);
	return;
}

void csv_reader::read_csv_row (std::ifstream &infile, std::vector <float> &vec)
{
	std::string buffer;
	for (unsigned int i = 0; i < vec.size(); i++) {
		getline(infile, buffer, ',') ;
		std::istringstream ss(buffer);
		ss >> vec[i];
	}

	return;
}

void csv_reader::read_csv (std::ifstream &infile, std::vector <std::vector<float> > &vec)
{
	for (unsigned int i = 0; i < vec.size(); i++) {
		read_csv_row(infile, vec[i]);
	}
	return;
}
