#include <bits/stdc++.h>
#include <limits>

//#define READ_FROM_FILE
//#define VERBOSE
//#define DEBUG_OUTPUT

#define NOT_DEFINED std::numeric_limits<std::int32_t>::min()

// Forward declaration
// The implementation of Suffix-Array-Builder and Kasai's Algorithm is a re-implementation of: https://www.geeksforgeeks.org/%C2%AD%C2%ADkasais-algorithm-for-construction-of-lcp-array-from-suffix-array/
class Suffix;
class SuffixArrayBuilder;
class KasaiAlgorithm;
class StringFunctionCalculator;
template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector);

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
// KasaiAlgorithm - Declaration
class StringFunctionCalculator {
 public:
    StringFunctionCalculator() = delete;
    static std::int32_t solve(std::vector<std::int32_t> lcp_array);
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
// StringFunctionCalculator - Implementation
std::int32_t StringFunctionCalculator::solve(std::vector<std::int32_t> lcp_array) {
    std::int32_t n{lcp_array.size()};
    std::int32_t max_value{n}; // min(f(t)) = n for any t, because at least the string it self matches once 

    std::int32_t top_stack_index{1U};
    std::vector<std::int32_t> index_stack(n, 0);
    lcp_array.push_back(0); // needed, because we compare up to lcp_array[n]
    index_stack.push_back(0);

#ifdef DEBUG_OUTPUT
    std::cout << "StringFunctionCalculator::solve()" << std::endl;
#endif

    for (std::int32_t i = 1; i < n + 1; ++i)
    {
#ifdef DEBUG_OUTPUT
        std::cout << "    index_stack[] = " << index_stack << std::endl;
#endif
        while (top_stack_index != 0 && lcp_array[i] < lcp_array[index_stack[top_stack_index - 1]])
        {
            std::int32_t intermediate_value = lcp_array[index_stack[top_stack_index - 1]];
            --top_stack_index;
            if (top_stack_index == 0) {
                intermediate_value *= i;
            }
            else {
                intermediate_value *= (i - index_stack[top_stack_index - 1]);
            }
            max_value = std::max(max_value, intermediate_value);
        }

        if (top_stack_index == 0 || 
            (lcp_array[i] >= lcp_array[index_stack[top_stack_index - 1]])) {
            index_stack[top_stack_index++] = i;
        }
#ifdef DEBUG_OUTPUT
        std::cout << "    ----" << std::endl;
#endif
    }

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
    std::ifstream textfile{"input00.txt"};
    GetInputs(t, textfile);
#else
    GetInputs(t);
#endif

    auto suffix_array = SuffixArrayBuilder::build(t);
    auto lcp_array = KasaiAlgorithm::build(t, suffix_array);
    auto max_value = StringFunctionCalculator::solve(lcp_array);

#ifdef VERBOSE
    std::cout << "t = " << t << std::endl;
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
