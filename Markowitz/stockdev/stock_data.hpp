#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string.h>
#include <curl/curl.h>

/* Stock data used for markotwitz model */
class stock_data {
    public:
        //markowitz stuff
        std::vector <float> prc;
        std::vector <float> ret;
        std::vector <std::vector<float> > cov; // Upper Triangular matrix

        //other stuff
        std::map<std::string, std::string> tickers; 

        //methods
        stock_data();
        int get_tickers();
        int get_data();
};


/* Legacy Functions I encourage you to overwrite them */
/*
namespace csv_reader {
    stock_data get_stock_data (std::ifstream &, std::ifstream &, std::ifstream &, unsigned int);
    void read_csv_row (std::ifstream &, std::vector <float> &);
    void read_csv (std::ifstream &, std::vector <std::vector<float> > &);
};
*/

/* TODO: Implement obtaining stock data */

/******************************************************************************
Obtain Historical Data -> Calculate Statistical Data -> Return data 

1: Obtaining Historical Data
    The stock data can be obtained from https://www.alphavantage.co/ it has a very simple API and only
    requires you generate an API key. The data can be downloaded in a JSON or CSV format. The website lists
    the different currencies you can get. You can download the data using CURL or a similar library. Save this
    data to a file or parse it and pass it to the next stage. You'll need to repeat this for every stock you
    want to check.

    Here's an example using my API key to get the daily values of Bitcoin
    https://www.alphavantage.co/query?function=DIGITAL_CURRENCY_DAILY&symbol=BTC&market=USD&apikey=G8CLVLYIVI5XYSH&datatype=csv
                                        ^ Daily values for crypto     ^ Bitcoin   ^ Dollars   ^ My Key             ^ CSV output

    Get the ticker values from the CSV at https://www.alphavantage.co/digital_currency_list/

2: Calculate Statistical Data
    Once you obtain the data you need to calculate the statistical data for the markowitz model. The Markowitz
    model uses Expected Return, Covariance, and the current Price. The CSV downloaded from the website should
    look like this with several rows for the actual data. We should only be converned with the "Open" or
    "Close" in this file. These values correspond to the (USD) value of one asset. 

    timestamp, open, high, low, close, open, high, low, close, volume, market cap 

    Expected return can simply be defined as the mean of the values observed (The most likely value of the asset) 
    So, the expected return of an asset can be calculated as follows for every opening or closing dollar value
    value a_i 

    mean(a) = E(a) = 1 / N * (sum_i a_i to N)

    The covariance is calculated using the mean for assets a and b. You make an uppper triangular matrix out
    of this by calculating the covariance of each unique pair. 

    cov(a, b) = 1 / (N - 1) * (sum_i (a_i - mean(a))*(b_i - mean(b)) to N)

    The current price can simply be read from the data file (you might need to take a the intraday data to get
    today's data).

3: Return Data
    Take this statistical data and return it in the stock_data struct. 

******************************************************************************/
