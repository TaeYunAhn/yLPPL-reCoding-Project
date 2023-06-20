#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
#include "LPPL_function.h"

#include "tools/global.h"
#include <string>

using namespace std;

bool LPPL::lppl()
{
    try
    {
        FILE* fp = fopen(string(outputpath + "\\pricedata.csv").c_str(), "r");
        vector<char[11]> date;
        date.reserve(100);

        vector<double> price;
        price.reserve(100);
        char buff[1024];
        char* parse = nullptr;
        int i = 0;

        fgets(buff, 1024, fp);
        while (!feof(fp))
        { //?????? ??? ?��?
            fgets(buff, 1024, fp);
            parse = strtok(buff, ",");
            while (parse != NULL)
            {
                strcpy(date[i], parse);
                i++;
                parse = strtok(NULL, ",");
                price.push_back(atof(parse));
                while (parse != NULL)
                {
                    parse = strtok(NULL, ",");
                }
            }
        }

        Matrix G = {};
        Matrix ip_cycle;
        Matrix ip_cycle_raw;
        vector<double> T;
        vector<double> lb = { *max_element(T.begin(), T.end()), DBL_MIN, (double)T.size(), 0.1, -1, 4.8, 0 };
        vector<double> ub = { DBL_MAX, 0, DBL_MAX, 0.9, 1, 13, 2 * M_PI };

        // price log?? ????(20??)
        vector<double> logPrice;
        logPrice.reserve(price.size());
        for (const auto& p : price) {
            logPrice.push_back(log(p));
        }

        fclose(fp);

        if (pricegyration == 1)
        {
            FILE* fp_initValue = fopen(string(outputpath + "\\INITIALVALUE_table.csv").c_str(), "r");

            Matrix ip;
            fgets(buff, 1024, fp);
            while (!feof(fp))
            { //?????? ??? ?��?
                vector<double> d;
                d.reserve(10);      //매직넘버를 정확히 모르겠어서...
                fgets(buff, 1024, fp);
                parse = strtok(buff, ",");
                while (parse != NULL)
                {
                    parse = strtok(NULL, ",");
                    d.push_back(atof(parse));
                }
                ip.push_back(d);
            }
            fclose(fp_initValue);

            G = {};

            for (int i_run = 1; i <= cycle; i++)
            {
                ip_cycle = {};
                ip_cycle_raw.reserve(ip.size());
                //????? ????? ????
                for (int i = ((i_run - 1) * populationSize); i < i_run * populationSize; i++)
                {
                    
                    ip_cycle_raw.push_back(ip[i]);
                }

                vector<double> v(8, 0);
                ip_cycle.reserve(ip_cycle_raw.size());

                for (int ip_cycle_lin = 1; ip_cycle_lin <= populationSize; ip_cycle_lin++)
                {
                    if (!EQVEC(ip_cycle_raw[ip_cycle_lin], v))
                        ip_cycle.push_back(ip_cycle_raw[ip_cycle_lin]);
                }

                //???? ??? 37??
                if (!EQVEC(ip_cycle_raw[0], v))
                {
                    double mw = 0;
                    T = logPrice;

                    // options = gaoptimset('Display', 'off', 'populationSize', populationSize, 'generations', generations);
                    //?????? ????? ???? ?? ?????
                    vector<double> G0; // G0?? ????? ??????? ?????
                    //[G0, RMSE] = ga(@LPPLFunctionGA, 7, [], [], [], [], lb, ub, [], options);
                    //???? ?? ?????. ??? ga

                    if (G0[4] > 0.9 || G0[4] < 0.1)
                        continue;
                    // G.push_back({ G0,RMSE,mw });	//G?? ????? ?? ?????????? ???? ????
                }

                else
                { // 49??�� ??
                    double mw = ip_cycle[1][8];
                    Matrix ip0;
                    const auto cycleSize = ip_cycle.size();
                    ip0.reserve(cycleSize);

                    for (int i = 0; i < cycleSize; i++)
                    { //?? ?????? ??????? ??? ?? ?????? ????? ????
                        copy(ip_cycle[i].begin(), ip_cycle[i].begin() + 7, ip0[i]);
                    }
                    int L = logPrice.size();
                    int WS_max = 30;
                    int L_min = L - WS_max;
                    vector<double> T1(logPrice.begin(), logPrice.begin() + L_min + mw);

                    // options = gaoptimset('Display', 'off', 'populationSize', populationSize, 'generations', generations);
                    //?????? ????? ???? ?? ?????
                    vector<double> G0; // G0?? ????? ??????? ?????
                    //[G0, RMSE] = ga(@LPPLFunctionGA, 7, [], [], [], [], lb, ub, [], options);
                    //???? ?? ?????. ??? ga

                    if (G0[4] > 0.9 || G0[4] < 0.1)
                        continue;
                    // G.push_back({ G0,RMSE,mw });	//G?? ????? ?? ?????????? ???? ????
                }
            }

            //??��?(67~91??) table?? ?????��? ???? ??? LPPL?? ??????
            fp = fopen(string(outputpath + "\\LPPL_table.csv").c_str(), "w");
            fprintf(fp, "A,B,Tc,beta,C,Omega,Phi,RMSE\n");
            for (int i = 0; i < G.size(); i++)
            {
                for (int j = 0; j < G[i].size(); j++)
                {
                    fwrite(&G[i][j], sizeof(double), 1, fp);
                    fprintf(fp, ",");
                }
                fprintf(fp, "\n");
            }
        }
        else // 92??
        {
            T = logPrice;
            G = Matrix(8, vector<double>(8, 0));

            for (int i_run = 1; i <= cycle; i++)
            {
                //??? ????? ??? ??????? ????? ????? ????

                // options = gaoptimset('Display', 'off', 'populationSize', populationSize, 'generations', generations);
                //?????? ????? ???? ?? ?????
                vector<double> G0; // G0?? ????? ??????? ?????
                //[G0, RMSE] = ga(@LPPLFunctionGA, 7, [], [], [], [], lb, ub, [], options);
                //???? ?? ?????. ??? ga

                if (G0[4] > 0.9 || G0[4] < 0.1)
                    continue;
                // G[i_run] = { G0,RMSE };	//G?? ????? ?? ?????????? ???? ????
            }

            fp = fopen(string(outputpath + "\\LPPL_table.csv").c_str(), "w");
            fprintf(fp, "A,B,Tc,beta,C,Omega,Phi,RMSE,\n");
            for (int i = 0; i < G.size(); i++)
            {
                for (int j = 0; j < G[i].size(); j++)
                {
                    fwrite(&G[i][j], sizeof(double), 1, fp);
                    fprintf(fp, ",");
                }
                fprintf(fp, "\n");
            }
        }
        return true;
    }
    catch (const std::exception& e)
    {
        // std::cerr << e.what() << '\n';
        // LOG_ERROR(Logger::getInstance(), "Exception Error => %s", e.what());
    }


}