#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include "stock_data.hpp"

using namespace std;

void usage(){
    fprintf(stderr, "usage: ./main refetchCurrencyList(y|n) refetchData(y|n)\n");
}

int main(int argc, char **argv) {
    stock_data* data;
    bool refetchList;
    bool refetchData;

    if(argc != 3){
        usage();
        exit(1);
    }

    refetchList = (strcmp(argv[1],"y") == 0) ? true : false;
    refetchData = (strcmp(argv[2],"y") == 0) ? true : false;
    
    data = new stock_data(refetchList, refetchData);

    data->print_stats();
    delete data;
}