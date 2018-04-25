#include "stock_data.hpp"
using namespace std;

/* global */

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

/* stock_data class */

stock_data::stock_data(){
    if(get_tickers()){
        fprintf(stderr, "Error in stock_data() -- Could not get ticker names\n");
    }
    if(get_data()){
        fprintf(stderr, "Error in stock_data() -- Could not get ticker data\n");
    }
}

int stock_data::get_tickers(){
    const string tickersURL = "https://www.alphavantage.co/digital_currency_list/";
    const string tickersFile = "./Data/tickers.txt";
    char buf[200];
    char* buf2;
    string currTick;
    string currName;
    ifstream tickerStream;
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if(curl == NULL){
        return 1;
    }

    fp = fopen(tickersFile.c_str(),"wb");
    curl_easy_setopt(curl, CURLOPT_URL, tickersURL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    fclose(fp);

    tickerStream.open(tickersFile.c_str());

    while(tickerStream.getline(buf, 200)){
        if(tickerStream.eof()){
            break;
        }
        buf2 = strtok(buf, ",");
        currTick = buf2;
        buf2 = strtok(NULL, "\r");
        currName = buf2;
        if(currTick == "currency code") continue;
        
        tickers.insert(make_pair(currTick, currName));
    }

    tickerStream.close();
    
    return 0;
}

int stock_data::get_data(){
    map<string, string>::iterator mit;
    string tickerDataURL;
    string tickerDataFile;
    char buf[500];
    CURL *curl;
    FILE *fp;
    CURLcode res;

    for(mit = tickers.begin(); mit != tickers.end(); mit++){
        printf("%s: %s\n", mit->first.c_str(), mit->second.c_str());

        sprintf(buf, "https://www.alphavantage.co/query?function=DIGITAL_CURRENCY_DAILY&symbol=%s&market=USD&apikey=G8CLVLYIVI5XYSH&datatype=csv", mit->first.c_str());
        tickerDataURL = buf;
        sprintf(buf, "./Data/%sdata.txt", mit->second.c_str());
        tickerDataFile = buf;

        printf("    %s\n    %s\n", tickerDataURL.c_str(), tickerDataFile.c_str());
        printf("\n");
/*
        fp = fopen(tickerDataFile.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_URL, tickersURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
*/
    }

    return 0;
}



/*
//csv_reader namespace
stock_data csv_reader::get_stock_data (std::ifstream &prc_file,
                                 std::ifstream &ret_file,
                                 std::ifstream &cov_file,
                                 unsigned int n)
{
    stock_data data;
    data.prc.resize(n);
    data.ret.resize(n);
    data.cov.resize(n);
    for (unsigned int i = 0; i < n; i++) data.cov[i].resize(n);

    read_csv_row(prc_file, data.prc);
    read_csv_row(ret_file, data.ret);
    read_csv(cov_file, data.cov);
    return data;
}

// Template functions maybe?
void csv_reader::read_csv_row (std::ifstream &infile, std::vector <float> &vec)
{
    std::string buffer;
    getline(infile, buffer, '\n');
    std::istringstream iss(buffer);
    for (unsigned int i = 0; i < vec.size(); i++) {
        getline(iss, buffer, ',') ;
        std::istringstream ss(buffer);
        ss >> vec[i];
    }

    return;
}

void csv_reader::read_csv (std::ifstream &infile, std::vector <std::vector<float> > &vec)
{
    for (unsigned int i = 0; i < vec.size(); i++) {
        read_csv_row(infile, vec[i]);
    }

    return;
}
*/