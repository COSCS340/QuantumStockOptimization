#include <vector>
#include <cmath>
#include "markowitz.h"

#define DEBUG

#ifdef DEBUG
	#include <stdio.h>
#endif

using namespace markowitz;

/* Initialize the QUBO matrix for the markowitz model */
classical::classical(const stock_data& data, unsigned int budget, const float theta_in[3]) : budget(budget), n(data.prc.size())
{
	theta[0] = theta_in[0];
	theta[1] = theta_in[1];
	theta[2] = theta_in[2];

	Qmatrix.resize(n);
	for (int i = 0; i < n; i++) 
		Qmatrix[i].resize(n, 0.0f);

// TODO: Is this the correct QUBO matrix?
	for (int r = 0; r < n; r++) {
		for (int c = r + 1; c < n; c++) {
			Qmatrix[r][c] = theta[1]*data.cov[r][c] + 2*theta[2]*data.prc[r]*data.prc[c];
		}
	}

	for (int i = 0; i < n; i++) {
		Qmatrix[i][i] += -theta[0]*data.ret[i] + theta[2]*data.prc[i]*data.prc[i] - 2*theta[2]*data.prc[i]*budget + theta[1]*data.cov[i][i];
	}

#ifdef DEBUG
	for (int r = 0; r < n; r++) {
		printf ("[");
		for (int c = 0; c < n; c++) {
			printf ("%8.2f,", Qmatrix[r][c]);
		}
		printf ("]\n");
	}
#endif
}

quantum::quantum(const stock_data& data, unsigned int budget, const float theta_in[3]) : budget(budget), n(data.prc.size())
{
	theta[0] = theta_in[0];
	theta[1] = theta_in[1];
	theta[2] = theta_in[2];
}
