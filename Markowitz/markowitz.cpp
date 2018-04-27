#include <vector>
#include <cmath>
#include "markowitz.h"
#include "AQCAcceleratorBuffer.hpp"
#include "XACC.hpp"
#include <iostream>

#define DEBUG

#ifdef DEBUG
	#include <stdio.h>
#endif

using namespace markowitz;

model::model(const stock_data& data, const float budget, const float theta[3]) : qmodel(data, budget, theta), cmodel(data, budget, theta)
{
	return;
}

std::vector <int> model::portfolio(operation mode)
{
	if (mode == quantum) {
		xacc::info("Setting up quantum graph");
		auto graph = qmodel.graph();

		xacc::info("Transforming to kernel code");
		const std::string kernelName = "markowitz";
		auto kernel = graph->toKernelSource(kernelName);

		// Generate D-Wave Accelerator QPU
		xacc::info("Allocating QPU resource");
		auto qpu = xacc::getAccelerator("dwave");

		// Generate a reference to qubits buffer,
		xacc::info("Allocating QPU buffer resource");
		auto buffer = qpu->createBuffer("qubits");

		// Generate QPU program
		xacc::info("Building QPU program");
		xacc::Program program(qpu, kernel);
		auto qpuprogram = program.getKernel(kernelName);

		// Execute the QPU program
		xacc::info("Launching program");
		qpuprogram(buffer);

		// Retrieve portfolio from program results
		xacc::info("Post-processing results");

		return qmodel.embedding(buffer);

	} else {
		xacc::info("Running brute-force solution");
		cmodel.solve();
		std::cout << "Optimum value = " << cmodel.getMinEnergy() << std::endl;
		return cmodel.getMinSoln();
	}
}

std::vector<int> Quantum::embedding(std::shared_ptr<xacc::AcceleratorBuffer> buffer) 
{
	// storage for data to be returned
	std::vector<int> results(n, 0);

	auto aBuffer = std::dynamic_pointer_cast<xacc::quantum::AQCAcceleratorBuffer>(buffer);
	if(!aBuffer) {
		xacc::error("Invalid AcceleratorBuffer passed to Factoring HUBO. Must be an AQCAcceleratorBuffer.");
	}

	// bits store the complete list of measurements corresponding to lowest observed energy
	boost::dynamic_bitset<> bits = aBuffer->getLowestEnergyMeasurement();


	// A function declaration...Can we put this somewhere else??
	auto getBit = [](boost::dynamic_bitset<> bits, int idx) -> int {
		// Bits operator[] is in reverse order
		return bits[bits.size()-idx-1];
	};

	// Based on embedding, get the hardware bit states for corresponding problem bits
	int counter = 0;
	auto embedding = aBuffer->getEmbedding();

	// Pretty printing measurement map
	std::cout << "Embedding:\n";
	for (auto& kv : embedding) {
		std::cout << kv.first << ": ";
		for (auto l : kv.second) {
			std::cout << l << " ";
		}; // l
		std::cout << "\n";
	}; // kv

	// Generate map from activeVariableIndices to hardwareIndices
	auto activeVariableIndices = aBuffer->getActiveVariableIndices();
	std::map<int, int> hardwareIndicesToBitIndices;
	for (auto i : activeVariableIndices) {
		// FIXME Is this labeling indices in the order they are found? Does the user know that?
		hardwareIndicesToBitIndices.insert(std::make_pair(i, counter));
		counter++;
	}

	// Reset a counter with no indication of what it counts...
	counter = 0;

	// Step through each embedded logical bit
	for (auto& kv : embedding) {
		auto probIdx = kv.first;
		std::cout << probIdx << ": ";

		// Grab list of associated hardware physical indices
		auto hardwareIndices = kv.second;
		std::vector<int> bitMeasurements;
		for (auto i : hardwareIndices) {
			// Printing bit string
			std::cout << getBit(bits, hardwareIndicesToBitIndices[i]) << " ";

			// Add measurement bits for hardware index i to bitMeasurements buffer
			bitMeasurements.push_back(getBit(bits, hardwareIndicesToBitIndices[i]));
		}; // i

		std::cout << " -> ";

		// Sum them up, ideally all qubits for a given problem
		// node should be measured the same, so sum should be 0
		// or sum / nVertices should be 1. If anything else,
		// then we had an error in the computation
		auto sum = std::accumulate(bitMeasurements.begin(), bitMeasurements.end(), 0);
		int si;
		if (sum == 0) {
			std::cout << "All Bits were 0, setting si to -1\n";
			// Qubits were all a 0 (-1 since this is Ising)
			si = -1;
		} 
		else if ((sum / bitMeasurements.size()) == 1) 
		{
			std::cout << "All Bits were 1, setting si to 1\n";
			// Qubits were all 1
			si = 1;
		}
		else 
		{
			std::cout << "Had a mix of 0 and 1, setting si to ";
			// If we make it here, then we have a mix of 0 and 1
			// for the given logical qubit. So I'm just gonna
			// take the one with the most occurrences
			auto nZeros = std::count(bitMeasurements.begin(), bitMeasurements.end(), 0);
			auto nOnes = std::count(bitMeasurements.begin(), bitMeasurements.end(), 1);
			if(nZeros > nOnes) { // randomly always favor 0...
				std::cout << "-1\n";
				si = -1;
			} else if(nOnes > nZeros) {
				std::cout << "1\n";
				si = 1;
			} else {
				std::cout << "X\n";
				si = -1;
			};
		}; // sum

		// Why didn't we set results sooner...
		int xi = (0.5 * (1 + si)); // what determines plus or minus? Form of Ising model
		results[counter] = xi;
		counter++;
	}

	float min = 0;
	for (int i = 0; i < n; i++) {
		float sum = 0;
		for (int j = 0; j < n; j++) {
			sum += results[j]*Qmatrix[j][i];
		}
		min += sum*results[i];
	}
	min += cfactor;
	std::cout << "Optimum value = " << min << std::endl;

	return results;
}


Quantum::Quantum(const stock_data& data, const float budget, const float theta_in[3]) : budget(budget), n(data.prc.size())
{
	// Theta values
	theta[0] = theta_in[0];
	theta[1] = theta_in[1];
	theta[2] = theta_in[2];

	Qmatrix.resize(n);
	for (int i = 0; i < n; i++) 
		Qmatrix[i].resize(n, 0.0f);

	// QUBO: minimize x'Qx where x = {0, 1}^n
	// Initialize QUBO qudratic couping terms upper triangular
	for (int r = 0; r < n; r++) {
		for (int c = r + 1; c < n; c++) {
			Qmatrix[r][c] = theta[1]*data.cov[r][c] + 2*theta[2]*data.prc[r]*data.prc[c];
		}
	}

	// Initialize QUBO linear terms along diagonal
	for (int i = 0; i < n; i++) {
		Qmatrix[i][i] += (-1.0)*theta[0]*data.ret[i] + theta[2]*data.prc[i]*data.prc[i] - 2*theta[2]*data.prc[i]*budget + theta[1]*data.cov[i][i];
	}

	cfactor = theta[2]*budget*budget;

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

void Quantum::CvtQBUO2Ising()
{
	Hvector.resize(n, 0.0f);
	Jmatrix.resize(n);
	for (int i = 0; i < n; i++) {
		Jmatrix[i].resize(n, 0.0f);
	}

	// Ising: minimize s'h + s'Js where s = {-1,1}^n
	// Convert QUBO to Ising using x = (s + 1) / 2
	// J_{i,j} = Q_{i,j} / 4
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			Jmatrix[i][j] = Qmatrix[i][j] / 4.0;
		}
	}

	// Data structure for Ising hvector is h_i =  0.5*Q_{i,i} + sum_j J_{i,j}
	float jrow    = 0.0f;
	float jfactor = 0.0f;
	float hfactor = 0.0f;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++){
			jrow += Jmatrix[i][j];
		}
		Hvector[i] = Qmatrix[i][i] / 2.0 + jrow;

		// Constant factors generated from conversion
		jfactor += jrow;
		hfactor += Qmatrix[i][i];
	}

	// Set the ising constant factor
	ifactor = cfactor + jfactor + hfactor / 2.0;

#ifdef DEBUG
	printf ("[");
	for (int r = 0; r < n; r++) {
		printf ("%8.2ff,", Hvector[r]); 
	}
	printf ("]\n");

	for (int r = 0; r < n; r++) {
		printf ("[");
		for (int c = 0; c < n; c++) {
			printf ("%8.2f,", Jmatrix[r][c]);
		}
		printf ("]\n");
	}
#endif
}

std::shared_ptr<xacc::quantum::DWGraph> Quantum::graph() 
{
	// Get Ising representatoin to embed problem
	CvtQBUO2Ising();	

	// Graph to represent the problem
	auto graphicalmodel = std::make_shared<xacc::quantum::DWGraph>(n);

	for(int r = 0; r < n; r++) {
		// Add linear vertex bias
		graphicalmodel->setVertexProperties(r, Hvector[r]);
		for(int c = r + 1; c < n; c++) {

			// Add quadratic coupler/edge terms
			graphicalmodel->addEdge(r, c);
			graphicalmodel->setEdgeWeight(r, c, Jmatrix[r][c]);
		}
	}

	return graphicalmodel;
}

/* Initialize the QUBO matrix for the markowitz model */
Classical::Classical(const stock_data& data, const float budget, const float theta_in[3]) : budget(budget), n(data.prc.size())
{
	theta[0] = theta_in[0];
	theta[1] = theta_in[1];
	theta[2] = theta_in[2];

	Qmatrix.resize(n);
	for (int i = 0; i < n; i++) 
		Qmatrix[i].resize(n, 0.0f);

	for (int r = 0; r < n; r++) {
		for (int c = r + 1; c < n; c++) {
			Qmatrix[r][c] = theta[1]*data.cov[r][c] + 2*theta[2]*data.prc[r]*data.prc[c];
		}
	}

	for (int i = 0; i < n; i++) {
		Qmatrix[i][i] += (-1.0)*theta[0]*data.ret[i] + theta[2]*data.prc[i]*data.prc[i] - 2*theta[2]*data.prc[i]*budget + theta[1]*data.cov[i][i];
	}

	cfactor = theta[2]*budget*budget;

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

void Classical::solve()
{
	std::vector <int> xvec(n);
	minEnergy = compute(0, xvec);
	soln = xvec;

	for (int i = 1; i < pow(2, n); i++) {
		float energy = compute(i, xvec);
		if (energy < minEnergy) {
			minEnergy = energy;
			soln = xvec;
		}
	}

	minEnergy += cfactor;
}

float Classical::compute(int x, std::vector <int> &xvec)
{
	for (int i = 0; i < n; i++) {
		xvec[i] = x & 0x1;
		x = x >> 1;
	}

	float min = 0;
	for (int j = 0; j < n; j++) {
		float sum = 0;
		for (int k = 0; k < n; k++) {
			sum += xvec[k]*Qmatrix[k][j];
		}
		min += sum*xvec[j];
	}

	return min;
}
