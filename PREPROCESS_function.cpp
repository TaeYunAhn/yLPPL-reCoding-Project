#include <iostream>
#include <vector>
#include <algorithm>
#include "PREPROCESS_function.h"

using namespace std;
date PREPROCESS::datetime(string day)
{
}
bool date::operator!=(date Date) {
	if (year == Date.year && month == Date.month && day == Date.day)
		return false;
	return true;
}
bool date::operator==(date Date) {
	if (year == Date.year && month == Date.month && day == Date.day)
		return true;
	return false;
}

bool PREPROCESS::preprocess()
{
	auto fp = fopen(string(File).c_str(), "r");
	vector<string> text;
	vector<double> num_raw;
	char buff[1024];
	char* parse;

	fgets(buff, 1024, fp);
	while (!feof(fp))
	{
		fgets(buff, 1024, fp);
		parse = strtok(buff, ",");
		while (parse != NULL)
		{
			text.push_back(parse);
			parse = strtok(NULL, ",");
			num_raw.push_back(atof(parse));
			while (parse != NULL)
			{
				parse = strtok(NULL, ",");
			}
		}
	}

	vector<string> DateStrings_raw;
	DateStrings_raw.assign(text.begin(), text.end()); 

	vector<double> num;
	vector<string> DateStrings;

	if (startday == "nothing")
	{
		num.assign(num_raw.begin(), num_raw.end());
		DateStrings = DateStrings_raw;  //TODO: assign
	}
	else
	{
		int startpoint;
		for (startpoint = 0; DateStrings_raw[startpoint] != startday; startpoint++);
		for (int i = startpoint; i < DateStrings_raw.size(); i++)
		{
			vector<string> temp(DateStrings_raw.begin() + startpoint, DateStrings_raw.end());
			DateStrings = temp;
			vector<double> temp2(num_raw.begin() + startpoint, num_raw.end());
			num = temp2;
		}
	}

	// 34줄
	vector<date> MatDate;
	for (const auto& date : DateStrings)
	{
		string year = date.substr(0, 4);
		string month = date.substr(5, 2);
		string day = date.substr(8, 2);
		MatDate.push_back({ year, month, day });
	}

	vector<double> price = num;
	rightscale = 1 - rightscale;

	int SRTPT;
	vector<date> DATE_CRASH;
	id_SRTPT(MatDate, price, SRTPT, DATE_CRASH);

	int idx = MatDate.size() - 1;
	date END = MatDate[idx];
	int ENDPT = idx;

	vector<int> keep;		//45행
	vector<date> Date_keep;
	vector<date> Date_datetime;
	vector<string> Date;
	vector<double> Price;
	for (int i = SRTPT; i <= ENDPT; i++) {	//keep이 더 안쓰이면 빼도 될듯
		keep.push_back(i);
		Date_keep.push_back(MatDate[i]);
		Date.push_back(Date_datetime[i].year + "-" + Date_datetime[i].month + "-" + Date_datetime[i].day);//Date_char를 그냥 바로 Date로 써버림
		Price.push_back(price[i]);
	}
	//53줄
	vector<date> Date_CRASH_datetime = DATE_CRASH;

	//그림그리는 부분

	fp = fopen(string(outputpath + "\\pricedata.csv").c_str(), "w");
	fprintf(fp, "data,price,\n");
	for (int i = 0; i < Date.size(); i++) {
		fprintf(fp,"%s,%d,\n", Date[i],Price[i]);
	}
	fclose(fp);
}

void PREPROCESS::id_SRTPT(vector<date> MatDate, vector<double> price, int& SRTPT, vector<date>& DATE_CRASH) {
	vector<int> I_CRASH = *(new vector<int>(MatDate.size(),0));
	DATE_CRASH = (*new vector<date>(MatDate.size(), {"0","0","0"}));
	vector<double> PERCENT_DROP = *(new vector<double>(MatDate.size(), 0));
	int Left_days = Tradingday;
	double max_prior, max_post, min_post;
	vector<double> drop_post;
	//인덱스 맞는지 확인해주세요
	for (int i = 0; i < MatDate.size(); i++) {
		if (i - Left_days > 0) {
			max_prior = *max_element(price.begin() + (i - Left_days), price.begin() + i - 1);
		}
		else if (i > 1) {
			max_prior = *max_element(price.begin(), price.begin() + i - 1);
		}
		else {
			max_prior = price[i];		//이런 의미가 맞는지..?
		}
		//161줄
		if (i + rightdays < MatDate.size()) {
			max_post = *max_element(price.begin() + i + 1, price.begin() + i + rightdays);
			for (int j = i + 1; i <= i + rightdays; i++) {
				if (price[j] < rightscale * price[i]) {
					drop_post.push_back(j);
				}
			}
			min_post = *min_element(price.begin() + i + 1, price.begin() + i + rightdays);
		}
		else if (i + 1 < MatDate.size()) {
			max_post = *max_element(price.begin()+i+1, price.end());
			for (int j = i + 1; i <= price.size(); i++) {
				if (price[j] < rightscale * price[i]) {
					drop_post.push_back(j);
				}
			}
			min_post = *min_element(price.begin() + i + 1, price.end());
		}
		else {
			max_post, min_post = *price.end();
			drop_post.push_back(price.size());
		}

		if (max_prior < price[i] && !drop_post.empty() && max_post < price[i]) {
			I_CRASH[i] = 1;
			DATE_CRASH[i] = MatDate[i];
			PERCENT_DROP[i] = (1 - min_post / price[i]) * 100;
		}
	}
	vector<date> new_DATE_CRASH;
	vector<double> new_PERCENT_DROP;
	for (int i = 0; i < DATE_CRASH.size(); i++) {
		if (DATE_CRASH[i] != date({"0","0","0"})) {
			new_DATE_CRASH.push_back(DATE_CRASH[i]);
		}
		if (PERCENT_DROP[i] != 0) {
			new_PERCENT_DROP.push_back(PERCENT_DROP[i]);
		}
	}
	//186줄
	date CT;
	int id_crash, I;
	double M;
	if (new_DATE_CRASH.size() > 0) {
		int idx = new_DATE_CRASH.size();
		CT = new_DATE_CRASH[idx];
		for (int i = 0; i < MatDate.size(); i++) {
			if (MatDate[i] == CT) {
				id_crash = i;
			}
		}
		M = price[id_crash];
		I = id_crash;
		for (int i = id_crash; i < price.size(); i++) {
			if (M > price[i]) {
				M = price[i];
				I = i;
			}
		}
	}
	else {
		M = price[0];		//매트랩에선 1인데 C++이니까 0으로 해줌
		I = 0;
		id_crash = 0;
		for (int i = 0; i < price.size(); i++) {
			if (M > price[i]) {
				M = price[i];
				I = i;
			}
		}
		//CT = 1; 여기를 어케 처리해야할까...?
	}

	SRTPT = id_crash + I - 1;
}
