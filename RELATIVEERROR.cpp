#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>

struct LPPLResult {
    double a1, a2, a3, a4, a5, a6, a7, a8;
};

struct PriceData {
    std::vector<double> date;
    std::vector<double> price;
};

struct RelativeErrorResult {
    std::vector<double> date;
    std::vector<double> relativeError;
};

LPPLResult findBestFit(const std::vector<LPPLResult>& results) {
    auto bestFit = std::min_element(results.begin(), results.end(), [](const LPPLResult& r1, const LPPLResult& r2) {
        return r1.a8 < r2.a8;
    });
    return *bestFit;
}

void calculateRelativeError(const LPPLResult& bestFit, const PriceData& priceData, RelativeErrorResult& result) {
    int n = priceData.date.size();
    result.date = priceData.date;
    result.relativeError.resize(n);

    for (int i = 0; i < n; ++i) {
        double x = priceData.date[i];
        double y = priceData.price[i];
        double Y = bestFit.a1 + (bestFit.a2 * pow(bestFit.a3 - x, bestFit.a4)) * (1 + bestFit.a5 * cos(bestFit.a6 * log(bestFit.a3 - x) + bestFit.a7));
        double e = Y - y;
        double re = e / y;
        result.relativeError[i] = re;
    }
}

void saveRelativeErrorTable(const RelativeErrorResult& result, const std::string& outputpath) {
    std::ofstream file(outputpath + "/RELATIVEERROR_table.csv");
    file << "Datetime,Relative Error\n";
    int n = result.date.size();
    for (int i = 0; i < n; ++i) {
        file << result.date[i] << "," << result.relativeError[i] << "\n";
    }
    file.close();
}

int main() {
    std::string outputpath = "path/to/output";  // Replace with the desired output path

    // Read LPPL results from file
    std::ifstream lpplFile(outputpath + "/LPPL_table.csv");
    std::vector<LPPLResult> lpplResults;
    std::string line;
    std::getline(lpplFile, line);  // Skip header
    while (std::getline(lpplFile, line)) {
        std::istringstream iss(line);
        LPPLResult result;
        iss >> result.a1 >> result.a2 >> result.a3 >> result.a4 >> result.a5 >> result.a6 >> result.a7 >> result.a8;
        lpplResults.push_back(result);
    }
    lpplFile.close();

    // Read price data from file
    std::ifstream priceFile(outputpath + "/pricedata.csv");
    PriceData priceData;
    std::getline(priceFile, line);  // Skip header
    while (std::getline(priceFile, line)) {
        std::istringstream iss(line);
        double date, price;
        iss >> date >> price;
        priceData.date.push_back(date);
        priceData.price.push_back(price);
    }
    priceFile.close();

    // Find the best fit LPPL result
    LPPLResult bestFit = findBestFit(lpplResults);

    // Calculate relative error
    RelativeErrorResult relativeError;
    calculateRelativeError(bestFit, priceData, relativeError);

    // Save relative error table
    saveRelativeErrorTable(relativeError, outputpath);

    return 0;
}
