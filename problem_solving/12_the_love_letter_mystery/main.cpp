#include <bits/stdc++.h>
#include <limits>

using namespace std;

int count_changes_to_palindrom(string s) {
    int changes{0};
    for (size_t idx = 0; idx < s.length(); ++idx) {
        auto current_letter = s.at(idx);
        auto mirror_letter = s.at(s.length() - idx - 1);
        cout << current_letter << "--" << mirror_letter;
        // Rule 1
        int change_cost{0};
        if (int(current_letter) < int(mirror_letter)) {
            cout << " is not allowed" << endl;
            
        } else {
            change_cost = int(current_letter) - int(mirror_letter);
        }
        cout << " costs " << to_string(change_cost) << endl;
        changes += change_cost;
    }
    cout << "   Total Cost: " << changes << endl;
    return changes;
}

// Complete the theLoveLetterMystery function below.
int theLoveLetterMystery(string s) {
    cout << "Input: " << s << endl;
    int changes_left = count_changes_to_palindrom(s);
    cout << "---" << endl;
    reverse(s.begin(), s.end());
    cout << "Reversed: " << s << endl;
    int changes_right = count_changes_to_palindrom(s);
    cout << "=================" << endl;
    return min(changes_left, changes_right);
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int q;
    cin >> q;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int q_itr = 0; q_itr < q; q_itr++) {
        string s;
        getline(cin, s);

        int result = theLoveLetterMystery(s);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}
