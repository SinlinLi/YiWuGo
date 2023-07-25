#pragma once
#define nl '\n'

#include <utility>
#include <unordered_set>
#include <iostream>

using namespace std;

typedef pair<int, int> pii;
typedef unsigned long long ull;

enum Player {
	NONE,
	BLACK,
	WHITE
};

struct PairHash {
	size_t operator()(const std::pair<int, int>& p) const {
		return hash<int>()(p.first * 1000 + p.second);  //·Ç¶Ô³Æ
	}
};

template<class T, class U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& p) {
	os << "(" << (char)('A'+p.first) << ", " << (char)('A'+p.second) << ")";
	return os;
}
