#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <cassert>
#include <map>
#include "custom_free_allocator/custom_free_allocator.h"


class Big
{
public:
	typedef uint16_t cell;
	typedef uint64_t d_cell; //double cell

	static constexpr size_t CELL_LENGTH = sizeof(cell);
	static constexpr size_t CELL_BITS   = sizeof(cell) * 8;

	static constexpr cell CELL_MAXVALUE = static_cast<cell>(0) - 1;
	static inline constexpr d_cell bitmodule(size_t);

	friend std::ostream& operator<< (std::ostream& out, const Big&);
	friend std::istream& operator>> (std::istream& in, Big&);

private:
	using init_vect = std::vector<cell, custom_free_allocator<cell>>;
	using deleter_type = custom_free_allocator<cell>::cleanup_deleter;
	using ptr_type = std::unique_ptr<cell[], deleter_type>;

	size_t   m_length; //amount of bytes
	size_t   m_cell_amount;
	ptr_type m_arr;
	bool     m_positive;


	//this constructor assumes that (d_cell)(cell)v[i] == v[i] for each i
	Big(const std::vector<d_cell>& v);
	Big(init_vect&& v);

	d_cell operator[] (const size_t& index) const;

	std::pair<Big, Big> quot_rem_big  (const Big& r) const;
	std::pair<Big, Big> quot_rem_small(const Big& r) const;

	void negate_this() {m_positive = !m_positive;}

	// arithmetic operators that disregard the sign
	Big atomic_plus(const Big&) const;
	Big atomic_minus(const Big&) const;
	Big atomic_product(const Big&) const;

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
	bool is_positive() const {return m_positive;}

	Big  shift(size_t) const;

	std::string dump(bool print_sign = true) const;
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

	template<typename T> Big operator+ (const T& r) const;
	template<typename T> Big operator- (const T& r) const;
	template<typename T> Big operator* (const T& r) const;
	template<typename T> Big operator/ (const T& r) const;
	template<typename T> Big operator% (const T& r) const;

	template<typename T> Big& operator+= (const T& r);
	template<typename T> Big& operator-= (const T& r);
	template<typename T> Big& operator*= (const T& r);
	template<typename T> Big& operator/= (const T& r);
	template<typename T> Big& operator%= (const T& r);

	std::pair<Big, Big> quot_rem(const Big& r) const;
};

#include "Big-inline.inc"
