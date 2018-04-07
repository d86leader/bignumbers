// vim: filetype=cpp
#pragma once
//won't ever be included, but is there for my checkers to work
#include "Big.h"


inline Big::Big()
: m_cell_amount   (0) // clength/CellLength rounded up
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
{}

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
		result.push_back(value % CellModulo);
		value /= CellModulo;
	}

	m_cell_amount  = result.size();
	m_storage.reset( new cell [m_cell_amount], deleter_type() );
	m_arr = m_storage.get();
	// a hack for constructors, the only place arr is allowed to chage
	cell* nonconst_arr = const_cast<cell*>(m_arr);
	for (size_t i = 0; i < m_cell_amount; ++i)
		nonconst_arr[i] = result[i];
}


////////////////////////////////////////////////////////////////////////////////


inline Big& Big::operator= (const Big& r)
{
	m_cell_amount   = r.m_cell_amount;
	m_first_nonzero = r.m_first_nonzero;
	m_positive      = r.m_positive;
	m_storage       = r.m_storage;
	m_arr           = r.m_arr;

	return *this;
}


inline Big& Big::operator= (Big&& r)
{
	m_cell_amount   = r.m_cell_amount;
	m_first_nonzero = r.m_first_nonzero;
	m_positive      = r.m_positive;
	m_storage       = std::move(r.m_storage);
	m_arr           = r.m_arr;

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

	r.m_storage.reset(new cell [m_cell_amount], deleter_type());
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
	if (length == 0)
	{
		return Big(0);
	}

	Big r = *this;

	// determine where to start the number
	if (index <= r.m_first_nonzero)
	{
		r.m_first_nonzero -= index;
	}
	else
	{
		// no zeroes left in slice
		size_t arr_index = index - r.m_first_nonzero;
		r.m_first_nonzero = 0;

		r.m_arr = m_arr + arr_index; //start observing elements since index
	}

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

	// remove zero digits from significant positions
	size_t size;
	for (size = r.m_cell_amount; size > 0; --size)
	{
		if (r.at(size - 1) != 0) break;
	}
	if (size == 0) return Big(0);
	r.m_cell_amount = size;

	return r;
}


inline Big Big::shift_l (int amount) const
{
	if (this->is_nil())
	{
		return Big(0);
	}
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
