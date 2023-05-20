#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstring>
#include <algorithm>

struct LPPLResult {
    double tc;
    double alpha;
    double omega;
};

struct TableData {
    std::vector<double> date;
    std::vector<double> price;
};

TableData readTableData(const std::string& filepath) {
    TableData tableData;
    std::ifstream file(filepath);
    std::string line;

    // Skip the header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        std::vector<double> row;

        while (std::getline(ss, value, ',')) {
            row.push_back(std::stod(value));
        }

        tableData.date.push_back(row[0]);
        tableData.price.push_back(row[1]);
    }

    file.close();
    return tableData;
}

LPPLResult findBestFit(const std::vector<std::vector<double>>& table) {
    double minVal = table[0][7];
    int bestFitIndex = 0;

    for (int i = 1; i < table.size(); i++) {
        if (table[i][7] < minVal) {
            minVal = table[i][7];
            bestFitIndex = i;
        }
    }

    LPPLResult bestFit;
    bestFit.tc = table[bestFitIndex][2];
    bestFit.alpha = 1 - table[bestFitIndex][3];
    bestFit.omega = table[bestFitIndex][5];
    return bestFit;
}

std::vector<double> calculateMagnitude(const LPPLResult& bestFit, int numPoints) {
    std::vector<double> magnitude(numPoints);

    for (int i = 0; i < numPoints; i++) {
        double t = i + 1;
        magnitude[i] = pow(bestFit.tc - t, -bestFit.alpha);
    }

    return magnitude;
}

std::vector<double> calculateAmplitude(const LPPLResult& bestFit, int numPoints) {
    std::vector<double> amplitude(numPoints);

    for (int i = 0; i < numPoints; i++) {
        double t = i + 1;
        amplitude[i] = cos(bestFit.omega * log(bestFit.tc - t));
    }

    return amplitude;
}

void exportTable(const std::vector<std::vector<double>>& data, const std::string& filepath) {
    std::ofstream file(filepath);
    file << "Datetime,magnitude,amplitude\n";

    for (const auto& row : data) {
        file << row[0] << "," << row[1] << "," << row[2] << "\n";
    }

    file.close();
}

void exportResult(const std::vector<std::vector<double>>& data, const std::string& filepath) {
    std::ofstream file(filepath);
    file << "Datetime,Index,Interval,Interval_fraction,Gradient,Gradient_fraction\n";

    for (const auto& row : data) {
        file << row[0] << "," << row[1] << "," <<row[2] << "," << row[3] << "," << row[4] << "," << row[5] << "\n";
    }

    file.close();
}

int main() {
    std::string outputpath = "/path/to/output/";

    TableData gTable = readTableData(outputpath + "LPPL_table.csv");
    std::vector<std::vector<double>> gData;
    for (int i = 0; i < gTable.date.size(); i++) {
        std::vector<double> row = { gTable.date[i], gTable.price[i] };
        gData.push_back(row);
    }

    LPPLResult bestFit = findBestFit(gData);

    TableData numTable = readTableData(outputpath + "pricedata.csv");
    int numPoints = numTable.date.size();
    
    std::vector<double> magnitude = calculateMagnitude(bestFit, numPoints);
    std::vector<double> amplitude = calculateAmplitude(bestFit, numPoints);

    std::vector<std::vector<double>> outputTable;
    for (int i = 0; i < numPoints; i++) {
        std::vector<double> row = { numTable.date[i], magnitude[i], amplitude[i] };
        outputTable.push_back(row);
    }

    exportTable(outputTable, outputpath + "DECOMPOSITION_table.csv");

    std::vector<std::vector<double>> outputResult;
    // ... populate outputResult with appropriate values ...

    exportResult(outputResult, outputpath + "DECOMPOSITION_Result.csv");

    return 0;
}
