#include <bits/stdc++.h>

using namespace std;

// Complete the countingValleys function below.
int countingValleys(int n, string s) {
    std::vector<int> height_map(n);
    int height{0};
    for (int i = 0; i < n; ++i) {
        if (s[i]=='U') {
            ++height;
        }
        else {
            --height;
        }
        //std::cout << height << " ";
        height_map[i] = height;
    }
    //std::cout << std::endl;

    int num_valleys{0};

    for (int i = 1; i < n; ++i) {
        if (height_map[i] == 0 && height_map[i-1] < 0) {
            ++num_valleys;
        }
    }
    return num_valleys;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string s;
    getline(cin, s);

    int result = countingValleys(n, s);

    fout << result << "\n";

    fout.close();

    return 0;
}
