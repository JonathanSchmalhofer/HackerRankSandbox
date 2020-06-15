#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);

/*
 * Complete the 'getTotalX' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY a
 *  2. INTEGER_ARRAY b
 */

 std::int32_t gcd(std::int32_t a, std::int32_t b) {
    if(b == 0)
        return a;
    else return gcd(b, a % b);
}

std::int32_t gcd(std::vector<std::int32_t> arr) {
    auto n = static_cast<std::int32_t>(arr.size());
    std::int32_t result = arr[0];
    for (std::int32_t i = 1; i < n; i++) {
        result = gcd(arr[i], result);
  
        if(result == 1) {
           return result;
        } 
    }
    return result;
} 

std::int32_t lcm(std::vector<std::int32_t> arr) 
{ 
    auto n = static_cast<std::int32_t>(arr.size());
    std::int32_t ans = arr[0];
    for (std::int32_t i = 1; i < n; ++i) {
        ans = (((arr[i] * ans)) / (gcd(arr[i], ans)));
    }
    return ans;
}

std::int32_t getTotalX(vector<std::int32_t> a, vector<std::int32_t> b) {
    auto lcm_a = lcm(a);
    auto gcd_b = gcd(b);
    std::int32_t counter{0};
    std::int32_t denominator{lcm_a};
    while (denominator <= gcd_b) {
        if (std::fmod(gcd_b, denominator) == 0) {
            ++counter;
        }
        denominator += lcm_a;
    }
    return counter;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string first_multiple_input_temp;
    getline(cin, first_multiple_input_temp);

    vector<string> first_multiple_input = split(rtrim(first_multiple_input_temp));

    std::int32_t n = stoi(first_multiple_input[0]);

    std::int32_t m = stoi(first_multiple_input[1]);

    string arr_temp_temp;
    getline(cin, arr_temp_temp);

    vector<string> arr_temp = split(rtrim(arr_temp_temp));

    vector<std::int32_t> arr(n);

    for (std::int32_t i = 0; i < n; i++) {
        std::int32_t arr_item = stoi(arr_temp[i]);

        arr[i] = arr_item;
    }

    string brr_temp_temp;
    getline(cin, brr_temp_temp);

    vector<string> brr_temp = split(rtrim(brr_temp_temp));

    vector<std::int32_t> brr(m);

    for (std::int32_t i = 0; i < m; i++) {
        std::int32_t brr_item = stoi(brr_temp[i]);

        brr[i] = brr_item;
    }

    std::int32_t total = getTotalX(arr, brr);

    fout << total << "\n";

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
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
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
