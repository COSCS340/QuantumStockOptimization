#pragma once

#include <vector>
#include "stock_data.h"

namespace markowitz {
	class quantum {
		private:
			stock_data data;
	};

	class classical {
		public:

			classical(stock_data, unsigned int);
			classical(stock_data, unsigned int, float *);

			void adjust_weights(float *);
			void solve(); // TODO: Solution always all 1's

			float get_minimum() {return minimum;};
			std::vector<int> get_solution() {return solution;};

		private:
			void inc_bvec(std::vector <int> &);
			float compute(std::vector <int> &);

			float minimum;
			std::vector <int> solution;

			stock_data data;
			float theta[3];
			unsigned int b, n;
	};
};
