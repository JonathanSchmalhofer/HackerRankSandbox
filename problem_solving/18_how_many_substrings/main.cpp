#include <bits/stdc++.h>
#include <limits>

//#define READ_FROM_FILE
//#define VERBOSE
//#define DEBUG_OUTPUT

#define NOT_DEFINED std::numeric_limits<std::int32_t>::min()

// Class forward declarations
// PLEASE NOTE CREDITS: The implementation of Suffix-Array-Builder and Kasai's Algorithm is a re-implementation of: https://www.geeksforgeeks.org/%C2%AD%C2%ADkasais-algorithm-for-construction-of-lcp-array-from-suffix-array/
class Suffix;
class SuffixArrayBuilder;
class KasaiAlgorithm;
class SubstringCalculator;
// PLEASE NOTE CREDITS: The idea for using a FenwickTree, SubstringSum, LCPInterval, and LCPIntervalBuilder as well as the original C# implementation come from Nick Brett
class FenwickTree;
class SubstringSum;
class LCPInterval;
class LCPIntervalBuilder;

// Function forward declarations
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
    static std::vector<std::int32_t> Build(const std::string& s);
    static std::vector<std::int32_t> Build(const std::string& s, std::vector<Suffix>& suffixes);
};

///////////////////////////////////////
// KasaiAlgorithm - Declaration
class KasaiAlgorithm {
 public:
    KasaiAlgorithm() = delete;
    static std::vector<std::int32_t> Build(const std::string& s, const std::vector<std::int32_t>& suffix_array);
    static std::vector<std::int32_t> Invert(const std::vector<std::int32_t>& suffix_array);
};

///////////////////////////////////////
// SubstringCalculator - Declaration
class SubstringCalculator {
 public:
    SubstringCalculator() = delete;
    static std::vector<std::int32_t> Solve(const std::string& s, const std::vector<std::vector<std::int32_t>>& queries);
};

///////////////////////////////////////
// FenwickTree - Declaration
class FenwickTree {
 public:
    FenwickTree(const std::int32_t n);
    void UpdateValue(std::int32_t index, const std::int32_t value);
    std::int32_t LestSignificantBit(std::int32_t i);
    std::int32_t GetValue(std::int32_t i);
private:
    std::vector<std::int32_t> array;
};

///////////////////////////////////////
// SubstringSum - Declaration
class SubstringSum {
 public:
    SubstringSum(const std::int32_t n);
    void Add(std::int32_t index, const std::int32_t value);
    void AddRange(std::int32_t left, std::int32_t right);
    std::int32_t GetSum(std::int32_t i);
    std::int32_t GetSum(std::int32_t left, std::int32_t right);
private:
    FenwickTree m_linear_sum;
    FenwickTree m_offset_sum;
    std::int32_t m_length;
};

///////////////////////////////////////
// LCPInterval - Declaration
class LCPInterval {
 public:
    LCPInterval(const std::int32_t length, LCPInterval* parent);
    friend std::ostream& operator<<(std::ostream& os, const LCPInterval& interval);

    LCPInterval* m_parent;
    std::int32_t m_min_lcp;
    std::int32_t m_last_seen{std::numeric_limits<std::int32_t>::max()};
};

///////////////////////////////////////
// LCPIntervalBuilder - Declaration
class LCPIntervalBuilder {
 public:
    LCPIntervalBuilder() = delete;
    static std::vector<std::pair<std::int32_t, std::int32_t>> GetLastSeen(const std::int32_t i, const std::vector<LCPInterval*>& lcp_interval, const std::vector<std::int32_t>& inverse_suffix_array);
    static std::vector<LCPInterval*> Build(const std::vector<std::int32_t>& lcp_array);
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
std::vector<std::int32_t> SuffixArrayBuilder::Build(const std::string& s, std::vector<Suffix>& suffixes) {
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

std::vector<std::int32_t> SuffixArrayBuilder::Build(const std::string& s) {
    std::vector<Suffix> suffixes;
    return SuffixArrayBuilder::Build(s, suffixes);
}

///////////////////////////////////////
// KasaiAlgorithm - Implementation
std::vector<std::int32_t> KasaiAlgorithm::Build(const std::string& s, const std::vector<std::int32_t>& suffix_array) {
    std::int32_t n = static_cast<std::int32_t>(s.length());
    std::vector<std::int32_t> lcp_array(n, 0);

    std::vector<std::int32_t> inv_suffix_array = Invert(suffix_array);

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

std::vector<std::int32_t> KasaiAlgorithm::Invert(const std::vector<std::int32_t>& suffix_array) {
    std::int32_t n = static_cast<std::int32_t>(suffix_array.size());
    std::vector<std::int32_t> inv_suffix_array(n, 0);

    for (std::int32_t i = 0; i < n; ++i) {
        inv_suffix_array[suffix_array[i]] = i;
    }
    return inv_suffix_array;
}

///////////////////////////////////////
// FenwickTree - Implementation
FenwickTree::FenwickTree(const std::int32_t n) {
    array.resize(n);
}

void FenwickTree::UpdateValue(std::int32_t index, const std::int32_t value) {
    while (index < static_cast<std::int32_t>(array.size())) {
        array[index] += value;
        index += LestSignificantBit(index + 1);
    }
}

std::int32_t FenwickTree::LestSignificantBit(std::int32_t i) {
    return i & -i;
}

std::int32_t FenwickTree::GetValue(std::int32_t i) {
    std::int32_t value{0};
    while (i != 0) {
        value += array[i - 1];
        i -= LestSignificantBit(i);
    }
    return value;
}

///////////////////////////////////////
// SubstringSum - Implementation
SubstringSum::SubstringSum(const std::int32_t n)
 : m_linear_sum{n}, m_offset_sum{n}, m_length{n} {
}

void SubstringSum::Add(std::int32_t index, const std::int32_t value) {
    if (index < m_length) {
        m_offset_sum.UpdateValue(index, value);
    }
}

void SubstringSum::AddRange(std::int32_t left, std::int32_t right) {
    m_linear_sum.UpdateValue(left, 1);
    m_offset_sum.UpdateValue(left, 1 - left);

    m_linear_sum.UpdateValue(right, -1);
    m_offset_sum.UpdateValue(right, right);
}

std::int32_t SubstringSum::GetSum(std::int32_t i) {
    std::int32_t sum{i};
    sum *= m_linear_sum.GetValue(i + 1);
    sum += m_offset_sum.GetValue(i + 1);
    return sum;
}

std::int32_t SubstringSum::GetSum(std::int32_t left, std::int32_t right) {
    std::int32_t sum = (right + 1 - left) * (right + 2 - left) / 2 - GetSum(right);
    return sum;
}   

///////////////////////////////////////
// LCPInterval - Implementation

LCPInterval::LCPInterval(const std::int32_t length, LCPInterval* parent) 
 : m_parent{parent}, m_min_lcp{length} {
}

std::ostream& operator<<(std::ostream& os, const LCPInterval& interval) {
    os << "min_lcp = " << interval.m_min_lcp << ", last_seen = " << interval.m_last_seen;
    return os;
}

///////////////////////////////////////
// LCPIntervalBuilder - Implementation

std::vector<std::pair<std::int32_t, std::int32_t>> LCPIntervalBuilder::GetLastSeen(const std::int32_t i, const std::vector<LCPInterval*>& lcp_interval, const std::vector<std::int32_t>& inverse_suffix_array) {
    LCPInterval* interval = lcp_interval[inverse_suffix_array[i]];
    std::vector<std::pair<std::int32_t, std::int32_t>> last_seen_stack{};

    std::int32_t last_seen{std::numeric_limits<std::int32_t>::max()};

    while (interval->m_parent != nullptr) {
        if (interval->m_last_seen < last_seen) {
            last_seen_stack.push_back(std::move(std::pair<std::int32_t, std::int32_t>{interval->m_last_seen, interval->m_min_lcp}));
            last_seen = interval->m_last_seen;
        }
        interval->m_last_seen = i;
        interval = interval->m_parent;
    }

    return last_seen_stack;
}

std::vector<LCPInterval*> LCPIntervalBuilder::Build(const std::vector<std::int32_t>& lcp_array) {
    // the root interval should be the only one with parent == null
    LCPInterval* interval = new LCPInterval(0, nullptr);
    std::vector<LCPInterval*> suffix_array_intervals{};
    std::int32_t n{static_cast<std::int32_t>(lcp_array.size())};
    suffix_array_intervals.resize(n);

    // step through lcp build interval assigning max count interval to each item of the initial string;
    // each interval has a link to the containing interval.

    for (std::int32_t i = 0; i < n - 1; ++i) {
        //start off we are in the current interval
        //We know we are in the same interval as indexes to the left. We might be in 
        //a sub interval if we have a higher matching prefex to the right.
        suffix_array_intervals[i] = interval;
        LCPInterval* prev_interval = interval;
        while (interval->m_min_lcp > lcp_array[i + 1] && interval->m_parent != nullptr) {
            //close the current ineterval
            prev_interval = interval;
            interval = interval->m_parent;
        }

        if (interval->m_min_lcp < lcp_array[i + 1]) {
            if (interval != prev_interval) {
                // We have closed a higher LCP interval but are inserting a new one between it and its previous
                // SuperInterval.
                // e.g. oldInterval was MinimumLCP = 2 with SuperInterval MinimumLCP=0
                // but we now whish to open an interval MinimumLCP=1
                interval = new LCPInterval(lcp_array[i + 1], interval);
                prev_interval->m_parent = interval;
            }
            else {
                // Starting a new interval from the previous interval with higher LCP
                interval = new LCPInterval(lcp_array[i + 1], interval);
                suffix_array_intervals[i] = interval;
            }
        }

    }
    suffix_array_intervals[lcp_array.size() - 1] = interval;
    return suffix_array_intervals;
}


///////////////////////////////////////
// SubstringCalculator - Implementation
std::vector<std::int32_t> SubstringCalculator::Solve(const std::string& s, const std::vector<std::vector<std::int32_t>>& queries) {
    std::vector<std::int32_t> all_counters{};
    std::int32_t n = static_cast<std::int32_t>(s.length());
    all_counters.resize(queries.size());

    auto suffix_array = SuffixArrayBuilder::Build(s);
    auto lcp_array = KasaiAlgorithm::Build(s, suffix_array);
    auto inv_suffix_array = KasaiAlgorithm::Invert(suffix_array); // this could also be gotten as by-product from Build()
    SubstringSum substring_sum{n};
    auto lcp_interval = LCPIntervalBuilder::Build(lcp_array);

#ifdef DEBUG_OUTPUT
    std::cout << "s = " << s << std::endl;
    std::cout << "Suffix Array = " << suffix_array << std::endl;
    std::cout << "Inverse Suffix Array = " << inv_suffix_array << std::endl;
    std::cout << "LCP Array = " << lcp_array << std::endl;
#endif

    for (std::int32_t idx = n - 1; idx >= 0; --idx) {
        std::int32_t query_idx{0};

        auto last_seen_stack = LCPIntervalBuilder::GetLastSeen(idx, lcp_interval, inv_suffix_array);

        std::int32_t matched_lcp{0};
        while (last_seen_stack.size() > 0) {
            auto last_seen_pair = last_seen_stack.back();
            last_seen_stack.pop_back();
            std::int32_t j = last_seen_pair.first;
            std::int32_t new_lcp = last_seen_pair.second;
            substring_sum.AddRange(j + matched_lcp, j + new_lcp - 1);
            matched_lcp = new_lcp;
        }

        for (const auto& query : queries) {
            auto left = query[0];
            auto right = query[1];

            // Only process queries, that start at the idx we currently look at
            // Yes, this is not optimal, but as computation is skipped for all idx != left, the price should be low
            if (left == idx) {
                // THE CODE BELOW IS FOR MY PERSONAL COMPREHENSION ONLY
                /*
                std::int32_t counter{0};
                std::int32_t number_distinct_substrings{0};

                // n - suffixArr[i] will be the length of suffix
                // at ith position in suffix array initializing
                // count with length of first suffix of sorted
                // suffixes
                number_distinct_substrings = n - suffix_array[0];
                for (std::int32_t i = 1; i < lcp_array.size(); ++i) {
                    counter += (n - suffix_array[i]) - lcp_array[i - 1];
                }
                //counter++;  // for empty string
                */
                // THE CODE ABOVE IS FOR MY PERSONAL COMPREHENSION ONLY
                all_counters[query_idx] = substring_sum.GetSum(left, right);
            }
            ++query_idx;
        }

    }
    return all_counters;
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
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

    auto num_distinct_substrings = SubstringCalculator::Solve(s, queries);

#ifdef VERBOSE
    std::cout << "num_distinct_substrings = " << std::endl;
#endif
    for (const auto num : num_distinct_substrings) {
        std::cout << num << std::endl;
    }

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
