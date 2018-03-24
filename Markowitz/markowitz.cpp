#include <vector>
#include <iostream>
#include <cmath>
#include "markowitz.h"

using namespace markowitz;

classical::classical(stock_data *input, unsigned int budget)
{
	data = input;
	n = input->prc.size();
	b = budget;
}

classical::classical(stock_data *input, unsigned int budget, float O1, float O2, float O3)
{
	data = input;
	n = input->prc.size();
	t1 = O1;
	t2 = O2;
	t3 = O3;
	b = budget;
}

void classical::adjust_weights(float O1, float O2, float O3)
{
	t1 = O1;
	t2 = O2;
	t3 = O3;
}

void classical::solve()
{
	std::vector<int> trial(n, 0);
	minimum = compute(trial);
	solution = trial;

	for (int i = 0; i < pow(2, n) - 1; i++){
		inc_bvec(trial);
		float result = compute(trial);
		if (result < minimum) {
			minimum = result;
			solution = trial;
		}
	}
}

float classical::compute(std::vector <int> &x)
{
	float returns = 0.0f;
	float penalty = 0.0f;
	float covariance = 0.0f;

	for (unsigned int i = 0u; i < n; i++) {
		returns += x[i]*data->ret[i];
	}

	float term1 = 0.0f;
	float term2 = 0.0f;
	for (unsigned int i = 0u; i < n; i++) {
		for (unsigned int j = 0u; j < n; j++) {
			term1 += x[i]*data->prc[i]*data->prc[j]*x[i];
		}
		term2 += x[i]*data->prc[i]*x[i]*b;
	}
	penalty = t1 - 2*t2 + b*b;

	for (unsigned int i = 0u; i < n; i++) {
		for (unsigned int j = 0u; j < n; j++) {
			covariance += x[i]*x[j]*data->cov[i][j];
		}
	}
	
	return t1*returns + t2*penalty + t3*covariance;
}

void classical::inc_bvec(std::vector <int> &vec)
{
	int c = 1;
	for (unsigned int i = 0U; i < vec.size(); i++) {
		int temp = vec[i];
		vec[i] = vec[i] ^ c;
		c = c & temp;
	}
}
