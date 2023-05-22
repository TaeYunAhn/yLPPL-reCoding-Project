#include <string>

using namespace std;

struct date
{
    string year;
    string month;
    string day;
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
};