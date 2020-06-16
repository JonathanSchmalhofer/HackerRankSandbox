#include <bits/stdc++.h>

using namespace std;

// Complete the solve function below.
void solve(double meal_cost, int tip_percent, int tax_percent) {
    double total_cost{0.0F};
    int rounded_total_cost{0};

    total_cost = meal_cost * (1.0F + static_cast<double>(tip_percent)/100 + static_cast<double>(tax_percent)/100);
    rounded_total_cost = static_cast<int>(round(total_cost));
    std::cout << rounded_total_cost << std::endl;
}

int main()
{
    double meal_cost;
    cin >> meal_cost;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int tip_percent;
    cin >> tip_percent;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int tax_percent;
    cin >> tax_percent;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    solve(meal_cost, tip_percent, tax_percent);

    return 0;
}
