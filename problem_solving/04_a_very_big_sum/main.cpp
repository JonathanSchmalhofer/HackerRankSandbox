#include <bits/stdc++.h>
#include <numeric>

using namespace std;

vector<string> split_string(string);

// Complete the aVeryBigSum function below.
long long int aVeryBigSum(vector<long long int> ar) {
    //static_cast
    long long int sum{0};
    for (const auto val : ar) {
        sum += val;
    }
    return sum;
    //return std::accumulate(ar.begin(), ar.end(), 0);
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int ar_count;
    cin >> ar_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string ar_temp_temp;
    getline(cin, ar_temp_temp);

    vector<string> ar_temp = split_string(ar_temp_temp);

    vector<long long int> ar(ar_count);

    for (int i = 0; i < ar_count; i++) {
        long long int ar_item = stol(ar_temp[i]);

        ar[i] = ar_item;
    }

    long long int result = aVeryBigSum(ar);

    fout << result << "\n";

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
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
