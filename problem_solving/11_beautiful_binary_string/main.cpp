#include <bits/stdc++.h>

using namespace std;

int count_changes_needed(string b) {
    int changes{0};
    for (size_t i = 0; i < b.size()-2; ++i) {
        if (b.at(i) == '0' && b.at(i+1) == '1' && b.at(i+2) == '0') {
            ++changes;
            i+=2;
        }
    }
    return changes;
}

// Complete the beautifulBinaryString function below.
int beautifulBinaryString(string b) {
    int sum_start_left{0}, sum_start_right{0};
    sum_start_left = count_changes_needed(b);
    reverse(b.begin(),b.end());
    sum_start_right = count_changes_needed(b);
    return min(sum_start_left, sum_start_right);
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string b;
    getline(cin, b);

    int result = beautifulBinaryString(b);

    fout << result << "\n";

    fout.close();

    return 0;
}
