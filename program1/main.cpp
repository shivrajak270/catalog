#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// Function to decode the value based on the base
long long decodeValue(const string& value, int base) {
    return stoll(value, nullptr, base); // Convert string to long long using the base
}

// Function to perform Lagrange Interpolation and calculate 'c'
double lagrangeInterpolation(const vector<pair<int, long long>>& points, int k) {
    double c = 0.0;
    int size = points.size();

    for (int i = 0; i < size; ++i) {
        double term = points[i].second;
        for (int j = 0; j < size; ++j) {
            if (i != j) {
                term = term * (0 - points[j].first) / (points[i].first - points[j].first);
            }
        }
        c += term;
    }

    return c;
}

int main() {
    // Open the JSON file
    ifstream file("input.json");
    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        return 1;
    }

    // Parse the JSON file
    json j;
    file >> j;

    // Get the number of roots (n) and minimum number required (k)
    int n = j["keys"]["n"].get<int>(); // Extract as int
    int k = j["keys"]["k"].get<int>(); // Extract as int

    // Store the points (x, y) after decoding
    vector<pair<int, long long>> points;

    for (const auto& item : j.items()) {
        if (item.key() != "keys") { // Skip the keys object
            int x = stoi(item.key()); // Convert key to int (x)

            // Get base and decode y value
            int base = item.value()["base"].is_string() ? stoi(item.value()["base"].get<string>()) : item.value()["base"].get<int>();
            string encodedValue = item.value()["value"].get<string>();
            long long decodedY = decodeValue(encodedValue, base);

            // Store the decoded point (x, y)
            points.push_back({x, decodedY});
        }
    }

    // Perform Lagrange Interpolation to find the constant term 'c'
    double c = lagrangeInterpolation(points, k);

    // Output the result
    cout << "Constant term (c) = " << c << endl;

    return 0;
}
