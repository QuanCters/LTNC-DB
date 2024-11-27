#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>

struct ChuyenKhoan
{
    std::string date_time;
    int trans_no;
    int credit;
    int debit;
    std::string detail;
};

std::string removeQuotes(const std::string &s)
{
    std::string result = s;

    // Remove leading quote if present
    if (!result.empty() && result.front() == '"')
    {
        result.erase(result.begin());
    }

    // Remove trailing quote if present
    if (!result.empty() && result.back() == '"')
    {
        result.pop_back();
    }

    return result;
}

std::vector<ChuyenKhoan> readCSVAndBuildVector(const std::string &filename)
{
    std::ifstream file(filename);
    std::string line;
    std::vector<ChuyenKhoan> data; // Vector to store ChuyenKhoan structs

    // Check if the file is open
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }

    // Skip the first line
    std::getline(file, line);

    // Read each line from the CSV file
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string value;
        ChuyenKhoan record; // Create a ChuyenKhoan struct to hold the data

        // Split the line by commas
        std::getline(ss, value, ','); // Read date_time
        record.date_time = removeQuotes(value);
        std::getline(ss, value, ','); // Read trans_no
        record.trans_no = std::stoi(removeQuotes(value));
        std::getline(ss, value, ','); // Read credit
        record.credit = std::stoi(removeQuotes(value));
        std::getline(ss, value, ','); // Read debit
        record.debit = std::stoi(removeQuotes(value));
        std::getline(ss, value, ','); // Read detail
        record.detail = removeQuotes(value);

        // Add the record to the data vector
        data.push_back(record);
    }

    file.close();

    return data;
}

std::vector<ChuyenKhoan> rangeSearchVector(
    const std::vector<ChuyenKhoan> &data,
    int lower_bound_val,
    int upper_bound_val)
{
    std::vector<ChuyenKhoan> results;

    // Find the iterator to the first element not less than lower_bound_val
    auto it_lower = std::lower_bound(data.begin(), data.end(), lower_bound_val,
                                     [](const ChuyenKhoan &a, int b)
                                     { return a.credit < b; });
    // Find the iterator to the first element greater than upper_bound_val
    auto it_upper = std::upper_bound(data.begin(), data.end(), upper_bound_val,
                                     [](int a, const ChuyenKhoan &b)
                                     { return a < b.credit; });

    // Iterate from it_lower to it_upper
    for (auto it = it_lower; it != it_upper; ++it)
    {
        results.push_back(*it);
    }

    return results;
}

int main(int argc, char *argv[])
{
    // Check if the correct number of arguments is provided
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <lower_key> <upper_key>" << std::endl;
        return 1; // Exit with an error code
    }

    // Parse the command-line arguments
    int lower_key = std::stoi(argv[1]);
    int upper_key = std::stoi(argv[2]);

    auto start = std::chrono::high_resolution_clock::now();

    std::string filename = "chuyen_khoan.csv";
    std::vector<ChuyenKhoan> data = readCSVAndBuildVector(filename);

    std::sort(data.begin(), data.end(), [](const ChuyenKhoan &a, const ChuyenKhoan &b)
              {
                  return a.credit < b.credit; // Sort by credit in ascending order
              });

    std::vector<ChuyenKhoan> results = rangeSearchVector(data, lower_key, upper_key);

    std::cout << "Total count: " << results.size() << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Program execution time: " << duration.count() << " seconds." << std::endl;

    return 0;
}