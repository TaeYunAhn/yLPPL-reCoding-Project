#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

struct date {
	string year;
	string month;
	string day;

};

class PREPROCESS {
private:
	string File;
	string outputpath;
	string startday;
	int rightdays;
	double rightscale;
	int Tradingday;
	FILE* fp;
public:
	PREPROCESS(string File, string outputpath, string startday = "nothing", int rightdyas = 60, double rightscale = 0.20, int Tradingday = 252)
		:File(File), outputpath(outputpath),startday(startday),rightdays(rightdays),rightscale(rightscale), Tradingday(Tradingday){}

	date datetime(string day) {		//day를 date타입으로 바꾸는 함수

	}

	bool preprocess() {
		fp = fopen(string(File).c_str(), "r");
		vector<string> text;
		vector<double> num_raw;
		char buff[1024];
		char* parse;
		int i = 0;

		fgets(buff, 1024, fp);
		while (!feof(fp)) {					//데이터 입력 부분
			fgets(buff, 1024, fp);
			parse = strtok(buff, ",");
			while (parse != NULL) {
				text.push_back(parse);
				i++;
				parse = strtok(NULL, ",");
				num_raw.push_back(atof(parse));
				while (parse != NULL) {
					parse = strtok(NULL, ",");
				}
			}
		}

		vector<string> DateStrings_raw = text;	//text랑 DateStrings_raw같은거?

		vector<double> num;
		vector<string> DateStrings;

		if (startday == "nothing") {
			num = num_raw;
			DateStrings = DateStrings_raw;
		}
		else {
			int startpoint;
			for (startpoint = 0; DateStrings_raw[i] != startday; startpoint++);
			vector<string> temp(DateStrings_raw.begin() + startpoint, DateStrings_raw.end());
			DateStrings = temp;
			vector<double> temp2(num_raw.begin() + startpoint, num_raw.end());
			num = temp2;
		}

		//원본 34행
		vector<date> t;
		for (int i = 0; i < DateStrings.size(); i++) {
			string year = DateStrings[i].substr(0,4);
			string month = DateStrings[i].substr(5, 2);
			string day = DateStrings[i].substr(8, 2);
			t.push_back({ year,month,day });
		}
		
	}

};