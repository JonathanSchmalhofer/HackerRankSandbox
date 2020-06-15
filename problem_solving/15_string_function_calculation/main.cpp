#include <bits/stdc++.h>
#include <limits>

#define READ_FROM_FILE
#define VERBOSE

// Start Implementation of Helper-Class using Aho-Corasick Automaton

#define ALPHABET_SIZE 26
#define NOT_DEFINED std::numeric_limits<std::uint32_t>::max()

// Forward declaration
// The implementation of Suffix-Array-Builder and Kasai's Algorithm is a re-implementation of: https://www.geeksforgeeks.org/%C2%AD%C2%ADkasais-algorithm-for-construction-of-lcp-array-from-suffix-array/
class Suffix;
class SuffixArrayBuilder;
class KasaiAlgorithm;
class StringFunctionCalculator;

///////////////////////////////////////
// Suffix - Declaration
class Suffix {
 public:
    Suffix() = default;
    bool operator<(const Suffix &rhs);

    std::uint32_t m_original_index{0U};
    std::uint32_t m_rank{0U};
    std::uint32_t m_next_rank{NOT_DEFINED};
};



///////////////////////////////////////
// SuffixArrayBuilder - Declaration
class SuffixArrayBuilder {
 public:
    SuffixArrayBuilder() = delete;
    static std::vector<std::uint32_t> build(const std::string& s);
    static std::vector<std::uint32_t> build(const std::string& s, std::vector<Suffix>& suffixes);
};

///////////////////////////////////////
// KasaiAlgorithm - Declaration
class KasaiAlgorithm {
 public:
    KasaiAlgorithm() = delete;
    static std::vector<std::uint32_t> build(const std::string& s, const std::vector<std::uint32_t>& suffix_array);
};

///////////////////////////////////////
// KasaiAlgorithm - Declaration
class StringFunctionCalculator {
 public:
    StringFunctionCalculator() = delete;
    static std::uint32_t solve(const std::vector<std::uint32_t>& lcp_array);
};

///////////////////////////////////////
// Suffix - Implementation
bool Suffix::operator<(const Suffix &rhs) {
    bool is_less{false};
    if (m_rank < rhs.m_rank) {
        is_less = true;
    }
    else if (m_rank == rhs.m_rank) {
        if (m_next_rank < rhs.m_next_rank) {
            is_less = true;
        }
    }
    return is_less;
}

///////////////////////////////////////
// SuffixArrayBuilder - Implementation
std::vector<std::uint32_t> SuffixArrayBuilder::build(const std::string& s, std::vector<Suffix>& suffixes) {
    std::uint32_t n = static_cast<std::uint32_t>(s.length());
    suffixes.clear();
    suffixes.resize(n);

    for (std::uint32_t idx = 0; idx < n; ++idx) {
        suffixes[idx].m_original_index = idx;
        suffixes[idx].m_rank = static_cast<std::uint32_t>(s[idx] - 'a');
        if (idx+1 < n) {
            suffixes[idx].m_next_rank = static_cast<std::uint32_t>(s[idx + 1] - 'a');
        }
    }

    // sort by first two characters (m_rank and m_next_rank)
    std::sort(suffixes.begin(), suffixes.end());

    // now sort by first k = 2*n characters, with k = 4, 8, 16, ...
    std::vector<std::uint32_t> index_in_suffix(n, 0U);
    for (std::uint32_t k = 4; k < 2*n; k *= 2) {
        // Assigning rank and index values to first suffix 
        std::uint32_t rank{0U};
        std::uint32_t prev_rank = suffixes[0].m_rank;
        suffixes[0].m_rank = rank;
        index_in_suffix[suffixes[0].m_original_index] = 0U;

        // Assigning rank to suffixes
        for (std::uint32_t i = 1; i < n; ++i) {
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
        for (std::uint32_t i = 0; i < n; ++i) {
            std::uint32_t next_index = suffixes[i].m_original_index + k/2;
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
    std::vector<std::uint32_t> suffix_array(n, 0U);
    for (std::uint32_t i = 0; i < n; ++i) {
        suffix_array[i] = suffixes[i].m_original_index;
    }

    return suffix_array;
}

std::vector<std::uint32_t> SuffixArrayBuilder::build(const std::string& s) {
    std::vector<Suffix> suffixes;
    return SuffixArrayBuilder::build(s, suffixes);
}

///////////////////////////////////////
// KasaiAlgorithm - Implementation
std::vector<std::uint32_t> KasaiAlgorithm::build(const std::string& s, const std::vector<std::uint32_t>& suffix_array) {
    std::uint32_t n{s.length()};
    std::vector<std::uint32_t> lcp_array(n, 0U);

    // if j = suffix_array[k], then inv_suffix_array[j] = k
    // used to get next suffix string from suffix array
    std::vector<std::uint32_t> inv_suffix_array(n, 0U);

    for (std::uint32_t i = 0; i < n; ++i) {
        inv_suffix_array[suffix_array[i]] = i;
    }

    // Initialize length of previous LCP 
    std::uint32_t k = 0;

    // Process all suffixes one by one starting from 
    // first suffix in txt[] 
    for (std::uint32_t i = 0; i < n; ++i) {
        if (inv_suffix_array[i] == n-1) {
            k = 0;
            continue;
        } 

        /* j contains index of the next substring to 
           be considered  to compare with the present 
           substring, i.e., next string in suffix array */
        std::uint32_t j = suffix_array[inv_suffix_array[i]+1];
  
        // start matching from k'th index as at-least k-1 characters will match
        while (i+k < n && j+k < n && s[i+k] == s[j+k]) {
            ++k;
        }

        lcp_array[inv_suffix_array[i]] = k;

        // Delete the starting character from the string
        if (k > 0) {
            --k;
        }
    } 

    return lcp_array;
}

///////////////////////////////////////
// StringFunctionCalculator - Implementation
std::uint32_t StringFunctionCalculator::solve(const std::vector<std::uint32_t>& lcp_array) {
    std::uint32_t max_value{0U};

    return max_value;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef READ_FROM_FILE
void GetInputs(std::string& t, std::ifstream& textfile) {
    std::getline (textfile, t);
}
#else
void GetInputs(std::string& t) {
    std::getline(std::cin, t);
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
    std::string t;

#ifdef READ_FROM_FILE
    std::ifstream textfile{"input01.txt"};
    GetInputs(t, textfile);
#else
    GetInputs(t);
#endif

    auto suffix_array = SuffixArrayBuilder::build(t);
    auto lcp_array = KasaiAlgorithm::build(t, suffix_array);
    auto max_value = StringFunctionCalculator::solve(lcp_array);

#ifdef VERBOSE
    std::cout << "Suffix Array = " << suffix_array << std::endl;
    std::cout << "LCP Array = " << lcp_array << std::endl;
    std::cout << "Max f(t) = ";
#endif
    std::cout << max_value << std::endl;

#ifdef READ_FROM_FILE
    textfile.close();
#endif

    return 0;
}
