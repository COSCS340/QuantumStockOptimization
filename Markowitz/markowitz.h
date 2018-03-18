#ifndef _MARKOWITZ_H_
#define _MARKOWITZ_H_

#include <vector>
#include "stock_data.h"

namespace markowitz {
	class quantum {
		private:
			stock_data *data;
	};

	class classical {
		public:
			classical(stock_data *, unsigned int);
			classical(stock_data *, unsigned int, float, float, float);
			void adjust_weights(float, float, float);
			void solve();
			float get_minimum() {return minimum;};
			std::vector<int> get_solution() {return solution;};

		private:
			void inc_bvec(std::vector <int> &);
			float compute(std::vector <int> &);

			std::vector <int> solution;
			float minimum;
			stock_data *data;
			float t1, t2, t3;
			unsigned int b;
			int n;
	};
};

#endif
