#include <vector>
#include <cmath>
#include "markowitz.h"

using namespace markowitz;

classical::classical(stock_data input, unsigned int budget)
{
	data = input;
	n = input.prc.size();
	b = budget;
}

classical::classical(stock_data input, unsigned int budget, float *th)
{
	data = input;
	n = input.prc.size();
	theta[0] = th[0];
	theta[1] = th[1];
	theta[2] = th[2];
	b = budget;
}

void classical::adjust_weights(float *th)
{
	theta[0] = th[0];
	theta[1] = th[1];
	theta[2] = th[2];
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
		returns -= x[i]*data.ret[i];
	}

	for (unsigned int i = 0u; i < n; i++) {
		penalty += x[i]*data.prc[i];
	}
	penalty = (penalty - b)*(penalty - b);

	for (unsigned int i = 0u; i < n; i++) {
		for (unsigned int j = 0u; j < n; j++) {
			covariance += x[i]*x[j]*data.cov[i][j];
		}
	}
	
	return theta[0]*returns + theta[1]*penalty + theta[2]*covariance;
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
