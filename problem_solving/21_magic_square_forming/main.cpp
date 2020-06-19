//#include <bits/stdc++.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

std::vector<std::vector<int>> Difference(const std::vector<std::vector<int>>& lhs, const std::vector<std::vector<int>>& rhs) {
    int n = static_cast<int>(lhs.size());
    std::vector<std::vector<int>> diff;
    diff.resize(n);
    for (int row = 0; row < n; ++row) {
        diff[row].resize(n);
        for (int col = 0; col < n; ++col) {
            diff[row][col] = lhs[row][col] - rhs[row][col];
        }
    }
    return diff;
}

int AbsoluteSum(const std::vector<std::vector<int>>& m) {
    int sum{0};
    int n = static_cast<int>(m.size());
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            sum += std::abs(m[row][col]);
        }
    }
    return sum;
}

std::vector<std::vector<int>> ConvertToMatrix(const std::vector<int>& array) {
    std::vector<std::vector<int>> matrix;
    int n = static_cast<int>(std::sqrt(array.size()));
    int i{0};
    matrix.resize(n);
    for (int row = 0; row < n; ++row) {
        matrix[row].resize(n);
        for (int col = 0; col < n; ++col) {
            matrix[row][col] = array[i];
            ++i;
        }
    }
    return matrix;
}


int SumCol(const std::vector<std::vector<int>>& s, const int col) {
    int sum{0};
    for (int row = 0; row < s.size(); ++row) {
        sum += s[row][col];
    }
    return sum;
}

int SumRow(const std::vector<std::vector<int>>& s, const int row) {
    int sum{0};
    for (int col = 0; col < s.size(); ++col) {
        sum += s[row][col];
    }
    return sum;
}

bool IsMagicSquare(const std::vector<std::vector<int>>& s) {
    // Use first row as reference
    int magic_constant = SumRow(s, 0);

    // check all rows
    for (int row = 0; row < s.size(); ++row) {
        int row_sum = SumRow(s, row);
        if (row_sum != magic_constant) {
            return false;
        }
    }
    // check all cols
    for (int col = 0; col < s.size(); ++col) {
        int col_sum = SumCol(s, col);
        if (col_sum != magic_constant) {
            return false;
        }
    }
	// check main diagonal
	int diag_sum{ 0 };
	for (int row = 0; row < s.size(); ++row) {
		if (s[row].size() == s.size()) {
			diag_sum += s[row][row];
		}
	}
	if (diag_sum != magic_constant) {
		return false;
	}
	// check anti diagonal
	diag_sum = 0;
	for (int row = 0; row < s.size(); ++row) {
		if (s[row].size() == s.size()) {
			diag_sum += s[row][s[row].size()-row-1];
		}
	}
	if (diag_sum != magic_constant) {
		return false;
	}

    return true;
}

inline bool operator==(const std::vector<std::vector<int>>& lhs, const std::vector<std::vector<int>>& rhs) {
	if (lhs.size() != rhs.size()) {
		return false;
	}
	for (int row = 0; row < lhs.size(); ++row) {
		if (lhs[row].size() != rhs[row].size()) {
			return false;
		}
		for (int col = 0; col < lhs[row].size(); ++col) {
			if (lhs[row][col] != rhs[row][col]) {
				return false;
			}
		}
	}
	return true;
}

std::vector<std::vector<std::vector<int>>> FindAllMagicSquaresBruteForce(const int n) {
    std::vector<int> array(n*n);
    std::vector<std::vector<std::vector<int>>>  magic_squares{};

	std::vector<std::vector<int>> square;

	
    // fill with range [1;n*n]
    for (int i = 0; i < n*n; ++i) {
        array[i] = i+1;
    }

    // check all possible combinations - super brute force
    do {
        if (IsMagicSquare(ConvertToMatrix(array))) { 
            magic_squares.push_back(ConvertToMatrix(array)); 
        } 
    } while (std::next_permutation(array.begin(), array.end()));
	//magic_squares.erase(std::unique(magic_squares.begin(), magic_squares.end()), magic_squares.end());
    return magic_squares;
}


// Complete the formingMagicSquare function below.
int formingMagicSquare(vector<vector<int>> s) {
    auto candidates = FindAllMagicSquaresBruteForce(s.size());

    int min_sum{std::numeric_limits<int>::max()};
    
    for (const auto& c : candidates) {
        int cost = AbsoluteSum(Difference(c, s));
        min_sum = std::min(min_sum, cost);
    }
    return min_sum;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    vector<vector<int>> s(3);
    for (int i = 0; i < 3; i++) {
        s[i].resize(3);

        for (int j = 0; j < 3; j++) {
            cin >> s[i][j];
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int result = formingMagicSquare(s);

	std::cout << result << std::endl;

    fout << result << "\n";

    fout.close();

    return 0;
}
