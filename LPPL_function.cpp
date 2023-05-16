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
	LPPL(string opp, int cycle = 500, double PopulationSize = 200, double Generations = 700, double pricegyration = 0)		//������
		:outputpath(opp), cycle(cycle), PopulationSize(PopulationSize), Generations(700), pricegyration(pricegyration) {
	}
	

	bool input(string filename) {

	}

	bool lppl() {
		fp = fopen(string(outputpath+"\\pricedata.csv").c_str(), "r");
		vector<char[11]> date(100);
		vector<double> price;
		char buff[1024];
		char* parse;
		int i = 0;
		
		fgets(buff, 1024, fp);
		while (!feof(fp)) {					//������ �Է� �κ�
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
		
		//price log�� �ٲٱ�(20��)
		vector<double> Logprice;
		for (int i = 0; i < price.size(); i++) {
			Logprice.push_back(log(price[i]));
		}
		fclose(fp);


		if (pricegyration == 1) {
			fp = fopen(string(outputpath + "\\INITIALVALUE_table.csv").c_str(), "r");

			vector<vector<double>> ip_table;		//���� ���� ���� ���� �𸣰���
			vector<vector<double>> ip;			//��������
			G = {};

			for (int i_run = 1; i <= cycle; i++) {
				ip_cycle = {};
				//�˸��� ����� ����
				for (int i = ((i_run - 1) * PopulationSize); i < i_run * PopulationSize; i++) {
					ip_cycle_raw.push_back(ip[i]);
				}

				for (int ip_cycle_lin = 1; ip_cycle_lin <= PopulationSize; ip_cycle_lin++) {
					if (ip_cycle_raw[ip_cycle_lin] != *new vector<double>(8, 0)) {
						ip_cycle.push_back(ip_cycle_raw[ip_cycle_lin]);
					}
				}
				//���� �ڵ� 37��
				if (ip_cycle_raw[0] == *new vector<double>(8, 0)) {
					double mw = 0;
					T = Logprice;
					
					//options = gaoptimset('Display', 'off', 'PopulationSize', PopulationSize, 'Generations', Generations);
					//������ Ÿ���� ���� �� �𸣰���
					vector<double> G0;		//G0�� Ÿ���� ��Ȯ�ϰԴ� �𸣰���
					//[G0, RMSE] = ga(@LPPLFunctionGA, 7, [], [], [], [], lb, ub, [], options);
					//�̰ŵ� �� �𸣰���. ��ư ga

					if (G0[4] > 0.9 || G0[4] < 0.1)
						continue;
					//G.push_back({ G0,RMSE,mw });	//G�� ��Ȯ�� �� ���������� ���� �𸣰ڴ�

				}

				else { //49��° ��
					double mw = ip_cycle[1][8];
					vector<vector<double>> ip0(ip_cycle.size());
					for (int i = 0; i < ip_cycle.size(); i++) {	//�� ������ �𸣰����� �ϴ� �� �ȵ��� ���߿� ����
						copy(ip_cycle[i].begin(), ip_cycle[i].begin() + 7, ip0[i]);
					}
					int L = Logprice.size();
					int WS_max = 30;
					int L_min = L - WS_max;
					vector<double> T(Logprice.begin(), Logprice.begin() + L_min + mw);
					
					//options = gaoptimset('Display', 'off', 'PopulationSize', PopulationSize, 'Generations', Generations);
					//������ Ÿ���� ���� �� �𸣰���
					vector<double> G0;		//G0�� Ÿ���� ��Ȯ�ϰԴ� �𸣰���
					//[G0, RMSE] = ga(@LPPLFunctionGA, 7, [], [], [], [], lb, ub, [], options);
					//�̰ŵ� �� �𸣰���. ��ư ga

					if (G0[4] > 0.9 || G0[4] < 0.1)
						continue;
					//G.push_back({ G0,RMSE,mw });	//G�� ��Ȯ�� �� ���������� ���� �𸣰ڴ�
				}

			}

			//67��
			if (ip_cycle_raw[1] == *new vector<double>(8, 0)) {
				if (G.size() > 1) {			//size�� ��Ʈ�� lenght�� ������ �Ȱ����� Ȯ���غ��� �� ��
					//array2table�� C++�� �����ϴ°� �����ε�
				}
				else {
					//���⵵ ���������� arry2table�� C++�� �����ؾ���
				}
			}
			else {//�Ȱ��� �ڵ絥 �� if�� ���� ��������...?

				if (G.size() > 1) {			//size�� ��Ʈ�� lenght�� ������ �Ȱ����� Ȯ���غ��� �� ��
					//array2table�� C++�� �����ϴ°� �����ε�
				}
				else {
					//���⵵ ���������� arry2table�� C++�� �����ؾ���
				}
			}
			//��º��ε�... �ϴ� ����
			//writetable(LPPL_table, outputpath + "\" +'LPPL_table.xlsx');
		}

		//92��
		else {
			T = Logprice;
			G = *new vector<vector<double>>(8, vector<double>(8, 0));

			for (int i_run = 1; i <= cycle; i++) {
				//��� �Ȱ��� �ڵ� �����µ� �Լ��� ���߿� �ٲٱ�
				
				//options = gaoptimset('Display', 'off', 'PopulationSize', PopulationSize, 'Generations', Generations);
				//������ Ÿ���� ���� �� �𸣰���
				vector<double> G0;		//G0�� Ÿ���� ��Ȯ�ϰԴ� �𸣰���
				//[G0, RMSE] = ga(@LPPLFunctionGA, 7, [], [], [], [], lb, ub, [], options);
				//�̰ŵ� �� �𸣰���. ��ư ga

				if (G0[4] > 0.9 || G0[4] < 0.1)
					continue;
				//G[i_run] = { G0,RMSE };	//G�� ��Ȯ�� �� ���������� ���� �𸣰ڴ�
			}
			if (G.size() > 1) {			//size�� ��Ʈ�� lenght�� ������ �Ȱ����� Ȯ���غ��� �� ��
					//array2table�� C++�� �����ϴ°� �����ε�
			}
			else {
				//���⵵ ���������� arry2table�� C++�� �����ؾ���
			}
			//writetable(LPPL_table, outputpath + "\" +'LPPL_table.xlsx');
		}
		return true;
	}

};

int main() {
	string path = "C:\\Users\\USER\\OneDrive\\���� ȭ��";
	LPPL test(path);
	test.lppl();

	return 0;
}