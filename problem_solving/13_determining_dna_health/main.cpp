#include <bits/stdc++.h>
#include <limits>

//#define READ_FROM_FILE
//#define VERBOSE

// Start Implementation of Helper-Class using Aho-Corasick Automaton

#define ALPHABET_SIZE 26

// Forward declaration
class AhoCorasickDnaHealthAutomaton;
class TrieNode;
class AhoCorasickDnaHealthAutomaton {
 public:
    AhoCorasickDnaHealthAutomaton();
    ~AhoCorasickDnaHealthAutomaton() = default;
    void Reset();
    TrieNode* Insert(const std::string& s, const unsigned long long int health, const std::uint32_t gene_index);
    void FindFailureLinks();
    void PrintAllFailureLinks();
    unsigned long long int CalculateHealth(const std::string& d, const std::uint32_t first, const std::uint32_t last);
 private:
    unsigned long long int StepAndLookupHealth(const char c, const std::uint32_t first, const std::uint32_t last);

    TrieNode* m_root;
    TrieNode* m_state;
};

class TrieNode {
 public:
    TrieNode(const char c);
    ~TrieNode();
    TrieNode* GetChild(const char c);
    std::vector<TrieNode*> GetChildren();
    TrieNode* AddChild(const char c);
    std::uint32_t ChildrenSize();

    char m_value;
    std::vector<unsigned long long int> m_healths; // duplicates are allowed, thus we need a vector
    std::vector<std::uint32_t> m_gene_indices; // // duplicates are allowed, thus we need a vector
    bool m_is_leaf{false};
    TrieNode* m_suffix{nullptr};
    TrieNode* m_output{nullptr};
    std::vector<TrieNode*> m_children;
};

////////////////////////////////////////////
// AhoCorasick
AhoCorasickDnaHealthAutomaton::AhoCorasickDnaHealthAutomaton()
  : m_root(new TrieNode(' ')) {
    Reset();
}

void AhoCorasickDnaHealthAutomaton::Reset() {
    m_state = m_root;
}

TrieNode* AhoCorasickDnaHealthAutomaton::Insert(const std::string& s, const unsigned long long int health, const std::uint32_t gene_index) {
    TrieNode* node{m_root};
#ifdef VERBOSE
    std::cout << "  Inserting to Automaton: " << s << std::endl;
#endif
    for (auto c : s) {
        node = node->AddChild(c);
    }   
    node->m_is_leaf = true;
    node->m_healths.push_back(health);
    node->m_gene_indices.push_back(gene_index);
#ifdef VERBOSE
    std::cout << "    m_root.ChildrenSize() = " << m_root->ChildrenSize() << ", leaf.m_healths.size() = " << node->m_healths.size() << std::endl;
#endif 
    return node;
}

void AhoCorasickDnaHealthAutomaton::FindFailureLinks() {
    std::queue<TrieNode*> node_queue;
    m_root->m_suffix = nullptr;
    for (auto child : m_root->GetChildren()) {
        child->m_suffix = m_root;
        node_queue.push(child);
    }
    while (!node_queue.empty()) {
        auto current_node = node_queue.front();
        node_queue.pop();

        for (auto child : current_node->GetChildren()) {
            auto suffix_node = current_node->m_suffix;
            while (child->m_suffix == nullptr) {
                if (suffix_node->GetChild(child->m_value) != nullptr) {
                    child->m_suffix = suffix_node->GetChild(child->m_value);
                }
                else if (suffix_node == m_root) {
                    child->m_suffix = m_root;
                }
                else {
                    suffix_node = suffix_node->m_suffix;
                }
            }
            child->m_output = (child->m_suffix->m_is_leaf) ? child->m_suffix : child->m_suffix->m_output;
            node_queue.push(child);
        }
    }
}

void AhoCorasickDnaHealthAutomaton::PrintAllFailureLinks() {
    std::queue<TrieNode*> node_queue;
    m_root->m_suffix = nullptr;
    node_queue.push(m_root);
    while (!node_queue.empty()) {
        auto current_node = node_queue.front();
        node_queue.pop();
        for (auto child : current_node->GetChildren()) {
            node_queue.push(child);
        }
        std::cout << "Looking at node: " << current_node->m_value;
        if (current_node->m_suffix != nullptr) {
            std::cout << ", failure links points to: ";
            if (current_node->m_suffix == m_root) {
                std::cout << "root";
            }
            else {
                std::cout << current_node->m_suffix->m_value;
            }
        }
        std::cout << std::endl;
    }
}

unsigned long long int AhoCorasickDnaHealthAutomaton::StepAndLookupHealth(const char c, const std::uint32_t first, const std::uint32_t last) {
    unsigned long long int health_increment{0};

    // If the current State has no child c, we follow the failure links back to root. Else, we stay at that state.
    while (m_state->GetChild(c) == nullptr) {
        if (m_state == m_root) {
            break;
        }
        m_state = m_state->m_suffix;
    }

    // If a child c exists, that is our next state.
    auto next_state = m_state->GetChild(c);
    if (next_state != nullptr) {
        m_state = next_state;
    }

    for (auto out = m_state; out != nullptr; out = out->m_output) {
        if (out->m_is_leaf) {
            for (auto i = std::lower_bound(out->m_gene_indices.begin(), out->m_gene_indices.end(), first);
                 i != out->m_gene_indices.end() && *i <= last;
                 ++i) {
                health_increment += out->m_healths[std::distance(out->m_gene_indices.begin(), i)];
            }
        }
    }

    return health_increment;
}

unsigned long long int AhoCorasickDnaHealthAutomaton::CalculateHealth(const std::string& d, const std::uint32_t first, const std::uint32_t last) {
    unsigned long long int total_health{0};
    for (const auto c : d) {
		total_health += StepAndLookupHealth(c, first, last);
	}
	Reset(); // don't forget to reset the state
    return total_health;
}

////////////////////////////////////////////
// TrieNode
TrieNode::TrieNode(const char character)
  : m_value(character), m_children(ALPHABET_SIZE, nullptr) {
}

TrieNode::~TrieNode() {
    for (auto& child : m_children) {
        delete child;
    }
}

TrieNode* TrieNode::GetChild(const char c) {
    char idx = c -'a'; // ASCII idx : a <- 0, b <- 1, ....
    if (idx < ALPHABET_SIZE && idx >= 0) {
        return m_children[idx];
    } else {
        return nullptr;
    }
}

TrieNode* TrieNode::AddChild(const char c) {
    TrieNode* &child{m_children[c - 'a']};
    if (child == nullptr) {
        child = new TrieNode(c);
    }
    return child;
}

std::vector<TrieNode*> TrieNode::GetChildren() {
    std::vector<TrieNode*> existing_children;
    for (auto child : m_children) {
        if (child != nullptr) {
            existing_children.push_back(child);
        }
    }
    return existing_children;
}

std::uint32_t TrieNode::ChildrenSize() {
    auto existing_children = GetChildren();
    return static_cast<std::uint32_t>(existing_children.size());
}


////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> SplitString(std::string);

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
    for (std::uint32_t i = 0; i < n; ++i) {
        std::string genes_item = genes_temp[i];
        genes.push_back(std::move(genes_item));
    }

    // health[]
    std::getline(textfile, line);
    std::vector<std::string> health_temp = SplitString(line);
    for (std::uint32_t i = 0; i < n; ++i) {
        unsigned long long int health_item = stoi(health_temp[i]);
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
    for (std::uint32_t i = 0; i < n; ++i) {
        std::string genes_item = genes_temp[i];
        genes.push_back(std::move(genes_item));
    }

    // health[]
    //std::cout << "Enter genes (n space separated numbers)" << std::endl;
    std::getline(std::cin, line);
    std::vector<std::string> health_temp = SplitString(line);
    for (std::uint32_t i = 0; i < n; ++i) {
        unsigned long long int health_item = stoi(health_temp[i]);
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

    AhoCorasickDnaHealthAutomaton automaton{};
    for (std::uint32_t idx{0}; idx < n; ++idx) {
        automaton.Insert(genes[idx], health[idx], idx);
    }
    automaton.FindFailureLinks();
#ifdef VERBOSE
    automaton.PrintAllFailureLinks();
#endif

    // Now iterate s times over all DNA strands
    for (std::uint32_t s_itr = 0; s_itr < s; ++s_itr) {
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

        unsigned long long int total_health = automaton.CalculateHealth(d, first, last);
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
