#pragma once
#include <string>

using namespace std;

struct date
{
    string year;
    string month;
    string day;
    bool operator!=(date Date);
    bool operator==(date Date);
};

class PREPROCESS
{
private:
    string File;
    string outputpath;
    string startday;
    int rightdays;
    double rightscale;
    int Tradingday;

public:
    PREPROCESS(string File, string outputpath, string startday = "nothing", int rightdyas = 60, double rightscale = 0.20, int Tradingday = 252)
        : File(File), outputpath(outputpath), startday(startday), rightdays(rightdays), rightscale(rightscale), Tradingday(Tradingday) {}

    date datetime(string day);

    bool preprocess();

    void id_SRTPT(vector<date> MatDate, vector<double> price, int& SRTPT, vector<date>& DATE_CRASH);
};