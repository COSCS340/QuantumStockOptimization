#include "stock_data.hpp"
using namespace std;

/* global */

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

/* stock_data class */

stock_data::stock_data(bool refetchList_p, bool refetchData_p){
    refetchList = refetchList_p;
    refetchData = refetchData_p;

    get_currencies();
    get_history();
    get_prices();
    calc_stats();
}

void stock_data::get_currencies(){
    const string currenciesURL = "https://www.alphavantage.co/digital_currency_list/";
    const string currenciesFile = "./Data/currencies.txt";
    char buf[200];
    char* currCode;
    char* currName;
    currency* currCurr;
    ifstream currencyStream;
    CURL *curl;
    FILE *fp;

    if(refetchList){
        //get the list of currencies
        curl = curl_easy_init();
        if(curl == NULL){
            fprintf(stderr, "ERROR -- stock_data::get_currencies() -- could not init curl\n");
            exit(1);
        }

        fp = fopen(currenciesFile.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_URL, currenciesURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    currencyStream.open(currenciesFile.c_str());

    //parse list of currencies into name and code and store in currencies vector
    while(currencyStream.getline(buf, 200)){
        if(currencyStream.eof()){
            break;
        }
        currCode = strtok(buf, ",");
        currName = strtok(NULL, "\r");

        if(strcmp(currCode,"currency code") == 0) continue;

        currCurr = new currency;
        currCurr->name = currName;
        currCurr->code = currCode;

        currencies.push_back(currCurr);
    }

    currencyStream.close();
    
    return;
}

void stock_data::get_history(){
    string currencyDataURL;
    string currencyDataFile;
    ifstream dataStream;
    double close1, close2, open1, open2;
    double ftrash;
    int itrash;
    char buf[500];
    char* word;
    CURL *curl;
    FILE *fp;
    int i;

    for(i=0; i < (int)currencies.size(); i++){
        sprintf(buf, "https://www.alphavantage.co/query?function=DIGITAL_CURRENCY_DAILY&symbol=%s&market=USD&apikey=G8CLVLYIVI5XYSH&datatype=csv", currencies[i]->code.c_str());
        currencyDataURL = buf;
        sprintf(buf, "./Data/%s_history.txt", currencies[i]->name.c_str());
        currencyDataFile = buf;

        if(refetchData){
            curl = curl_easy_init();
            if(curl == NULL){
                fprintf(stderr, "ERROR -- stock_data::get_history() -- could not init curl\n");
                exit(1);
            }

            //refetch data file 
            printf("Fetching URL:%s\n", currencyDataURL.c_str());

            fp = fopen(currencyDataFile.c_str(),"wb");
            curl_easy_setopt(curl, CURLOPT_URL, currencyDataURL.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
 
            sleep(1);
            fclose(fp);
        }

        //Then parse data file, if it has data
        fp = fopen(currencyDataFile.c_str(),"r");
        if(fp == NULL){
            continue;
        }
        fclose(fp);

        dataStream.open(currencyDataFile.c_str());

        dataStream.getline(buf, 500);
        word = strtok(buf, ",");
        if(strcmp(word, "timestamp") != 0){
            dataStream.close();
            continue;
        }

        //printf("    Parsing %s...\n", currencies[i]->name.c_str());

        while(dataStream.getline(buf, 500)){
            if(dataStream.eof()){
                break;
            }
            sscanf(buf, "%d-%d-%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &itrash,
                                                                        &itrash,
                                                                        &itrash,
                                                                        &open1,
                                                                        &ftrash,
                                                                        &ftrash,
                                                                        &close1,
                                                                        &open2,
                                                                        &ftrash,
                                                                        &ftrash,
                                                                        &close2,
                                                                        &ftrash);
            currencies[i]->values.push_back(open1);
            currencies[i]->values.push_back(open2);
            currencies[i]->values.push_back(close1);
            currencies[i]->values.push_back(close2);
        }
        dataStream.close();
    }

    return;
}

void stock_data::get_prices(){
    string currencyPriceURL;
    string currencyPriceFile;
    ifstream priceStream;
    double price;
    double ftrash;
    int itrash;
    char buf[500];
    char* word;
    CURL *curl;
    FILE *fp;
    int i;

    for(i=0; i < (int)currencies.size(); i++){
        sprintf(buf, "https://www.alphavantage.co/query?function=DIGITAL_CURRENCY_INTRADAY&symbol=%s&market=EUR&apikey=G8CLVLYIVI5XYSH&datatype=csv", currencies[i]->code.c_str());
        currencyPriceURL = buf;
        sprintf(buf, "./Data/%s_price.txt", currencies[i]->name.c_str());
        currencyPriceFile = buf;

        if(refetchData){
            curl = curl_easy_init();
            if(curl == NULL){
                fprintf(stderr, "ERROR -- stock_data::get_prices() -- could not init curl\n");
                exit(1);
            }

            //make sure price file exists, fetch it if it doesn't 
            printf("Fetching URL:%s\n", currencyPriceURL.c_str());

            fp = fopen(currencyPriceFile.c_str(),"wb");
            curl_easy_setopt(curl, CURLOPT_URL, currencyPriceURL.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_perform(curl);
            curl_easy_cleanup(curl);
 
            sleep(3);
            fclose(fp);
        }
/*

        //Then parse price file, if it has data
        fp = fopen(currencyPriceFile.c_str(),"r");
        if(fp == NULL){
            continue;
        }
        fclose(fp);
        priceStream.open(currencyPriceFile.c_str());

        priceStream.getline(buf, 500);
        word = strtok(buf, ",");
        if(strcmp(word, "timestamp") != 0){
            priceStream.close();
            continue;
        }
        //printf("    Parsing %s...\n", currencies[i]->name.c_str());

        while(priceStream.getline(buf, 500)){
            if(priceStream.eof()){
                break;
            }
            sscanf(buf, "%d-%d-%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &itrash,
                                                                        &itrash,
                                                                        &itrash,
                                                                        &open1,
                                                                        &ftrash,
                                                                        &ftrash,
                                                                        &close1,
                                                                        &open2,
                                                                        &ftrash,
                                                                        &ftrash,
                                                                        &close2,
                                                                        &ftrash);
            currencies[i]->price = price;
        }
        priceStream.close();
  */
    }

    return;
}

void stock_data::calc_stats(){
    int i,j;

    for(i=0; i < (int)currencies.size(); i++){
        if(currencies[i]->values.size() == 0) continue;

        currencies[i]->mean = 0;
        for(j=0;j<(int)currencies[i]->values.size();j++){
            currencies[i]->mean += currencies[i]->values[j] / (double)currencies[i]->values.size();
        }
        printf("%s:\n  MEAN:%lf\n", currencies[i]->name.c_str() , currencies[i]->mean);
    }
}