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
, m_storage     (nullptr)
, m_arr         (nullptr)
, m_positive    (true)
{}


inline Big::Big(const Big& r)
: m_length      (r.m_length)
, m_cell_amount (r.m_cell_amount)
, m_storage     (r.m_storage)
, m_arr         (r.m_arr)
, m_positive    (r.m_positive)
{
}

inline Big::Big(Big&& r)
: m_length      (r.m_length)
, m_cell_amount (r.m_cell_amount)
, m_storage     (std::move(r.m_storage))
, m_arr         (m_storage.get())
, m_positive    (r.m_positive)
{}


inline Big::Big(init_vect&& v)
: m_length      (v.size() * CELL_LENGTH)
, m_cell_amount (v.size())
, m_storage     (custom_free_allocator<cell>::own_shared(v))
, m_arr         (m_storage.get())
, m_positive    (true)
{
}


template<typename T>
inline Big::Big(const T& r)
: Big()
{
	if (r == 0)
	{
		return;
	}
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
	m_storage.reset( new cell [m_cell_amount], std::default_delete<cell[]>() );
	m_arr = m_storage.get();
	cell* nonconst_arr = const_cast<cell*>(m_arr);
	for (size_t i = 0; i < m_cell_amount; ++i)
		nonconst_arr[i] = result[i];
}


////////////////////////////////////////////////////////////////////////////////


inline Big& Big::operator= (const Big& r)
{
	m_length      = r.m_length;
	m_cell_amount = r.m_cell_amount;
	m_positive    = r.m_positive;
	m_storage     = r.m_storage;
	m_arr         = r.m_arr;

	return *this;
}


inline Big& Big::operator= (Big&& r)
{
	m_length      = r.m_length;
	m_cell_amount = r.m_cell_amount;
	m_positive    = r.m_positive;
	m_storage     = std::move(r.m_storage);
	m_arr         = m_storage.get();

	return *this;
}


template<typename T>
inline Big& Big::operator= (const T& r)
{
	Big temp (r);
	*this = std::move(temp);
	
	return *this;
}


////////////////////////////////////////////////////////////////////////////////


inline Big::Big(const std::vector<Big::d_cell>& v)
: m_length      (v.size() * CELL_LENGTH)
, m_cell_amount (v.size())
, m_storage     (new cell [v.size()], std::default_delete<cell[]>())
, m_arr         (m_storage.get())
, m_positive    (true)
{
	// a hack for constructors, the only place arr is allowed to chage
	cell* nonconst_arr = const_cast<cell*>(m_arr);
	for (size_t i = 0; i < m_cell_amount; ++i)
		nonconst_arr[i] = static_cast<cell>(v[i]);
}


inline Big Big::copy() const
{
	Big r = *this;

	r.m_storage.reset(new cell [m_cell_amount], std::default_delete<cell[]>());
	r.m_arr = r.m_storage.get();

	cell* nonconst_arr = const_cast<cell*>(r.m_arr);
	for (size_t i = 0; i < m_cell_amount; ++i)
	{
		nonconst_arr[i] = m_arr[i];
	}
	
	return r;
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
	if (index > m_cell_amount)
	{
		throw std::runtime_error("Big: number index out of range");
	}
	return static_cast<d_cell>(m_arr[index]);
}

inline Big::cell Big::bit_at (const size_t& index) const
{
	if (index > m_length*8)
	{
		throw std::runtime_error("Big: bit index out of range");
	}
	size_t cell_ind = index / Big::CELL_BITS;
	size_t bit_ind = index % Big::CELL_BITS;
	cell num = m_arr[cell_ind];
	return (num >> bit_ind) & 1;
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
