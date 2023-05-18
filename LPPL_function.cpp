#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>

using namespace std;

class LPPL {
private:
	string outputpath;
	int cycle;
	double PopulationSize;
	double Generations;
	double pricegyration;
	FILE* fp;
public:
	LPPL(string opp, int cycle = 500, double PopulationSize = 200, double Generations = 700, double pricegyration = 0)		//생성자
		:outputpath(opp), cycle(cycle), PopulationSize(PopulationSize), Generations(700), pricegyration(pricegyration) {
	}

	bool lppl() {
		fp = fopen(string(outputpath+"\\pricedata.csv").c_str(), "r");
		vector<char[11]> date(100);
		vector<double> price;
		char buff[1024];
		char* parse;
		int i = 0;
		
		fgets(buff, 1024, fp);
		while (!feof(fp)) {					//데이터 입력 부분
			fgets(buff, 1024, fp);
			parse = strtok(buff, ",");
			while (parse != NULL) {
				strcpy(date[i], parse);
				i++;
				parse = strtok(NULL, ",");
				price.push_back(atof(parse));
				while (parse != NULL) {
					parse = strtok(NULL, ",");
				}
			}
		}


		vector<vector<double>> G = {};
		vector<vector<double>> ip_cycle;
		vector<vector<double>> ip_cycle_raw;
		vector<double> T;

		vector<double> lb = { *max_element(T.begin(),T.end()), DBL_MIN, (double)T.size(), 0.1, -1, 4.8, 0 };
		vector<double> ub = { DBL_MAX, 0, DBL_MAX, 0.9, 1, 13, 2 * M_PI };
		
		//price log로 바꾸기(20행)
		vector<double> Logprice;
		for (int i = 0; i < price.size(); i++) {
			Logprice.push_back(log(price[i]));
		}
		fclose(fp);


		if (pricegyration == 1) {
			fp = fopen(string(outputpath + "\\INITIALVALUE_table.csv").c_str(), "r");

			vector<vector<double>> ip;
			i = 0;
			fgets(buff, 1024, fp);
			while (!feof(fp)) {					//데이터 입력 부분
				fgets(buff, 1024, fp);
				parse = strtok(buff, ",");
				while (parse != NULL) {
					parse = strtok(NULL, ",");
					ip[i].push_back(atof(parse));
				}
				i++;
			}
			fclose(fp);

			G = {};

			for (int i_run = 1; i <= cycle; i++) {
				ip_cycle = {};
				//알맞은 행들을 저장
				for (int i = ((i_run - 1) * PopulationSize); i < i_run * PopulationSize; i++) {
					ip_cycle_raw.push_back(ip[i]);
				}

				for (int ip_cycle_lin = 1; ip_cycle_lin <= PopulationSize; ip_cycle_lin++) {
					if (ip_cycle_raw[ip_cycle_lin] != *new vector<double>(8, 0)) {
						ip_cycle.push_back(ip_cycle_raw[ip_cycle_lin]);
					}
				}
				//원본 코드 37줄
				if (ip_cycle_raw[0] == *new vector<double>(8, 0)) {
					double mw = 0;
					T = Logprice;
					
					//options = gaoptimset('Display', 'off', 'PopulationSize', PopulationSize, 'Generations', Generations);
					//위에꺼 타입이 뭔지 잘 모르겠음
					vector<double> G0;		//G0의 타입을 정확하게는 모르겠음
					//[G0, RMSE] = ga(@LPPLFunctionGA, 7, [], [], [], [], lb, ub, [], options);
					//이거도 잘 모르겠음. 암튼 ga

					if (G0[4] > 0.9 || G0[4] < 0.1)
						continue;
					//G.push_back({ G0,RMSE,mw });	//G에 정확히 뭘 집어넣으라는 건지 모르겠다

				}

				else { //49번째 줄
					double mw = ip_cycle[1][8];
					vector<vector<double>> ip0(ip_cycle.size());
					for (int i = 0; i < ip_cycle.size(); i++) {	//잘 될지는 모르겠지만 일단 값 안들어가면 나중에 수정
						copy(ip_cycle[i].begin(), ip_cycle[i].begin() + 7, ip0[i]);
					}
					int L = Logprice.size();
					int WS_max = 30;
					int L_min = L - WS_max;
					vector<double> T(Logprice.begin(), Logprice.begin() + L_min + mw);
					
					//options = gaoptimset('Display', 'off', 'PopulationSize', PopulationSize, 'Generations', Generations);
					//위에꺼 타입이 뭔지 잘 모르겠음
					vector<double> G0;		//G0의 타입을 정확하게는 모르겠음
					//[G0, RMSE] = ga(@LPPLFunctionGA, 7, [], [], [], [], lb, ub, [], options);
					//이거도 잘 모르겠음. 암튼 ga

					if (G0[4] > 0.9 || G0[4] < 0.1)
						continue;
					//G.push_back({ G0,RMSE,mw });	//G에 정확히 뭘 집어넣으라는 건지 모르겠다
				}

			}

			//출력부(67~91행) table로 변환하는거 없이 바로 LPPL에 넣어버림
			fp = fopen(string(outputpath + "\\LPPL_table.csv").c_str(), "w");
			fprintf(fp, "A,B,Tc,beta,C,Omega,Phi,RMSE\n");
			for (int i = 0; i < G.size(); i++) {
				for (int j = 0; j < G[i].size(); j++) {
					fwrite(&G[i][j], sizeof(double), 1, fp);
					fprintf(fp, ",");
				}
				fprintf(fp, "\n");
			}
		}

		//92행
		else {
			T = Logprice;
			G = *new vector<vector<double>>(8, vector<double>(8, 0));

			for (int i_run = 1; i <= cycle; i++) {
				//계속 똑같은 코드 나오는듯 함수로 나중에 바꾸기
				
				//options = gaoptimset('Display', 'off', 'PopulationSize', PopulationSize, 'Generations', Generations);
				//위에꺼 타입이 뭔지 잘 모르겠음
				vector<double> G0;		//G0의 타입을 정확하게는 모르겠음
				//[G0, RMSE] = ga(@LPPLFunctionGA, 7, [], [], [], [], lb, ub, [], options);
				//이거도 잘 모르겠음. 암튼 ga

				if (G0[4] > 0.9 || G0[4] < 0.1)
					continue;
				//G[i_run] = { G0,RMSE };	//G에 정확히 뭘 집어넣으라는 건지 모르겠다
			}

			fp = fopen(string(outputpath + "\\LPPL_table.csv").c_str(), "w");
			fprintf(fp, "A,B,Tc,beta,C,Omega,Phi,RMSE,\n");
			for (int i = 0; i < G.size(); i++) {
				for (int j = 0; j < G[i].size(); j++) {
					fwrite(&G[i][j], sizeof(double), 1, fp);
					fprintf(fp, ",");
				}
				fprintf(fp, "\n");
			}
		}
		return true;
	}

};
