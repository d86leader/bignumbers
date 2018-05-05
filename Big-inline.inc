// vim: filetype=cpp
#pragma once
//won't ever be included, but is there for my checkers to work
#include "Big.h"


inline Big Big::operator/ (const Big& r) const
{
	return quot_rem(r).first;
}

inline Big Big::operator% (const Big& r) const
{
	return quot_rem(r).second;
}


////////////////////////////////////////////////////////////////////////////////


inline Big Big::abs() const
{
	Big t (*this);
	t.m_positive = true;
	return t;
}
inline Big Big::neg() const
{
	Big t (*this);
	t.m_positive = false;
	return t;
}
inline bool Big::is_nil() const
{
	return m_cell_amount == 0;
}
inline size_t Big::get_size() const
{
	return m_cell_amount * CellLength;
}
inline size_t Big::get_bit_amount() const
{
	return m_cell_amount * CellBits;
}

inline Big::cell Big::bit_at (const size_t& index) const
{
	if (index > get_bit_amount())
	{
		throw std::runtime_error("Big: bit index out of range");
	}
	size_t cell_ind = index / Big::CellBits;
	size_t bit_ind = index % Big::CellBits;
	cell num = at(cell_ind);
	return (num >> bit_ind) & 1;
}

inline Big::cell Big::at (const size_t& index) const
{
	if (index >= m_cell_amount)
	{
		throw std::runtime_error("Big: number index out of range");
	}
	if (index < m_first_nonzero)
	{
		return cell(0);
	}
	if (m_arr == nullptr)
	{
		throw std::runtime_error("FATAL: Big: storage was deleted");
	}
	return m_arr[index - m_first_nonzero];
}

inline Big::cell& Big::mut_ref_at (const size_t& index)
{
	if (index >= m_cell_amount)
	{
		throw std::runtime_error("Big: number index out of range");
	}
	if (index < m_first_nonzero)
	{
		throw std::runtime_error("Big: taking const reference of virtual zero. Try to copy() before you do");
	}
	// as this method requires this to be nonconst,
	// casting const away is ok
	cell* nonconst_arr = const_cast<cell*>(m_arr);
	return nonconst_arr[index];
}


////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline Big Big::operator+ (const T& r) const
{
	Big t (r);
	return *this + t;
}

template<typename T>
inline Big Big::operator- (const T& r) const
{
	Big t (r);
	return *this - t;
}

template<typename T>
inline Big Big::operator* (const T& r) const
{
	Big t (r);
	return *this * t;
}

template<typename T>
inline Big Big::operator/ (const T& r) const
{
	Big t (r);
	return *this / t;
}

template<typename T>
inline Big Big::operator% (const T& r) const
{
	Big t (r);
	return *this % t;
}


////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline Big& Big::operator+= (const T& r)
{
	Big t (r);
	return *this = *this + t;
}

template<typename T>
inline Big& Big::operator-= (const T& r)
{
	Big t (r);
	return *this = *this - t;
}

template<typename T>
inline Big& Big::operator*= (const T& r)
{
	Big t (r);
	return *this = *this * t;
}

template<typename T>
inline Big& Big::operator/= (const T& r)
{
	Big t (r);
	return *this = *this / t;
}

template<typename T>
inline Big& Big::operator%= (const T& r)
{
	Big t (r);
	return *this = *this % t;
}


////////////////////////////////////////////////////////////////////////////////


inline bool Big::operator== (const Big& r) const
{
	return this->compare(r) == Comp::Equal;
}
inline bool Big::operator!= (const Big& r) const
{
	return this->compare(r) != Comp::Equal;
}


inline bool Big::operator> (const Big& r) const
{
	return this->compare(r) == Comp::LeftGreater;
}


inline bool Big::operator>= (const Big& r) const
{
	auto res = this->compare(r);
	return res == Comp::Equal or res == Comp::LeftGreater;
}


inline bool Big::operator< (const Big& r) const
{
	return this->compare(r) == Comp::RightGreater;
}


inline bool Big::operator<= (const Big& r) const
{
	auto res = this->compare(r);
	return res == Comp::Equal or res == Comp::RightGreater;
}
