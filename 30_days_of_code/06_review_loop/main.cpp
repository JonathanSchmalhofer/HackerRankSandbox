#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */

    int T;
    string T_string;
    //std::cin >> T;
    std::getline(std::cin, T_string);
    T = std::stoi(T_string);

    //std::cout << "T is: " << T << std::endl;

    std::vector<std::string> all_S;
    all_S.resize(T);

    for (int i = 0; i < T; ++i) {
        std::getline(std::cin, all_S[i]);
        //std::cout << "S[" << i << "] = " << all_S[i] << std::endl;
    }

    for (const auto& S : all_S) {
        for (int i = 0; i < S.size(); ++(++i)) {
            std::cout << S[i];
        }
        std::cout << " ";
        for (int i = 1; i < S.size(); ++(++i)) {
            std::cout << S[i];
        }
        std::cout << std::endl;
    }

    return 0;
}
