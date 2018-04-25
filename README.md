# Quantum Stock Optimization

We are using Quantum Computing to do stock market optimization.

## Proposal is [here](proposal.md).

## Build

 - Prequisite xacc
 - git clone https://github.com/COSCS340/QuantumStockOptimization 
 - cd Markowitz
 - mkdir build
 - cd build
 - cmake ../
 - make

#3 Usage

 - ./Markowitz --s ticker_symbols.csv --b 1000. --n 63 --t1 0.33 --t2 0.33 --t3 0.33 --dwave-anneal-time 200
