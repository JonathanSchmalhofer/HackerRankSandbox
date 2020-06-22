#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;


vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y && x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */ 
    long n;
    std::string line;
    std::getline(std::cin, line);
    n = stol(line);
    std::map<std::string, std::string> phone_book;
    for (long i = 0; i < n; ++i) {
        std::getline(std::cin, line);
        auto line_tokens = split_string(line);
        phone_book.insert(std::make_pair(line_tokens[0], line_tokens[1]));
    }
    for (std::string line; std::getline(std::cin, line); ) {
        if (phone_book.count(line) != 0) {
            std::cout << line << "=" << phone_book[line] << std::endl;
        }
        else {
            std::cout << "Not found" << std::endl;
        }
    }
    return 0;
}
