// vim: filetype=cpp
#pragma once

inline constexpr d_cell bitmodule(size_t bits)
{
	return static_cast<d_cell>(1) << bits;
}


template<typename T>
inline Big& Big::operator= (const T& r)
{
	m_positive = r >= 0;
	auto value = m_positive? r : -r;

	vector<cell> result(0);
	while (value > 0)
	{
		result.push_back(value % bitmodule(CELL_BITS));
		value /= bitmodule(CELL_BITS);
	}

	m_cell_amount  = result.size();
	m_length = m_cell_amount * CELL_LENGTH;
	m_arr.reset( new cell [m_cell_amount] );
	for (size_t i = 0; i < m_cell_amount; ++i)
		m_arr[i] = result[i];
	
	return *this;
}


template<typename T>
inline Big::Big(const T& r)
{
	m_positive = r >= 0;
	auto value = m_positive? r : -r;

	vector<cell> result(0);
	while (value > 0)
	{
		result.push_back(value % bitmodule(CELL_BITS));
		value /= bitmodule(CELL_BITS);
	}

	m_cell_amount  = result.size();
	m_length = m_cell_amount * CELL_LENGTH;
	m_arr.reset( new cell [m_cell_amount] );
	for (size_t i = 0; i < m_cell_amount; ++i)
		m_arr[i] = result[i];
}


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
	memcpy( m_arr.get(), r.m_arr.get(), m_cell_amount*CELL_LENGTH );
}

inline Big::Big(Big&& r)
: m_length      (r.m_length)
, m_cell_amount (r.m_cell_amount)
, m_arr         (std::move(r.m_arr))
, m_positive    (r.m_positive)
{}


inline Big::Big(const vector<d_cell>& v)
: m_length      (v.size() * CELL_LENGTH)
, m_cell_amount (v.size())
, m_arr         (new cell [v.size()])
, m_positive    (true)
{
	for (size_t i = 0; i < m_cell_amount; ++i)
		m_arr[i] = static_cast<cell>(v[i]);
}
inline Big::Big(const vector<cell>& v)
: m_length      (v.size() * CELL_LENGTH)
, m_cell_amount (v.size())
, m_arr         (new cell [v.size()])
, m_positive    (true)
{
	for (size_t i = 0; i < m_cell_amount; ++i)
		m_arr[i] = v[i];
}


inline Big Big::operator/ (const Big& r) const
{
	//it doesn't work with zeroes
	if (this->is_nil())
		return *this;
	assert(! r.is_nil());
	if (r.m_cell_amount == 1)
		return quot_rem_small(r).first;
	else
		return quot_rem_big  (r).first;
}


inline Big Big::operator% (const Big& r) const
{
	//it doesn't work with zeroes
	if (this->is_nil())
		return *this;
	assert(! r.is_nil());
	if (r.m_cell_amount == 1)
		return quot_rem_small(r).second;
	else
		return quot_rem_big  (r).second;
}


inline Big Big::abs() const
{
	Big t {*this};
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


inline pair<Big, Big> Big::quot_rem (const Big& divider) const
{
	if (this->is_nil())
		return make_pair(*this, *this);
	assert(!divider.is_nil());
	if (divider.m_cell_amount == 1)
		return quot_rem_small(divider);
	else
		return quot_rem_big(divider);
}
