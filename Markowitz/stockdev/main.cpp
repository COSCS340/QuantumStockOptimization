#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include "stock_data.h"

using namespace std;

void usage(){
    fprintf(stderr, "usage: ./main refetchCurrencyList(y|n) refetchData(y|n)\n");
}

int main(int argc, char **argv) {
    bool refetchList;
    bool refetchData;
    int i, j;

    if(argc != 3){
        usage();
        exit(1);
    }

    refetchList = (strcmp(argv[1],"y") == 0) ? true : false;
    refetchData = (strcmp(argv[2],"y") == 0) ? true : false;
    
    stock_data data(refetchList, refetchData);

    //data.print_stats();

    printf("Returns\n");
    for(i = 0; i < (int)data.ret.size(); i++){
        printf("%10.2f ", data.ret[i]);
    }
    printf("\nPrice\n");
    for(i = 0; i < (int)data.prc.size(); i++){
        printf("%10.2f ", data.prc[i]);
    }
    printf("\nCovariance Matrix\n");
    printf("     ");
    for(i=0; i < (int)data.currencies.size(); i++){
        printf("%12s ", data.currencies[i]->code.c_str());
    }
    printf("\n");
    for(i=0; i < (int)data.currencies.size(); i++){
        printf("%5s", data.currencies[i]->code.c_str());
        for(j=0; j < (int)data.cov[i].size(); j++){
            printf("%12.3f ", data.cov[i][j]);
        }
        printf("\n");
    }
}
