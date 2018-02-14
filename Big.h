#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <cassert>
#include <map>

class Big
{
public:
	typedef uint16_t cell;
	typedef uint64_t d_cell; //double cell

	static constexpr size_t CELL_LENGTH = sizeof(cell);
	static constexpr size_t CELL_BITS   = sizeof(cell) * 8;

	static constexpr cell CELL_MAXVALUE = static_cast<cell>(0) - 1;
	static inline constexpr d_cell bitmodule(size_t);

private:
	friend std::ostream& operator<< (std::ostream& out, const Big&);
	friend std::istream& operator>> (std::istream& in, Big&);

	size_t                  m_length; //amount of bytes
	size_t                  m_cell_amount;
	std::unique_ptr<cell[]> m_arr;
	bool                    m_positive;

	//this constructor assumes that (d_cell)(cell)v[i] == v[i] for each i
	Big(const std::vector<d_cell>& v);
	Big(const std::vector<cell>&   v);

	d_cell operator[] (const size_t& index) const;

	std::pair<Big, Big> quot_rem_big  (const Big& r) const;
	std::pair<Big, Big> quot_rem_small(const Big& r) const;

public:
	static Big generate(size_t size);

	Big();
	Big(const Big&);
	Big(Big&&);
	template<typename T> Big(const T& rvalue);

	Big& operator= (const Big&);
	Big& operator= (Big&&);
	template<typename T> Big& operator= (const T&);

	Big  abs()       const;
	Big  neg()       const;
	bool is_nil()    const;
	Big  shift(size_t) const;

	std::string dump(bool printsign = true) const;
	Big& restore(const std::string& str);
	Big& restore(const char*        str);

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

	std::pair<Big, Big> quot_rem(const Big& r) const;
};

#include "Big-inline.inc"
