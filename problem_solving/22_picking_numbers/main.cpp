//#include <bits/stdc++.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <limits>
#include <functional>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'pickingNumbers' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY a as parameter.
 */

int CountOccurences(const std::vector<int>& a) {
    int occurences{0};
    for (int i = 1; i < a.size(); ++i) {
        if (std::abs(a[i]-a[0]) <= 1) {
            ++occurences;
        }
    }
    return occurences;
}

int pickingNumbersBruteForce(vector<int> a) {
    int max_numbers{0};
    do {
        max_numbers = std::max(max_numbers, CountOccurences(a));
    } while (std::next_permutation(a.begin(), a.end()));
    return max_numbers;
}

int pickingNumbers(vector<int> a) {
	int max_numbers{0};
	int max_entry = *std::max_element(a.begin(), a.end());
	std::vector<int> sorted_occurence_counter(max_entry+1);
	for (const auto& entry : a) {
		++sorted_occurence_counter[entry];
	}
	for (int i = 1; i < sorted_occurence_counter.size(); ++i) {
		max_numbers = std::max(max_numbers, sorted_occurence_counter[i - 1] + sorted_occurence_counter[i]);
	}
	return max_numbers;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string n_temp;
    getline(cin, n_temp);
	//n_temp = "6";

    int n = stoi(ltrim(rtrim(n_temp)));

    string a_temp_temp;
    getline(cin, a_temp_temp);
	//a_temp_temp = "4 6 5 3 3 1";

    vector<string> a_temp = split(rtrim(a_temp_temp));

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        int a_item = stoi(a_temp[i]);

        a[i] = a_item;
    }

    int result = pickingNumbers(a);

    fout << result << "\n";

    fout.close();

    return 0;
}

string ltrim(const string &str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string &str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), std::not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string &str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
