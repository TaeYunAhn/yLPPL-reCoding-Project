#include <iostream>
#include <vector>
#include "PREPROCESS_function.h"

using namespace std;
date PREPROCESS::datetime(string day)
{ // day�� dateŸ������ �ٲٴ� �Լ�
}

bool PREPROCESS::preprocess()
{
    auto fp = fopen(string(File).c_str(), "r");
    vector<string> text;
    vector<double> num_raw;
    char buff[1024];
    char *parse;

    fgets(buff, 1024, fp);
    while (!feof(fp))
    { // ������ �Է� �κ�
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
    DateStrings_raw.assign(text.begin(), text.end()); // text�� DateStrings_raw������?

    vector<double> num;
    vector<string> DateStrings;

    if (startday == "nothing")
    {
        num.assign(num_raw.begin(), num_raw.end());
        DateStrings = DateStrings_raw;  //TODO: assign
    }
    else
    {
        for (int startpoint = 0; DateStrings_raw[i] != startday; startpoint++)
        {
            vector<string> temp(DateStrings_raw.begin() + startpoint, DateStrings_raw.end());
            DateStrings = temp;
            vector<double> temp2(num_raw.begin() + startpoint, num_raw.end());
            num = temp2;
        }
    }

    // ���� 34��
    vector<date> t;
    for ( const auto& date : DateStrings)
    {
        string year = date.substr(0, 4);
        string month = date.substr(5, 2);
        string day = date.substr(8, 2);
        t.push_back({year, month, day});
    }
}