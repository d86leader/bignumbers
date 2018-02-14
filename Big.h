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



class bignumber
{
	friend ostream& operator<< (ostream& out, const bignumber&);
	friend istream& operator>> (istream& in, bignumber&);

	size_t             length; //amount of bytes
	size_t             cells;  //amount of cells
	unique_ptr<cell[]> arr;
	bool               sign;

	//this constructor assumes that (d_cell)(cell)v[i] == v[i] for each i
	bignumber(const vector<d_cell>& v);
	bignumber(const vector<cell>&   v);

	d_cell operator[] (const size_t& index) const;

	pair<bignumber, bignumber> quot_rem_big  (const bignumber& r) const;
	pair<bignumber, bignumber> quot_rem_small(const bignumber& r) const;
public:
	bignumber();
	bignumber(const bignumber&);
	~bignumber();

	template<typename T>
	bignumber(const T& rvalue);

	bignumber& operator= (const bignumber& r);
	template<typename T> bignumber& operator= (const T& r);

	bignumber abs()       const;
	bignumber neg()       const;
	bool      is_nil()    const;
	bignumber shift(size_t) const;

	string     dump(bool printsign = true) const;
	bignumber& restore(const string& str);
	bignumber& restore(const char*   str);

	void generate(size_t size);

	bool operator== (const bignumber& r) const;
	bool operator!= (const bignumber& r) const;
	bool operator>  (const bignumber& r) const;
	bool operator<  (const bignumber& r) const;
	bool operator<= (const bignumber& r) const;
	bool operator>= (const bignumber& r) const;

	bignumber operator+ (const cell& r) const;
	bignumber operator- (const cell& r) const;
	bignumber operator* (const cell& r) const;
	bignumber operator/ (const cell& r) const;
	bignumber operator% (const cell& r) const;

	bignumber operator* (const d_cell& r) const;
	bignumber operator/ (const d_cell& r) const;

	bignumber operator+ (const bignumber& r) const;
	bignumber operator- (const bignumber& r) const;
	bignumber operator* (const bignumber& r) const;
	bignumber operator/ (const bignumber& r) const;
	bignumber operator% (const bignumber& r) const;

	pair<bignumber, bignumber> quot_rem(const bignumber& r) const;
};


inline constexpr d_cell bitmodule(size_t bits)
{
	return static_cast<d_cell>(1) << bits;
}
template<typename T> bignumber& bignumber::operator= (const T& r)
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
bignumber::bignumber(const T& r)
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
