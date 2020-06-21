#include <bits/stdc++.h>
#include <limits>

using namespace std;


int sum(const std::vector<std::vector<int>>& arr, const int row, const int col) {
    int sum{0};
    // hardcode the hour-glass structure
    sum+=arr[row][col];sum+=arr[row][col+1];sum+=arr[row][col+2];
    sum+=arr[row+1][col+1];
    sum+=arr[row+2][col];sum+=arr[row+2][col+1];sum+=arr[row+2][col+2];
    return sum;
}


// Complete the hourglassSum function below.
int hourglassSum(const vector<vector<int>>& arr) {
    int max_sum{std::numeric_limits<int>::min()};
    for (int row = 0; row < arr.size()-2; ++row) {
        for (int col = 0; col < arr[0].size()-2; ++col) {
            max_sum = std::max(max_sum, sum(arr, row, col));
        }
    }
    return max_sum;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    vector<vector<int>> arr(6);
    for (int i = 0; i < 6; i++) {
        arr[i].resize(6);

        for (int j = 0; j < 6; j++) {
            cin >> arr[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = hourglassSum(arr);

    fout << result << "\n";

    fout.close();

    return 0;
}
