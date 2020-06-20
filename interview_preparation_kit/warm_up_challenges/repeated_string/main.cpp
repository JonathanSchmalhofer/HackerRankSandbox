#include <bits/stdc++.h>

using namespace std;

// Complete the repeatedString function below.
long repeatedString(string s, long n) {
    long occurences{0};
    long count_in_s{0};
    for (const auto& c : s) {
        if (c == 'a') {
            ++count_in_s;
        }
    }
    long full_strings{n/static_cast<long>(s.size())};
    occurences += full_strings * count_in_s;
    long end_pos_in_s = n % static_cast<long>(s.size());
    if (end_pos_in_s > 0) {
        for (long i = 0; i < end_pos_in_s; ++i ) {
            if (s[i] == 'a') {
                ++occurences;
            }
        }
    }
    return occurences;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string s;
    getline(cin, s);

    long n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    long result = repeatedString(s, n);

    fout << result << "\n";

    fout.close();

    return 0;
}
