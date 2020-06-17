#include <bits/stdc++.h>

using namespace std;

/*
If  is odd, print Weird
If  is even and in the inclusive range of 2 to 5, print Not Weird
If  is even and in the inclusive range of 6 to 20, print Weird
If  is even and greater than 20, print Not Weird
*/
std::string evaluate(const int n) {
    if (n % 2 != 0) {
        return std::string{"Weird"};
    }
    else {
        if (2 <= n && n <= 5) {
            return std::string{"Not Weird"};
        }
        else if (6 <= n && n <= 20) {
            return std::string{"Weird"};
        }
        else {
            return std::string{"Not Weird"};
        }
    }
}


int main()
{
    int N;
    cin >> N;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    std::cout << evaluate(N) << std::endl;

    return 0;
}
