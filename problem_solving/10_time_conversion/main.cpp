#include <bits/stdc++.h>

using namespace std;

/*
 * Complete the timeConversion function below.
 */

void addAfternoonHours(string& s) {
    bool is_afternoon = s.find("PM") != std::string::npos;
    bool is_twelve = stoi(s.substr(0,2)) == 12;
    if (is_afternoon && !is_twelve) {
        string military_hours = to_string(stoi(s.substr(0,2)) + 12);
        s.replace(0, 2, military_hours);
    }
}

void handleMidnight(string& s) {
    bool is_morning = s.find("AM") != std::string::npos;
    bool is_twelve = stoi(s.substr(0,2)) == 12;
    if (is_morning && is_twelve) {
        s.replace(0, 2, "00");
    }
}

void removeAmPm(string& s) {
    s.replace(s.end()-2, s.end(), "");
}

string timeConversion(string s) {
    /*
     * Write your code here.
     */
    addAfternoonHours(s);
    handleMidnight(s);
    removeAmPm(s);
    return s;

}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string s;
    getline(cin, s);

    string result = timeConversion(s);

    fout << result << "\n";

    fout.close();

    return 0;
}
