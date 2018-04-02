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
: m_cell_amount   (0) // clength/CELL_LENGTH rounded up
, m_first_nonzero (0)
, m_storage       (nullptr)
, m_arr           (nullptr)
, m_positive      (true)
{}


inline Big::Big(const Big& r)
: m_cell_amount   (r.m_cell_amount)
, m_first_nonzero (r.m_first_nonzero)
, m_storage       (r.m_storage)
, m_arr           (r.m_arr)
, m_positive      (r.m_positive)
{
}

inline Big::Big(Big&& r)
: m_cell_amount   (r.m_cell_amount)
, m_first_nonzero (r.m_first_nonzero)
, m_storage       (std::move(r.m_storage))
, m_arr           (r.m_arr)
, m_positive      (r.m_positive)
{}


inline Big::Big(init_vect&& v)
: Big()
{
	// find out the last non-zero element
	size_t last = v.size();
	while (last > 0 and v[last-1] == 0) last -= 1;
	// this will rarely happen, but the case needs to be caught
	if (last == 0)
	{
		// create a new zero number
		return;
	}

	m_cell_amount = last;
	m_storage.reset(new cell [last]);
	m_arr = m_storage.get();

	// a hack for constructors, the only place arr is allowed to chage
	cell* nonconst_arr = const_cast<cell*>(m_arr);
	for (size_t i = 0; i < last; ++i)
		nonconst_arr[i] = static_cast<cell>(v[i]);
}


template<typename Iter>
inline Big::Big(Iter begin, Iter end)
: Big()
{
	Iter last = end - 1;
	while (last != begin and *last == 0)
	{
		--last; --end;
	}
	if (last == begin and *last == 0)
	{
		// create a zero number
		return;
	}

	m_cell_amount = end - begin;
	m_storage.reset( new cell [m_cell_amount]);
	m_arr = m_storage.get();

	// a hack for constructors, the only place arr is allowed to chage
	cell* nonconst_arr = const_cast<cell*>(m_arr);
	for (size_t i = 0; begin != end; ++begin, ++i)
	{
		nonconst_arr[i] = static_cast<cell>(*begin);
	}
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
	m_storage.reset( new cell [m_cell_amount], std::default_delete<cell[]>() );
	m_arr = m_storage.get();
	// a hack for constructors, the only place arr is allowed to chage
	cell* nonconst_arr = const_cast<cell*>(m_arr);
	for (size_t i = 0; i < m_cell_amount; ++i)
		nonconst_arr[i] = result[i];
}


////////////////////////////////////////////////////////////////////////////////


inline Big& Big::operator= (const Big& r)
{
	m_cell_amount = r.m_cell_amount;
	m_positive    = r.m_positive;
	m_storage     = r.m_storage;
	m_arr         = r.m_arr;

	return *this;
}


inline Big& Big::operator= (Big&& r)
{
	m_cell_amount = r.m_cell_amount;
	m_positive    = r.m_positive;
	m_storage     = std::move(r.m_storage);
	m_arr         = r.m_arr;

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


inline Big Big::copy() const
{
	Big r = *this;

	r.m_storage.reset(new cell [m_cell_amount], std::default_delete<cell[]>());
	r.m_arr = r.m_storage.get();
	// don't forget to remove virtual zeroes
	r.m_first_nonzero = 0;

	// a hack for constructors, the only place arr is allowed to chage
	cell* nonconst_arr = const_cast<cell*>(r.m_arr);
	for (size_t i = 0; i < m_cell_amount; ++i)
	{
		nonconst_arr[i] = at(i);
	}
	
	return r;
}


////////////////////////////////////////////////////////////////////////////////


inline Big Big::slice(size_t index, size_t length) const
{
	if (index >= m_cell_amount)
	{
		return Big(0);
	}

	Big r;
	r.m_storage = m_storage;
	r.m_arr = m_arr + index; //start observing elements since index
	r.m_positive = true; //no sense in negative slice

	// determine the length of slice
	if (index + length < m_cell_amount)
	{
		// all ok, use supplied length
		r.m_cell_amount = length;
	}
	else
	{
		// example: length = m_cell_amount, index = 0
		r.m_cell_amount = m_cell_amount - index;
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
inline size_t Big::get_size() const
{
	return m_cell_amount * CELL_LENGTH;
}
inline size_t Big::get_bit_amount() const
{
	return m_cell_amount * CELL_BITS;
}

inline Big::cell Big::bit_at (const size_t& index) const
{
	if (index > get_bit_amount())
	{
		throw std::runtime_error("Big: bit index out of range");
	}
	size_t cell_ind = index / Big::CELL_BITS;
	size_t bit_ind = index % Big::CELL_BITS;
	cell num = at(cell_ind);
	return (num >> bit_ind) & 1;
}

inline Big::cell Big::at (const size_t& index) const
{
	if (index > m_cell_amount)
	{
		throw std::runtime_error("Big: number index out of range");
	}
	if (index < m_first_nonzero)
	{
		return cell(0);
	}
	return m_arr[index - m_first_nonzero];
}

inline Big::cell& Big::mut_ref_at (const size_t& index)
{
	if (index > m_cell_amount)
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


//////////////////////////////////////////////////////////////////////////////


inline Big Big::shift_l (int amount) const
{
	//shifting left (adding zeroes)
	Big result = *this;
	result.m_cell_amount += amount;
	result.m_first_nonzero = amount;

	return result;
}


inline Big Big::shift_r (int amount) const
{
	//shifting right (removing digits)
	// FIXME: what should be done here instead of cast?
	if (amount > (long long)m_cell_amount)
	{
		return Big(0);
	}

	return this->slice(amount, m_cell_amount);
}
