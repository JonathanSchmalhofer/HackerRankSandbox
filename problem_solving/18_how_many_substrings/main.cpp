#//include <bits/stdc++.h>
#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <limits>

//#define READ_FROM_FILE
//#define VERBOSE
//#define DEBUG_OUTPUT
//#define COMPARE_OUTPUT

#define NOT_DEFINED std::numeric_limits<long long int>::min()

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

	long long int m_original_index{ 0 };
	long long int m_rank{ 0 };
	long long int m_next_rank{ NOT_DEFINED };
};

///////////////////////////////////////
// SuffixArrayBuilder - Declaration
class SuffixArrayBuilder {
public:
	SuffixArrayBuilder() = delete;
	static std::vector<long long int> Build(const std::string& s);
	static std::vector<long long int> Build(const std::string& s, std::vector<Suffix>& suffixes);
};

///////////////////////////////////////
// KasaiAlgorithm - Declaration
class KasaiAlgorithm {
public:
	KasaiAlgorithm() = delete;
	static std::vector<long long int> Build(const std::string& s, const std::vector<long long int>& suffix_array);
	static std::vector<long long int> Invert(const std::vector<long long int>& suffix_array);
};

///////////////////////////////////////
// SubstringCalculator - Declaration
class SubstringCalculator {
public:
	struct Query {
		long long int m_left{ 0 };
		long long int m_right{ 0 };
		long long int m_answer{ -1 };
	};
	SubstringCalculator() = delete;
	static std::vector<long long int> Solve(const std::string& s, const std::vector<std::vector<long long int>>& queries);
private:
	static std::vector<std::vector<std::pair<long long int, SubstringCalculator::Query>>> ConvertQueries(const std::vector<std::vector<long long int>>& queries, const long long int n);
};

///////////////////////////////////////
// FenwickTree - Declaration
class FenwickTree {
public:
	FenwickTree(const long long int n);
	void UpdateValue(long long int index, const long long int value);
	long long int LestSignificantBit(long long int i);
	long long int GetValue(long long int i);
private:
	std::vector<long long int> array;
};

///////////////////////////////////////
// SubstringSum - Declaration
class SubstringSum {
public:
	SubstringSum(const long long int n);
	void Add(long long int index, const long long int value);
	void AddRange(long long int left, long long int right);
	long long int GetSum(long long int i);
	long long int GetSum(long long int left, long long int right);
private:
	FenwickTree m_linear_sum;
	FenwickTree m_offset_sum;
	long long int m_length;
};

///////////////////////////////////////
// LCPInterval - Declaration
class LCPInterval {
public:
	LCPInterval(const long long int length, LCPInterval* parent);
	friend std::ostream& operator<<(std::ostream& os, const LCPInterval& interval);

	LCPInterval* m_parent;
	long long int m_min_lcp;
	long long int m_last_seen{ std::numeric_limits<long long int>::max() };
};

///////////////////////////////////////
// LCPIntervalBuilder - Declaration
class LCPIntervalBuilder {
public:
	LCPIntervalBuilder() = delete;
	static std::vector<std::pair<long long int, long long int>> GetLastSeen(const long long int i, const std::vector<LCPInterval*>& lcp_interval, const std::vector<long long int>& inverse_suffix_array);
	static std::vector<LCPInterval*> Build(const std::vector<long long int>& lcp_array);
};

///////////////////////////////////////
// Suffix - Implementation
bool Suffix::operator<(const Suffix &rhs) {
	bool is_less{ false };
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
std::vector<long long int> SuffixArrayBuilder::Build(const std::string& s, std::vector<Suffix>& suffixes) {
	long long int n = static_cast<long long int>(s.length());
	suffixes.clear();
	suffixes.resize(n);

	for (long long int idx = 0; idx < n; ++idx) {
		suffixes[idx].m_original_index = idx;
		suffixes[idx].m_rank = static_cast<long long int>(s[idx] - 'a');
		if (idx + 1 < n) {
			suffixes[idx].m_next_rank = static_cast<long long int>(s[idx + 1] - 'a');
		}
	}

	// sort by first two characters (m_rank and m_next_rank)
	std::sort(suffixes.begin(), suffixes.end());

	// now sort by first k = 2*n characters, with k = 4, 8, 16, ...
	std::vector<long long int> index_in_suffix(n, 0);
	for (long long int k = 4; k < 2 * n; k *= 2) {
		// Assigning rank and index values to first suffix 
		long long int rank{ 0 };
		long long int prev_rank = suffixes[0].m_rank;
		suffixes[0].m_rank = rank;
		index_in_suffix[suffixes[0].m_original_index] = 0;

		// Assigning rank to suffixes
		for (long long int i = 1; i < n; ++i) {
			if (suffixes[i].m_rank == prev_rank &&
				suffixes[i].m_next_rank == suffixes[i - 1].m_next_rank) {
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
		for (long long int i = 0; i < n; ++i) {
			long long int next_index = suffixes[i].m_original_index + k / 2;
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
	std::vector<long long int> suffix_array(n, 0);
	for (long long int i = 0; i < n; ++i) {
		suffix_array[i] = suffixes[i].m_original_index;
	}

	return suffix_array;
}

std::vector<long long int> SuffixArrayBuilder::Build(const std::string& s) {
	std::vector<Suffix> suffixes;
	return SuffixArrayBuilder::Build(s, suffixes);
}

///////////////////////////////////////
// KasaiAlgorithm - Implementation
std::vector<long long int> KasaiAlgorithm::Build(const std::string& s, const std::vector<long long int>& suffix_array) {
	long long int n = static_cast<long long int>(s.length());
	std::vector<long long int> lcp_array(n, 0);

	std::vector<long long int> inv_suffix_array = Invert(suffix_array);

	long long int k = 0;
	// Process all suffixes one by one starting from first suffix in s 
	for (long long int i = 0; i < n; ++i) {
		if (inv_suffix_array[i] == n - 1) {
			k = 0;
			continue;
		}

		// j contains index of the next substring to be considered  to compare with the present substring, i.e., next string in suffix array
		long long int j = suffix_array[inv_suffix_array[i] + 1];

		// start matching from k'th index as at-least k-1 characters will match
		while (i + k < n && j + k < n && s[i + k] == s[j + k]) {
			++k;
		}

		lcp_array[inv_suffix_array[i] + 1] = k;

		// delete the starting character from the string
		if (k > 0) {
			--k;
		}
	}

	return lcp_array;
}

std::vector<long long int> KasaiAlgorithm::Invert(const std::vector<long long int>& suffix_array) {
	long long int n = static_cast<long long int>(suffix_array.size());
	std::vector<long long int> inv_suffix_array(n, 0);

	for (long long int i = 0; i < n; ++i) {
		inv_suffix_array[suffix_array[i]] = i;
	}
	return inv_suffix_array;
}

///////////////////////////////////////
// FenwickTree - Implementation
FenwickTree::FenwickTree(const long long int n) {
	array.resize(n);
}

void FenwickTree::UpdateValue(long long int index, const long long int value) {
	while (index < static_cast<long long int>(array.size())) {
		array[index] += value;
		index += LestSignificantBit(index + 1);
	}
}

long long int FenwickTree::LestSignificantBit(long long int i) {
	return i & -i;
}

long long int FenwickTree::GetValue(long long int i) {
	long long int value{ 0 };
	while (i != 0) {
		value += array[i - 1];
		i -= LestSignificantBit(i);
	}
	return value;
}

///////////////////////////////////////
// SubstringSum - Implementation
SubstringSum::SubstringSum(const long long int n)
	: m_linear_sum{ n }, m_offset_sum{ n }, m_length{ n } {
}

void SubstringSum::Add(long long int index, const long long int value) {
	if (index < m_length) {
		m_offset_sum.UpdateValue(index, value);
	}
}

void SubstringSum::AddRange(long long int left, long long int right) {
	m_linear_sum.UpdateValue(left, 1);
	m_offset_sum.UpdateValue(left, 1 - left);

	m_linear_sum.UpdateValue(right, -1);
	m_offset_sum.UpdateValue(right, right);
}

long long int SubstringSum::GetSum(long long int i) {
	long long int sum{ i };
	sum *= m_linear_sum.GetValue(i + 1);
	sum += m_offset_sum.GetValue(i + 1);
	return sum;
}

long long int SubstringSum::GetSum(long long int left, long long int right) {
	long long int sum = (right + 1 - left) * (right + 2 - left) / 2 - GetSum(right);
	return sum;
}

///////////////////////////////////////
// LCPInterval - Implementation

LCPInterval::LCPInterval(const long long int length, LCPInterval* parent)
	: m_parent{ parent }, m_min_lcp{ length } {
}

std::ostream& operator<<(std::ostream& os, const LCPInterval& interval) {
	os << "min_lcp = " << interval.m_min_lcp << ", last_seen = " << interval.m_last_seen;
	return os;
}

///////////////////////////////////////
// LCPIntervalBuilder - Implementation

std::vector<std::pair<long long int, long long int>> LCPIntervalBuilder::GetLastSeen(const long long int i, const std::vector<LCPInterval*>& lcp_interval, const std::vector<long long int>& inverse_suffix_array) {
	LCPInterval* interval = lcp_interval[inverse_suffix_array[i]];
	std::vector<std::pair<long long int, long long int>> last_seen_stack{};

	long long int last_seen{ std::numeric_limits<long long int>::max() };

	while (interval->m_parent != nullptr) {
		if (interval->m_last_seen < last_seen) {
			last_seen_stack.push_back(std::move(std::pair<long long int, long long int>{interval->m_last_seen, interval->m_min_lcp}));
			last_seen = interval->m_last_seen;
		}
		interval->m_last_seen = i;
		interval = interval->m_parent;
	}

	return last_seen_stack;
}

std::vector<LCPInterval*> LCPIntervalBuilder::Build(const std::vector<long long int>& lcp_array) {
	// the root interval should be the only one with parent == null
	LCPInterval* interval = new LCPInterval(0, nullptr);
	std::vector<LCPInterval*> suffix_array_intervals{};
	long long int n{ static_cast<long long int>(lcp_array.size()) };
	suffix_array_intervals.resize(n);

	// step through lcp build interval assigning max count interval to each item of the initial string;
	// each interval has a link to the containing interval.

	for (long long int i = 0; i < n - 1; ++i) {
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
std::vector<long long int> SubstringCalculator::Solve(const std::string& s, const std::vector<std::vector<long long int>>& queries) {
	std::vector<long long int> all_counters{};
	long long int n = static_cast<long long int>(s.length());
	all_counters.resize(queries.size());

	auto suffix_array = SuffixArrayBuilder::Build(s);
	auto lcp_array = KasaiAlgorithm::Build(s, suffix_array);
	auto inv_suffix_array = KasaiAlgorithm::Invert(suffix_array); // this could also be gotten as by-product from Build()
	SubstringSum substring_sum{ n };
	auto lcp_interval = LCPIntervalBuilder::Build(lcp_array);

#ifdef DEBUG_OUTPUT
	std::cout << "s = " << s << std::endl;
	std::cout << "Suffix Array = " << suffix_array << std::endl;
	std::cout << "Inverse Suffix Array = " << inv_suffix_array << std::endl;
	std::cout << "LCP Array = " << lcp_array << std::endl;
#endif

	// this optimization for the queries must be done to avoid a long running nested for-loop over the queries
	// (else the test cases 4+ seem to fail, probably because of a large number of q)
	auto queries_per_idx = SubstringCalculator::ConvertQueries(queries, n);

	for (long long int idx = n - 1; idx >= 0; --idx) {
		auto last_seen_stack = LCPIntervalBuilder::GetLastSeen(idx, lcp_interval, inv_suffix_array);

		long long int matched_lcp{ 0 };
		while (last_seen_stack.size() > 0) {
			auto last_seen_pair = last_seen_stack.back();
			last_seen_stack.pop_back();
			long long int j = last_seen_pair.first;
			long long int new_lcp = last_seen_pair.second;
			substring_sum.AddRange(j + matched_lcp, j + new_lcp - 1);
			matched_lcp = new_lcp;
		}

		/*
		Type of queries per index is:
		std::vector<std::vector<std::pair<long long int, SubstringCalculator::Query>>>
		Explanation:
		- outer most vector has n entries, so the size of the string;
		for each idx of the string a vector of all registered queries
		starting at this idx is stored. So if no query starts at left = idx (current),
		the vector of registered queries is empty and we can go to the next idx.
		- The next vector holds all registered queries with left = idx (current).
		- Each registered query is made as a pair of the query_index (so at which row should
		the result be printed on screen) and a helper structure for the query (Idea also came from Nick Brett).
		- The query struct has three attributes: left, right and answer (default: -1).
		With this slightly more complicated structure holding the queries, we can shorten the nested for-loop
		*/
		if (!queries_per_idx[idx].empty()) {
			for (const auto& q : queries_per_idx[idx]) {
				all_counters[q.first] = substring_sum.GetSum(q.second.m_left, q.second.m_right);
			}
		}
	}
	return all_counters;
}


std::vector<std::vector<std::pair<long long int, SubstringCalculator::Query>>> SubstringCalculator::ConvertQueries(const std::vector<std::vector<long long int>>& queries, const long long int n) {
	std::vector<std::vector<std::pair<long long int, SubstringCalculator::Query>>> queries_per_index;
	queries_per_index.resize(n);

	long long int query_index{ 0 };
	for (const auto& q : queries) {
		long long int idx = q[0];
		if (queries_per_index[idx].empty()) {
			// no query yet registered starting (=left) at this idx of the string
			SubstringCalculator::Query query;
			query.m_left = idx;
			query.m_right = q[1];
			std::vector<std::pair<long long int, SubstringCalculator::Query>> request_at_idx{};
			request_at_idx.push_back(std::make_pair(query_index, query));
			queries_per_index[idx] = request_at_idx;
		}
		else {
			// there already exists a query starting at this idx
			SubstringCalculator::Query query;
			query.m_left = idx;
			query.m_right = q[1];
			queries_per_index[idx].push_back(std::make_pair(query_index, query));
		}
		++query_index;
	}
	return queries_per_index;
}

////////////////////////////////////////////////////////////////////////////////

#ifdef READ_FROM_FILE
void GetInputs(long long int& n, long long int& q, std::string& s, std::vector<std::vector<long long int>>& queries, std::ifstream& textfile) {
	std::string line;
	std::getline(textfile, line);

	std::vector<std::string> nq = split_string(line);

	n = std::stoll(nq[0]);
	q = std::stoll(nq[1]);

	std::getline(textfile, s);

	queries.resize(q);
	for (long long int queries_row_itr = 0; queries_row_itr < q; ++queries_row_itr) {
		queries[queries_row_itr].resize(2);
		std::getline(textfile, line);

		std::vector<std::string> lr = split_string(line);

		long long int left = std::stoll(lr[0]);
		long long int right = std::stoll(lr[1]);

		queries[queries_row_itr][0] = left;
		queries[queries_row_itr][1] = right;
	}
}

#ifdef COMPARE_OUTPUT
void GetOutputs(const long long int q, std::vector<long long int>& results, std::ifstream& textfile) {
	std::string line;
	results.resize(q);
	for (long long int result_row_itr = 0; result_row_itr < q; ++result_row_itr) {
		std::getline(textfile, line);
		results[result_row_itr] = std::stoll(line);
	}
}
#endif

#else
void GetInputs(long long int& n, long long int& q, std::string& s, std::vector<std::vector<long long int>>& queries) {
	std::string nq_temp;
	std::getline(std::cin, nq_temp);

	std::vector<std::string> nq = split_string(nq_temp);

	n = std::stoll(nq[0]);
	q = std::stoll(nq[1]);

	std::getline(std::cin, s);

	queries.resize(q);
	for (long long int queries_row_itr = 0; queries_row_itr < q; ++queries_row_itr) {
		queries[queries_row_itr].resize(2);

		for (long long int queries_column_itr = 0; queries_column_itr < 2; ++queries_column_itr) {
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
	std::ios::sync_with_stdio(false); // to speed up std::cout
	long long int n{ 100000 };
	long long int q{ 100000 };
	std::string s;
	std::vector<std::vector<long long int>> queries;
	std::vector<std::vector<long long int>> counts;
	queries.reserve(100000); // Note: reserve means still, that size() == 0
	counts.reserve(100000);

#ifdef READ_FROM_FILE
	std::ifstream textfile{ "input06.txt" };
	GetInputs(n, q, s, queries, textfile);
#ifdef COMPARE_OUTPUT
	std::ifstream textfile_results{ "output06.txt" };
	std::vector<long long int> results;
	GetOutputs(q, results, textfile_results);
	textfile_results.close();
#endif
#else
	GetInputs(n, q, s, queries);
#endif

	auto num_distinct_substrings = SubstringCalculator::Solve(s, queries);

#ifdef VERBOSE
	std::cout << "num_distinct_substrings = " << std::endl;
#endif
	for (const auto num : num_distinct_substrings) {
		std::cout << num << "\n"; // "\n" is faster than std::endl
	}

#ifdef COMPARE_OUTPUT
	long long int q_idx{ 0 };
	for (const auto& result : results) {
		if (num_distinct_substrings[q_idx] != result) {
			std::cout << "ERROR at q_idx = " << q_idx << ", expected: " << result << ", got: " << num_distinct_substrings[q_idx] << std::endl;
		}
		++q_idx;
	}
#endif

#ifdef READ_FROM_FILE
	textfile.close();
#endif

	return 0;
}

std::vector<std::string> split_string(std::string input_string) {
	std::string::iterator new_end = unique(input_string.begin(), input_string.end(), [](const char &x, const char &y) {
		return x == y && x == ' ';
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
