#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

long long baseToDecimal(string value, int base) {
    long long result = 0;
    for (char c : value) {
        result = result * base + (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10);
    }
    return result;
}

long long lagrangeInterpolation(vector<pair<long long, long long>>& points, int k) {
    long long result = 0;
    for (int i = 0; i < k; i++) {
        long long numerator = points[i].second;
        long long denominator = 1;
        for (int j = 0; j < k; j++) {
            if (i != j) {
                numerator *= -points[j].first;
                denominator *= (points[i].first - points[j].first);
            }
        }
        result += numerator / denominator;
    }
    return result;
}

long long solve(istream& input) {
    string line, content;
    while (getline(input, line)) content += line;
    
    // Extract k
    size_t kPos = content.find("\"k\":");
    kPos = content.find(":", kPos) + 1;
    while (content[kPos] == ' ') kPos++;
    int k = 0;
    while (content[kPos] >= '0' && content[kPos] <= '9') {
        k = k * 10 + (content[kPos] - '0');
        kPos++;
    }
    
    vector<pair<long long, long long>> points;
    
    // Parse each numbered key
    for (int i = 1; i <= 10; i++) {
        string key = "\"" + to_string(i) + "\":";
        size_t pos = content.find(key);
        if (pos == string::npos) continue;
        
        // Find base
        size_t basePos = content.find("\"base\":", pos);
        if (basePos == string::npos || basePos > content.find("}", pos)) continue;
        basePos = content.find("\"", basePos + 7) + 1;
        int base = 0;
        while (content[basePos] >= '0' && content[basePos] <= '9') {
            base = base * 10 + (content[basePos] - '0');
            basePos++;
        }
        
        // Find value
        size_t valuePos = content.find("\"value\":", pos);
        valuePos = content.find("\"", valuePos + 8) + 1;
        string value = "";
        while (content[valuePos] != '\"') {
            value += content[valuePos];
            valuePos++;
        }
        
        points.push_back({i, baseToDecimal(value, base)});
    }
    
    return lagrangeInterpolation(points, k);
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        // Process files from command line arguments
        for (int i = 1; i < argc; i++) {
            ifstream file(argv[i]);
            cout << solve(file) << endl;
        }
    } else {
        // Read from standard input
        cout << solve(cin) << endl;
    }
    return 0;
}