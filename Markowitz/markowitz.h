#pragma once

#include <vector>
#include <boost/dynamic_bitset.hpp>
#include "stock_data.h"
#include "DWGraph.hpp"

namespace markowitz {
	enum operation {quantum, classical};

	class Quantum {
		protected:
			std::vector <std::vector <float> > Qmatrix; // Matrix to hold QUBO data
			std::vector <std::vector <float> > Jmatrix; // Matrix to hold Ising couplings
			std::vector <float> Hvector; // Vector to hold Ising linear biases
			unsigned int budget; // Budget for the problem
			unsigned int n; // Number of stocks
			float theta[3]; // Theta superposition weights
			float cfactor;  // Constant factor from the QUBO
			float ifactor;  // Constant factor from the Ising

			void CvtQBUO2Ising();

		public:
			Quantum(const stock_data& data, unsigned int budget, const float theta_in[3]);
			std::vector<int> embedding(std::shared_ptr<xacc::AcceleratorBuffer> buffer); 
			int size() {return n;};

			std::shared_ptr<xacc::quantum::DWGraph> graph(); 
	};

	class Classical {
		protected:
			std::vector <std::vector <float> > Qmatrix; // Matrix to hold QUBO data
			std::vector <int> soln; // BUffer for current solution
			unsigned int budget;
			unsigned int n;
			float minEnergy;
			float cfactor;
			float theta[3];
			
			float compute(int x, std::vector <int> &xvec);
		public:
			void solve();

			float getMinEnergy()                   {return minEnergy;}
			std::vector<int> getMinSoln() {return soln;}
			int size() {return n;};

			Classical(const stock_data& data, unsigned int budget, const float theta_in[3]);
	};
	class model {
		protected:
			Quantum   qmodel;
			Classical cmodel;
			
		public:
			model(const stock_data& data, unsigned int budget, const float theta[3]);
			std::vector <int> portfolio(operation type);
			

	};
};
