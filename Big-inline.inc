// vim: filetype=cpp
#pragma once
//won't ever be included, but is there for my checkers to work
#include "Big.h"

inline constexpr Big::d_cell Big::bitmodule(size_t bits)
{
	return static_cast<Big::d_cell>(1) << bits;
}


////////////////////////////////////////////////////////////////////////////////


inline Big::Big()
: m_length      (0)
, m_cell_amount (0) // clength/CELL_LENGTH rounded up
, m_arr         (nullptr)
, m_positive    (true)
{}


inline Big::Big(const Big& r)
: m_length      (r.m_length)
, m_cell_amount (r.m_cell_amount)
, m_arr         (new cell [m_cell_amount])
, m_positive    (r.m_positive)
{
	for (size_t i = 0; i < m_cell_amount; ++i)
	{
		m_arr[i] = r.m_arr[i];
	}
}

inline Big::Big(Big&& r)
: m_length      (r.m_length)
, m_cell_amount (r.m_cell_amount)
, m_arr         (std::move(r.m_arr))
, m_positive    (r.m_positive)
{}


template<typename T>
inline Big::Big(const T& r)
{
	m_positive = r >= 0;
	auto value = m_positive? r : -r;

	std::vector<cell> result(0);
	while (value > 0)
	{
		result.push_back(value % Big::bitmodule(CELL_BITS));
		value /= Big::bitmodule(CELL_BITS);
	}

	m_cell_amount  = result.size();
	m_length = m_cell_amount * CELL_LENGTH;
	m_arr.reset( new cell [m_cell_amount] );
	for (size_t i = 0; i < m_cell_amount; ++i)
		m_arr[i] = result[i];
}


////////////////////////////////////////////////////////////////////////////////


inline Big& Big::operator= (const Big& r)
{
	m_length      = r.m_length;
	m_cell_amount = r.m_cell_amount;
	m_positive    = r.m_positive;
	m_arr.reset( new cell [m_cell_amount] );

	for (size_t i = 0; i < m_cell_amount; ++i)
	{
		m_arr[i] = r.m_arr[i];
	}

	return *this;
}


inline Big& Big::operator= (Big&& r)
{
	m_length      = r.m_length;
	m_cell_amount = r.m_cell_amount;
	m_positive    = r.m_positive;
	m_arr = std::move(r.m_arr) ;

	return *this;
}


template<typename T>
inline Big& Big::operator= (const T& r)
{
	m_positive = r >= 0;
	auto value = m_positive? r : -r;

	std::vector<cell> result(0);
	while (value > 0)
	{
		result.push_back(value % Big::bitmodule(CELL_BITS));
		value /= Big::bitmodule(CELL_BITS);
	}

	m_cell_amount  = result.size();
	m_length = m_cell_amount * CELL_LENGTH;
	m_arr.reset( new cell [m_cell_amount] );
	for (size_t i = 0; i < m_cell_amount; ++i)
		m_arr[i] = result[i];
	
	return *this;
}


////////////////////////////////////////////////////////////////////////////////


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
	Big t {*this};
	t.m_positive = false;
	return t;
}
inline bool Big::is_nil() const
{
	return m_cell_amount == 0;
}
inline Big::d_cell Big::operator[] (const size_t& index) const
{
	return static_cast<d_cell>(m_arr[index]);
}


////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline Big Big::operator+ (const T& r) const
{
	Big t = r;
	return *this + t;
}

template<typename T>
inline Big Big::operator- (const T& r) const
{
	Big t = r;
	return *this - t;
}

template<typename T>
inline Big Big::operator* (const T& r) const
{
	Big t = r;
	return *this * t;
}

template<typename T>
inline Big Big::operator/ (const T& r) const
{
	Big t = r;
	return *this / t;
}

template<typename T>
inline Big Big::operator% (const T& r) const
{
	Big t = r;
	return *this % t;
}


////////////////////////////////////////////////////////////////////////////////


template<typename T>
inline Big& Big::operator+= (const T& r)
{
	Big t = r;
	return *this = *this + t;
}

template<typename T>
inline Big& Big::operator-= (const T& r)
{
	Big t = r;
	return *this = *this - t;
}

template<typename T>
inline Big& Big::operator*= (const T& r)
{
	Big t = r;
	return *this = *this * t;
}

template<typename T>
inline Big& Big::operator/= (const T& r)
{
	Big t = r;
	return *this = *this / t;
}

template<typename T>
inline Big& Big::operator%= (const T& r)
{
	Big t = r;
	return *this = *this % t;
}
