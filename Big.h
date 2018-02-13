#pragma once

#include <memory>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cinttypes>
#include <sstream>
#include <cassert>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

typedef uint16_t cell;
typedef uint64_t d_cell; //double cell

constexpr size_t CELL_LENGTH = sizeof(cell);
constexpr size_t CELL_BITS   = sizeof(cell) * 8;

constexpr cell CELL_MAXVALUE = static_cast<cell>(0) - 1;
inline constexpr d_cell bitmodule(size_t);


class Big
{
private:
	friend ostream& operator<< (ostream& out, const Big&);
	friend istream& operator>> (istream& in, Big&);

	size_t             m_length; //amount of bytes
	size_t             m_cell_amount;
	unique_ptr<cell[]> m_arr;
	bool               m_positive;

	//this constructor assumes that (d_cell)(cell)v[i] == v[i] for each i
	Big(const vector<d_cell>& v);
	Big(const vector<cell>&   v);

	d_cell operator[] (const size_t& index) const;

	pair<Big, Big> quot_rem_big  (const Big& r) const;
	pair<Big, Big> quot_rem_small(const Big& r) const;
public:
	Big();
	Big(const Big&);
	Big(Big&&);

	template<typename T>
	Big(const T& rvalue);

	Big& operator= (const Big& r);
	template<typename T> Big& operator= (const T& r);

	Big abs()       const;
	Big neg()       const;
	bool      is_nil()    const;
	Big shift(size_t) const;

	string     dump(bool printm_positive = true) const;
	Big& restore(const string& str);
	Big& restore(const char*   str);

	void generate(size_t size);

	bool operator== (const Big& r) const;
	bool operator!= (const Big& r) const;
	bool operator>  (const Big& r) const;
	bool operator<  (const Big& r) const;
	bool operator<= (const Big& r) const;
	bool operator>= (const Big& r) const;

	Big operator+ (const cell& r) const;
	Big operator- (const cell& r) const;
	Big operator* (const cell& r) const;
	Big operator/ (const cell& r) const;
	Big operator% (const cell& r) const;

	Big operator* (const d_cell& r) const;
	Big operator/ (const d_cell& r) const;

	Big operator+ (const Big& r) const;
	Big operator- (const Big& r) const;
	Big operator* (const Big& r) const;
	Big operator/ (const Big& r) const;
	Big operator% (const Big& r) const;

	pair<Big, Big> quot_rem(const Big& r) const;
};

#include "Big-inline.inc"
