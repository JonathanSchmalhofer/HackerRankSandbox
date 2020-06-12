#include <bits/stdc++.h>
#include <limits>

#define READ_FROM_FILE

// Start Implementation Aho-Corasick

// Forward declaration
class AhcDnaCalculator;
class TrieNode;
class AhcDnaCalculator {
 public:
    AhcDnaCalculator();
    ~AhcDnaCalculator();
    void Reset();
    void Feed();
 private:
    void FindLinks();
    void Insert();

    TrieNode* m_root;
};

class TrieNode {
 public:
    TrieNode();
    ~TrieNode();
    TrieNode* GetChild();
    std::vector<TrieNode*> GetChildren();
    TrieNode* AddChild();

    char value;
    unsigned long long int m_health;
    unsigned long long int m_index;
    bool m_leaf{false};
    TrieNode* m_suffix{nullptr};
    TrieNode* m_output{nullptr};
    std::vector<TrieNode*> m_children;
};

// End Implementation Aho-Corasick

std::vector<std::string> SplitString(std::string);

unsigned long long int CalculateHealth(const std::string& d, const std::uint32_t first, const std::uint32_t last, const std::vector<std::string>& genes, const std::vector<unsigned long long int>& health) {
    unsigned long long int total_health{0};
    
    return total_health;
}

#ifdef READ_FROM_FILE
void GetInputs(std::uint32_t& n, std::uint32_t&s, std::vector<unsigned long long int>& health, std::vector<std::string>& genes, std::ifstream& textfile) {
    std::string line;

    // n
    std::getline(textfile, line);
    n = stoi(line);

    // genes[]
    std::getline(textfile, line);
    std::vector<std::string> genes_temp = SplitString(line);
    genes.clear();
    for (int i = 0; i < n; i++) {
        std::string genes_item = genes_temp[i];
        genes.push_back(std::move(genes_item));
    }

    // health[]
    std::getline(textfile, line);
    std::vector<std::string> health_temp = SplitString(line);
    for (int i = 0; i < n; i++) {
        int health_item = stoi(health_temp[i]);
        health[i] = health_item;
    }

    // s
    std::getline (textfile, line);
    s = stoi(line);
}
#else
void GetInputs(std::uint32_t& n, std::uint32_t&s, std::vector<unsigned long long int>& health, std::vector<std::string>& genes) {
    std::string line;

    // n
    //std::cout << "Enter n" << std::endl;
    std::cin >> n;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // genes[]
    //std::cout << "Enter genes (n space separated strings)" << std::endl;
    std::getline(std::cin, line);
    std::vector<std::string> genes_temp = SplitString(line);
    for (int i = 0; i < n; i++) {
        std::string genes_item = genes_temp[i];
        genes.push_back(std::move(genes_item));
    }

    // health[]
    //std::cout << "Enter genes (n space separated numbers)" << std::endl;
    std::getline(std::cin, line);
    std::vector<std::string> health_temp = SplitString(line);
    for (int i = 0; i < n; i++) {
        int health_item = stoi(health_temp[i]);
        health[i] = health_item;
    }

    // s
    //std::cout << "Enter s" << std::endl;
    std::cin >> s;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
#endif



int main()
{
    // Init with sizes from contraints from https://www.hackerrank.com/challenges/determining-dna-health/problem
    std::uint32_t n{100000};
    std::uint32_t s{100000};
    std::uint32_t first{0U};
    std::uint32_t last{0U};
    std::vector<unsigned long long int> health;
    std::vector<std::string> genes;
    health.reserve(100000); // Note: reserve means still, that size() == 0
    genes.reserve(100000);
    unsigned long long int min_health{std::numeric_limits<unsigned long long int>::max()};
    unsigned long long int max_health{std::numeric_limits<unsigned long long int>::min()};

#ifdef READ_FROM_FILE
    std::ifstream textfile{"input00.txt"};
    GetInputs(n, s, health, genes, textfile);
#else
    GetInputs(n, s, health, genes);
#endif

    // Now iterate s times over all DNA strands
    for (int s_itr = 0; s_itr < s; s_itr++) {
        std::string first_last_d_line;
#ifdef READ_FROM_FILE
        std::getline(textfile, first_last_d_line);
#else
        std::getline(std::cin, first_last_d_line);
#endif

        std::vector<std::string> first_last_d = SplitString(first_last_d_line);
        first = stoi(first_last_d[0]);
        last = stoi(first_last_d[1]);
        std::string d = first_last_d[2];

        unsigned long long int total_health = CalculateHealth(d, first, last, genes, health);
        min_health = std::min(min_health, total_health);
        max_health = std::max(max_health, total_health);
    }

    std::cout << min_health << " " << max_health << std::endl;

#ifdef READ_FROM_FILE
    textfile.close();
#endif

    return 0;
}

std::vector<std::string> SplitString(std::string input_string) {
    std::string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    std::vector<std::string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != std::string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, std::min(pos, input_string.length()) - i + 1));

    return splits;
}
