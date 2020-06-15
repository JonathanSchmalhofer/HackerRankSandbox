#include <bits/stdc++.h>
#include <limits>

#define READ_FROM_FILE
//#define VERBOSE
//#define DEBUG_OUTPUT

#define NOT_DEFINED std::numeric_limits<std::int32_t>::min()

// Forward declaration
// The implementation of Suffix-Array-Builder and Kasai's Algorithm is a re-implementation of: https://www.geeksforgeeks.org/%C2%AD%C2%ADkasais-algorithm-for-construction-of-lcp-array-from-suffix-array/
class Suffix;
class SuffixArrayBuilder;
class KasaiAlgorithm;
class SubstringCalculator;
template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector);
std::vector<std::string> split_string(std::string input_string);

///////////////////////////////////////
// Suffix - Declaration
class Suffix {
 public:
    Suffix() = default;
    bool operator<(const Suffix &rhs);

    std::int32_t m_original_index{0};
    std::int32_t m_rank{0};
    std::int32_t m_next_rank{NOT_DEFINED};
};



///////////////////////////////////////
// SuffixArrayBuilder - Declaration
class SuffixArrayBuilder {
 public:
    SuffixArrayBuilder() = delete;
    static std::vector<std::int32_t> build(const std::string& s);
    static std::vector<std::int32_t> build(const std::string& s, std::vector<Suffix>& suffixes);
};

///////////////////////////////////////
// KasaiAlgorithm - Declaration
class KasaiAlgorithm {
 public:
    KasaiAlgorithm() = delete;
    static std::vector<std::int32_t> build(const std::string& s, const std::vector<std::int32_t>& suffix_array);
};

///////////////////////////////////////
// SubstringCalculator - Declaration
class SubstringCalculator {
 public:
    SubstringCalculator() = delete;
    static std::int32_t solve(const std::string& s);
};

///////////////////////////////////////
// Suffix - Implementation
bool Suffix::operator<(const Suffix &rhs) {
    bool is_less{false};
    if (this->m_rank < rhs.m_rank) {
        is_less = true;
    }
    else if (this->m_rank == rhs.m_rank) {
        if (this->m_next_rank < rhs.m_next_rank) {
            is_less = true;
        }
    }
    return is_less;
}

///////////////////////////////////////
// SuffixArrayBuilder - Implementation
std::vector<std::int32_t> SuffixArrayBuilder::build(const std::string& s, std::vector<Suffix>& suffixes) {
    std::int32_t n = static_cast<std::int32_t>(s.length());
    suffixes.clear();
    suffixes.resize(n);

    for (std::int32_t idx = 0; idx < n; ++idx) {
        suffixes[idx].m_original_index = idx;
        suffixes[idx].m_rank = static_cast<std::int32_t>(s[idx] - 'a');
        if (idx+1 < n) {
            suffixes[idx].m_next_rank = static_cast<std::int32_t>(s[idx + 1] - 'a');
        }
    }

    // sort by first two characters (m_rank and m_next_rank)
    std::sort(suffixes.begin(), suffixes.end());

    // now sort by first k = 2*n characters, with k = 4, 8, 16, ...
    std::vector<std::int32_t> index_in_suffix(n, 0);
    for (std::int32_t k = 4; k < 2*n; k *= 2) {
        // Assigning rank and index values to first suffix 
        std::int32_t rank{0};
        std::int32_t prev_rank = suffixes[0].m_rank;
        suffixes[0].m_rank = rank;
        index_in_suffix[suffixes[0].m_original_index] = 0;

        // Assigning rank to suffixes
        for (std::int32_t i = 1; i < n; ++i) {
            if (suffixes[i].m_rank == prev_rank &&
                suffixes[i].m_next_rank == suffixes[i-1].m_next_rank) {
                prev_rank = suffixes[i].m_rank;
                suffixes[i].m_rank = rank;
            }
            else {
                prev_rank = suffixes[i].m_rank;
                suffixes[i].m_rank = ++rank;
            }
            index_in_suffix[suffixes[i].m_original_index] = i;
        }

        // Assign next rank to every suffix
        for (std::int32_t i = 0; i < n; ++i) {
            std::int32_t next_index = suffixes[i].m_original_index + k/2;
            if ((next_index < n)) {
                suffixes[i].m_next_rank = suffixes[index_in_suffix[next_index]].m_rank;
            }
            else {
                suffixes[i].m_next_rank = NOT_DEFINED;
            }
        }
  
        // Sort the suffixes according to first k characters 
        std::sort(suffixes.begin(), suffixes.end());
    }

    // Store indexes of all sorted suffixes in the suffix array
    std::vector<std::int32_t> suffix_array(n, 0);
    for (std::int32_t i = 0; i < n; ++i) {
        suffix_array[i] = suffixes[i].m_original_index;
    }

    return suffix_array;
}

std::vector<std::int32_t> SuffixArrayBuilder::build(const std::string& s) {
    std::vector<Suffix> suffixes;
    return SuffixArrayBuilder::build(s, suffixes);
}

///////////////////////////////////////
// KasaiAlgorithm - Implementation
std::vector<std::int32_t> KasaiAlgorithm::build(const std::string& s, const std::vector<std::int32_t>& suffix_array) {
    std::int32_t n = static_cast<std::int32_t>(s.length());
    std::vector<std::int32_t> lcp_array(n, 0);

    // if j = suffix_array[k], then inv_suffix_array[j] = k
    // used to get next suffix string from suffix array
    std::vector<std::int32_t> inv_suffix_array(n, 0);

    for (std::int32_t i = 0; i < n; ++i) {
        inv_suffix_array[suffix_array[i]] = i;
    }

    std::int32_t k = 0;

    // Process all suffixes one by one starting from first suffix in s 
    for (std::int32_t i = 0; i < n; ++i) {
        if (inv_suffix_array[i] == n-1) {
            k = 0;
            continue;
        } 

        // j contains index of the next substring to be considered  to compare with the present substring, i.e., next string in suffix array
        std::int32_t j = suffix_array[inv_suffix_array[i]+1];
  
        // start matching from k'th index as at-least k-1 characters will match
        while (i+k < n && j+k < n && s[i+k] == s[j+k]) {
            ++k;
        }

        lcp_array[inv_suffix_array[i]+1] = k;

        // delete the starting character from the string
        if (k > 0) {
            --k;
        }
    } 

    return lcp_array;
}

///////////////////////////////////////
// SubstringCalculator - Implementation
std::int32_t SubstringCalculator::solve(const std::string& s) {
    std::int32_t counter{0};
    return counter;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef READ_FROM_FILE
void GetInputs(std::int32_t& n, std::int32_t& q, std::string& s, std::vector<std::vector<std::int32_t>>& queries, std::ifstream& textfile) {
    std::string line;
    std::getline(textfile, line);

    std::vector<std::string> nq = split_string(line);

    n = std::stoi(nq[0]);
    q = std::stoi(nq[1]);

    std::getline(textfile, s);

    queries.resize(q);
    for (std::int32_t queries_row_itr = 0; queries_row_itr < q; ++queries_row_itr) {
        queries[queries_row_itr].resize(2);
        std::getline(textfile, line);

        std::vector<std::string> lr = split_string(line);

        std::int32_t left = std::stoi(lr[0]);
        std::int32_t right = std::stoi(lr[1]);

        queries[queries_row_itr][0] = left;
        queries[queries_row_itr][1] = right;
    }
}
#else
void GetInputs(std::int32_t& n, std::int32_t& q, std::string& s, std::vector<std::vector<std::int32_t>>& queries) {
    std::string nq_temp;
    std::getline(std::cin, nq_temp);

    std::vector<std::string> nq = split_string(nq_temp);

    n = std::stoi(nq[0]);
    q = std::stoi(nq[1]);

    std::getline(std::cin, s);

    queries.resize(q);
    for (std::int32_t queries_row_itr = 0; queries_row_itr < q; ++queries_row_itr) {
        queries[queries_row_itr].resize(2);

        for (std::int32_t queries_column_itr = 0; queries_column_itr < 2; ++queries_column_itr) {
            std::cin >> queries[queries_row_itr][queries_column_itr];
        }
        std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
#endif

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector)
{
    for (const auto& element : vector) {
        os << element << ' ';
    }
    return os;
}

//##############################################################################

int main()
{
    std::int32_t n{100000};
    std::int32_t q{100000};
    std::string s;
    std::vector<std::vector<std::int32_t>> queries;
    std::vector<std::vector<std::int32_t>> counts;
    queries.reserve(100000); // Note: reserve means still, that size() == 0
    counts.reserve(100000);

#ifdef READ_FROM_FILE
    std::ifstream textfile{"input00.txt"};
    GetInputs(n, q, s, queries, textfile);
#else
    GetInputs(n, q, s, queries);
#endif

    auto suffix_array = SuffixArrayBuilder::build(s);
    auto lcp_array = KasaiAlgorithm::build(s, suffix_array);
    auto max_value = SubstringCalculator::solve(s);

#ifdef VERBOSE
    std::cout << "s = " << s << std::endl;
    std::cout << "Suffix Array = " << suffix_array << std::endl;
    std::cout << "LCP Array = " << lcp_array << std::endl;
    std::cout << "num(substrings) = ";
#endif
    std::cout << max_value << std::endl;

#ifdef READ_FROM_FILE
    textfile.close();
#endif

    return 0;
}

std::vector<std::string> split_string(std::string input_string) {
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
