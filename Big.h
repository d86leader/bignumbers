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
	friend ostream& operator<< (ostream& out, const Big&);
	friend istream& operator>> (istream& in, Big&);

	size_t             length; //amount of bytes
	size_t             cells;  //amount of cells
	unique_ptr<cell[]> arr;
	bool               sign;

	//this constructor assumes that (d_cell)(cell)v[i] == v[i] for each i
	Big(const vector<d_cell>& v);
	Big(const vector<cell>&   v);

	d_cell operator[] (const size_t& index) const;

	pair<Big, Big> quot_rem_big  (const Big& r) const;
	pair<Big, Big> quot_rem_small(const Big& r) const;
public:
	Big();
	Big(const Big&);
	~Big();

	template<typename T>
	Big(const T& rvalue);

	Big& operator= (const Big& r);
	template<typename T> Big& operator= (const T& r);

	Big abs()       const;
	Big neg()       const;
	bool      is_nil()    const;
	Big shift(size_t) const;

	string     dump(bool printsign = true) const;
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


inline constexpr d_cell bitmodule(size_t bits)
{
	return static_cast<d_cell>(1) << bits;
}
template<typename T> Big& Big::operator= (const T& r)
{
	sign = r >= 0;
	auto value = sign? r : -r;

	vector<cell> result(0);
	while (value > 0)
	{
		result.push_back(value % bitmodule(CELL_BITS));
		value /= bitmodule(CELL_BITS);
	}

	cells  = result.size();
	length = cells * CELL_LENGTH;
	arr.reset( new cell [cells] );
	for (size_t i = 0; i < cells; ++i)
		arr[i] = result[i];
	
	return *this;
}
template<typename T>
Big::Big(const T& r)
{
	sign = r >= 0;
	auto value = sign? r : -r;

	vector<cell> result(0);
	while (value > 0)
	{
		result.push_back(value % bitmodule(CELL_BITS));
		value /= bitmodule(CELL_BITS);
	}

	cells  = result.size();
	length = cells * CELL_LENGTH;
	arr.reset( new cell [cells] );
	for (size_t i = 0; i < cells; ++i)
		arr[i] = result[i];
}
