#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

int main() {
    int i = 4;
    double d = 4.0;
    string s = "HackerRank ";

    
    // Declare second integer, double, and String variables.
    int own_i;
    double own_d;
    string own_s;
    
    // Read and save an integer, double, and String to your variables.
    // Note: If you have trouble reading the entire string, please go back and review the Tutorial closely.
    cin >> own_i;
    cin >> own_d;
    getline(cin >> ws, own_s);
    
    // Print the sum of both integer variables on a new line.
    cout << (i + own_i) << endl;
    
    // Print the sum of the double variables on a new line.
    cout << fixed << setprecision(1) << (d + own_d) << endl;
    
    // Concatenate and print the String variables on a new line
    // The 's' variable above should be printed first.
    cout << (s + own_s) << endl;

    return 0;