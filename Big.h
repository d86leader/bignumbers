#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <cassert>
#include <map>

#ifdef DEBUG
# define private public
#endif


class Big
{
public:
	typedef uint16_t cell;
	typedef uint64_t d_cell; //double cell

	static constexpr size_t CellLength = sizeof(cell);
	static constexpr size_t CellBits   = sizeof(cell) * 8;

	static constexpr cell CellMaxValue = static_cast<cell>(0) - 1;
	static constexpr d_cell CellModulo = static_cast<d_cell>(1) << CellBits;

	enum struct Comp
	{
		LeftGreater,
		Equal,
		RightGreater
	};

private:
	friend std::ostream& operator<< (std::ostream& out, const Big&);
	friend std::istream& operator>> (std::istream& in, Big&);

	using init_vect = std::vector<cell>;
	using deleter_type = std::default_delete<cell>;
	using ptr_type = std::shared_ptr<cell>;

	size_t       m_cell_amount;
	size_t       m_first_nonzero;
	ptr_type     m_storage;
	const cell * m_arr;
	bool         m_positive;

	//this constructor assumes that (d_cell)(cell)v[i] == v[i] for each i
	Big(init_vect&& v);
	template<typename Iter>
	Big(Iter begin, Iter end);
	// deleted constructors as to suppress compilation garbage
	Big(const std::vector<d_cell>& v) = delete;
	Big(init_vect& v) = delete;
	Big(const init_vect&) = delete;
	// WARNING: whenever init_vect is different form vector<cell>,
	// constructors from vector<cell> should be deleted either
	// i could not achieve this with metaprogramming

	//make a deep copy of number, meaning copy the arr
	Big copy() const;

	cell   bit_at     (const size_t& index) const;
	cell   at         (const size_t& index) const;
	cell&  mut_ref_at (const size_t& index);

	//those divisions disregard the sign
	std::pair<Big, Big> quot_rem_big  (const Big& r) const;
	std::pair<Big, Big> quot_rem_small(const Big& r) const;

	void negate_this() {m_positive = !m_positive;}

	// comparison taht disregards the sign
	Comp atomic_compare(const Big&) const;

	// arithmetic operators that disregard the sign
	Big atomic_plus(const Big&) const;
	Big atomic_minus(const Big&) const;
	Big atomic_product(const Big&) const;
	Big molecular_product(const Big&) const;

	// disregards the sign. Right-to-left algorithm
	Big exponentiate_rtl(const Big&, const Big&) const;

	// efficient shifts
	Big shift_l(int) const;
	Big shift_r(int) const;

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
	size_t get_size() const; //size in bytes
	size_t get_bit_amount() const;

	Big  shift(int) const; //cellwise shift

	std::string dump(bool print_sign = true) const;

	Comp compare    (const Big& r) const;
	bool operator== (const Big& r) const;
	bool operator!= (const Big& r) const;
	bool operator>  (const Big& r) const;
	bool operator<  (const Big& r) const;
	bool operator<= (const Big& r) const;
	bool operator>= (const Big& r) const;

	Big operator+ (const Big& r) const;
	Big operator- (const Big& r) const;
	Big operator* (const Big& r) const;
	Big operator/ (const Big& r) const;
	Big operator% (const Big& r) const;
	// no need for atomic for those two, they are already simple enough
	Big operator& (const Big& r) const;
	Big operator| (const Big& r) const;

	Big exp  (const Big& r, const Big& m) const;
	Big slice(size_t start, size_t length) const;

	Big operator>> (size_t) const;
	Big operator<< (size_t) const;

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
#include "constructors.inc"
