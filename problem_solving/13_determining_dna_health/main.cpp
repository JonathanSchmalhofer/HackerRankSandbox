#include <bits/stdc++.h>
#include <limits>

using namespace std;

vector<string> split_string(string);

long long int calculate_health(string d, int first, int last, vector<string> genes, vector<int> health) {
    long long int total_health{0};

    assert(first <= last);
    assert(last < int(genes.size()));
    //cout << "We look at strand d = " << d << endl;
    for (int i = first; i <= last; ++i) {
        auto g_i = genes[i];
        //cout << "  beneficial i = " << i << ", " << g_i << endl;
        // Repeat till end is reached
        size_t pos = 0;
        size_t found_position = d.find(g_i);
        while (found_position!= std::string::npos) {
            auto sub_string = d.substr(pos,d.size()-pos);
            size_t skipped = d.size()-sub_string.size();
            //cout << "    Looking only at " << sub_string << endl;
            found_position = sub_string.find(g_i);
            if (found_position != std::string::npos) {
                //cout << "    FOUND AT " << found_position << endl;
                total_health += health[i];
                pos = found_position + skipped + 1;
            }
        }
    }

    return total_health;
}

int main()
{
    string line; // self added
    ifstream myfile ("input00.txt"); // self added

    int n;
    //cin >> n;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline (myfile,line); // self added
    n = stoi(line); // self added

    string genes_temp_temp;
    //getline(cin, genes_temp_temp);
    getline(myfile, genes_temp_temp);

    vector<string> genes_temp = split_string(genes_temp_temp);

    vector<string> genes(n);

    for (int i = 0; i < n; i++) {
        string genes_item = genes_temp[i];

        genes[i] = genes_item;
    }

    string health_temp_temp;
    //getline(cin, health_temp_temp);
    getline(myfile, health_temp_temp);

    vector<string> health_temp = split_string(health_temp_temp);

    vector<int> health(n);

    for (int i = 0; i < n; i++) {
        int health_item = stoi(health_temp[i]);

        health[i] = health_item;
    }

    int s;
    //cin >> s;
    //cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline (myfile,line); // self added
    s = stoi(line); // self added

    long long int min_health{std::numeric_limits<long long int>::max()}, max_health{0};

    for (int s_itr = 0; s_itr < s; s_itr++) {
        string firstLastd_temp;
        //getline(cin, firstLastd_temp);
        getline(myfile, firstLastd_temp);

        vector<string> firstLastd = split_string(firstLastd_temp);

        int first = stoi(firstLastd[0]);

        int last = stoi(firstLastd[1]);

        string d = firstLastd[2];

        long long int total_health = calculate_health(d, first, last, genes, health);
        
        min_health = min(min_health, total_health);
        max_health = max(max_health, total_health);

    }

    cout << min_health << " " << max_health << endl;

    myfile.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
