#pragma once

#include <vector>
#include "stock_data.h"

namespace markowitz {
	// TODO: Implement everything here
	class quantum {
		protected:
			std::vector <std::vector <float> > Qmatrix; // Matrix to hold QUBO data
			std::vector <std::vector <float> > Jmatrix; // Matrix to hold Ising couplings
			std::vector <float> Hvector; // Vector to hold Ising strengths
			unsigned int budget;
			unsigned int n;
			float theta[3];

		public:
			quantum(const stock_data& data, unsigned int budget, const float theta_in[3]);
	};

	// TODO: Find solution
	class classical {
		protected:
			std::vector <std::vector <float> > Qmatrix; // Matrix to hold QUBO data
			unsigned int budget;
			unsigned int n;
			float theta[3];
			
		public:
			classical(const stock_data& data, unsigned int budget, const float theta_in[3]);
	};
};
