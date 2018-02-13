#include "Big.h"

const bool debug = false;

bool overflown(const d_cell& value)
{
	return (value >> CELL_BITS) != 0;
}

Big::Big()
: m_length (0)
, m_cell_amount  (0) // clength/CELL_LENGTH rounded up
, m_arr    (nullptr)
, m_positive   (true)
{}


Big::Big(const Big& r)
: m_length (r.m_length)
, m_cell_amount  (r.m_cell_amount)
, m_arr    (new cell [m_cell_amount])
, m_positive   (r.m_positive)
{
	memcpy( m_arr.get(), r.m_arr.get(), m_cell_amount*CELL_LENGTH );
}


Big::Big(const vector<d_cell>& v)
: m_length (v.size() * CELL_LENGTH)
, m_cell_amount  (v.size())
, m_arr    (new cell [v.size()])
, m_positive   (true)
{
	for (size_t i = 0; i < m_cell_amount; ++i)
		m_arr[i] = static_cast<cell>(v[i]);
}
Big::Big(const vector<cell>& v)
: m_length (v.size() * CELL_LENGTH)
, m_cell_amount  (v.size())
, m_arr    (new cell [v.size()])
, m_positive   (true)
{
	for (size_t i = 0; i < m_cell_amount; ++i)
		m_arr[i] = v[i];
}


Big::~Big()
{}


////////////////////////////////////////////////////////////////////////////////


Big Big::abs() const
{
	Big t {*this};
	t.m_positive = true;
	return t;
}
Big Big::neg() const
{
	Big t {*this};
	t.m_positive = false;
	return t;
}


bool Big::is_nil() const
{
	return m_cell_amount == 0;
}


//shifts left by amount
Big Big::shift(size_t amount) const
{
	assert (amount >= 0);
	if (amount == 0) return *this;

	auto r = vector<cell>(0);
	while (amount --> 0)
		r.push_back(0);
	for (size_t i = 0; i < m_cell_amount; ++i)
		r.push_back(m_arr[i]);
	return Big(r);
}


void Big::generate(size_t size)
{
	srand(time(NULL));
	vector<cell> r;
	while (size --> 0)
	{
		r.push_back(rand() % bitmodule(CELL_BITS));
	}
	*this = Big(r);
}


////////////////////////////////////////////////////////////////////////////////


string Big::dump(bool printm_positive) const
{
	if (m_cell_amount == 0)
		return string("0x0");

	std::stringstream dumpstream;

	if (printm_positive)
	{
		if (m_positive)
			dumpstream << "pos 0x";
		else
			dumpstream << "neg 0x";
	}
	else
		dumpstream << "0x";

	dumpstream << std::hex << std::setfill('0') << std::setw(CELL_LENGTH*2);

	for (size_t i = m_cell_amount - 1; i > 0; --i)
	{
		dumpstream << m_arr[i] <<'_' << std::setfill('0') << std::setw(CELL_LENGTH*2);;
	}
	dumpstream << m_arr[0] << std::dec;

	string r {dumpstream.str()};
	return r;
}

inline int digit(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}
Big& Big::restore(const string& str) //aa ff b0 1c
{
	m_length = ( str.size() + 1 ) / 3; //each byte is two symbols && space save for the first one
	m_cell_amount  = (m_length - 1 + CELL_LENGTH)/CELL_LENGTH;
	m_positive   = true;
	m_arr.reset( new cell [m_cell_amount] );
	for (size_t i = 0; i < m_cell_amount; ++i)
		m_arr[i] = 0;

	auto   j = str.size() - 1;
	size_t i = 0;
	size_t shifted = 0;

	for(; j > 1; j -= 3)
	{
		auto byte = (digit(str[j-1]) * 16) + digit(str[j]);
		m_arr[i] <<= 8;
		m_arr[i] += byte;
		shifted += 8;
		if (shifted == CELL_BITS)
		{
			shifted = 0;
			i += 1;
		}
	}
	auto byte = (digit(str[j-1]) * 16) + digit(str[j]);
	m_arr[i] <<= 8;
	m_arr[i] += byte;

	return *this;
}
Big& Big::restore(const char* str)
{
	string s {str};
	return this->restore(s);
}


d_cell Big::operator[] (const size_t& index) const
{
	return static_cast<d_cell>(m_arr[index]);
}


////////////////////////////////////////////////////////////////////////////////


Big& Big::operator= (const Big& r)
{
	m_length = r.m_length;
	m_cell_amount  = r.m_cell_amount;
	m_positive   = r.m_positive;
	m_arr.reset( new cell [m_cell_amount] );

	memcpy( m_arr.get(), r.m_arr.get(), m_cell_amount*CELL_LENGTH );

	return *this;
}


////////////////////////////////////////////////////////////////////////////////


bool Big::operator== (const Big& r) const
{
	if (m_cell_amount != r.m_cell_amount)
		return false;
	if (m_positive != r.m_positive && m_cell_amount != 0)
		return false;
	for (size_t i = 0; i < m_cell_amount; ++i)
	{
		if (m_arr[i] != r.m_arr[i])
			return false;
	}
	return true;
}
bool Big::operator!= (const Big& r) const
{
	return ! (*this == r);
}


bool Big::operator> (const Big& r) const
{
	if (m_positive && !r.m_positive)
		return true;
	if (!m_positive && r.m_positive)
		return false;
	if (!m_positive && !r.m_positive)
		return r.abs() > this->abs();
	if (m_cell_amount > r.m_cell_amount) return true;
	if (m_cell_amount < r.m_cell_amount) return false;
	for (size_t i = m_cell_amount; i > 0; --i)
	{
		if (m_arr[i-1] > r.m_arr[i-1]) return true;
		if (m_arr[i-1] < r.m_arr[i-1]) return false;
	}
	return false;
}


bool Big::operator>= (const Big& r) const
{
	if (m_cell_amount > r.m_cell_amount) return true;
	if (m_cell_amount < r.m_cell_amount) return false;
	if (m_positive && !r.m_positive)
		return true;
	if (!m_positive && r.m_positive)
		return false;
	if (!m_positive && !r.m_positive)
		return r.abs() >= this->abs();
	for (size_t i = m_cell_amount; i > 0; --i)
	{
		if (m_arr[i-1] > r.m_arr[i-1]) return true;
		if (m_arr[i-1] < r.m_arr[i-1]) return false;
	}
	return true;
}


bool Big::operator< (const Big& r) const
{
	return r > *this;
}


bool Big::operator<= (const Big& r) const
{
	return r >= *this;
}


////////////////////////////////////////////////////////////////////////////////


Big Big::operator+ (const cell& r) const
{
	Big t = r;
	return *this + t;
}


Big Big::operator- (const cell& r) const
{
	Big t = r;
	return *this - t;
}


Big Big::operator/ (const cell& r) const
{
	Big t;
	t = r;
	return *this / t;
}
Big Big::operator/ (const d_cell& r) const
{
	Big t;
	t = r;
	return *this / t;
}


Big Big::operator* (const cell& r) const
{
	Big t;
	t = r;
	return *this * t;
}
Big Big::operator* (const d_cell& r) const
{
	Big t;
	t = r;
	return *this * t;
}


////////////////////////////////////////////////////////////////////////////////


Big Big::operator+ (const Big& r) const
{
	if (!m_positive && !r.m_positive)
		return (this->abs() + r.abs()).neg();
	if (!m_positive)
		return r - this->abs();
	if (!r.m_positive)
		return *this - r.abs();

	auto bigger_m_array = (m_cell_amount > r.m_cell_amount) ? m_arr.get() : r.m_arr.get();
	auto high_index = max(m_cell_amount, r.m_cell_amount);
	auto low_index  = min(m_cell_amount, r.m_cell_amount);
	auto result = vector<d_cell>(high_index+1, 0);

	size_t i;
	for (i = 0; i < low_index; ++i)
	{
		result[i] += static_cast<d_cell>(m_arr[i]) + static_cast<d_cell>(r.m_arr[i]);
		if (overflown(result[i]))
		{
			result[i+1] += 1;
			result[i]   %= bitmodule(CELL_BITS);
		}
	}
	for (; i < high_index; ++i)
	{
		result[i] += static_cast<d_cell>(bigger_m_array[i]);
		if (overflown(result[i]))
		{
			result[i+1] += 1;
			result[i]   %= bitmodule(CELL_BITS);
		}
	}
	if (result.back() == 0)
		result.pop_back();

	return Big {result};
}


Big Big::operator- (const Big& r) const
{
	if (*this == r)
		return Big {0};

	if (m_positive && !r.m_positive)
		return *this + r.abs();
	if (!m_positive && r.m_positive)
		return (this->abs() + r).neg();
	if (!m_positive && !r.m_positive)
		return r.abs() + *this;

	if (*this < r)
		return (r - *this).neg();

	//now it's just a subtraction a - b with a >= 0, b >= 0 and a >= b
	auto result = vector<cell>(m_cell_amount);
	size_t i;
	for (i = 0; i < r.m_cell_amount; ++i)
	{
		if (m_arr[i] < r.m_arr[i])
		{
			result.at(i) = bitmodule(CELL_BITS) + static_cast<d_cell>(m_arr[i]) - r.m_arr[i];

			//lend the 1
			for (auto j = i + 1; ; ++j)
			{
				if (m_arr[j] != 0)
				{
					m_arr[j] -= 1;
					break;
				}
				else
				{
					m_arr[j] = CELL_MAXVALUE;
				}
			}
		}
		else
		{
			result.at(i) = m_arr[i] - r.m_arr[i];
		}
	}
	for (; i < m_cell_amount; ++i)
		result.at(i) = m_arr[i];

	while (result.size() > 0 && result.back() == 0)
		result.pop_back();
	
	if (result.size() == 0)
		return Big ();
	return Big {result};
}


Big Big::operator* (const Big& r) const
{
	if (r.is_nil() || this->is_nil())
		return Big(0);
	if (!m_positive && r.m_positive)
		return (this->abs() * r).neg();
	if (m_positive && !r.m_positive)
		return (*this * r.abs()).neg();
	if (!m_positive && !r.m_positive) {}
		//actions are the same actually
	
	auto result = vector<d_cell>(r.m_cell_amount + m_cell_amount + 2);

	for (size_t i = 0; i < m_cell_amount; ++i)
	{
		d_cell t = 0;
		for (size_t j = 0; j < r.m_cell_amount; ++j)
		{
			t = static_cast<d_cell>(m_arr[i]) * static_cast<d_cell>(r.m_arr[j])
			    + t / bitmodule(CELL_BITS) + result.at(i + j);
			result.at(i + j) = t % bitmodule(CELL_BITS);
		}
		result.at(i + r.m_cell_amount) = t / bitmodule(CELL_BITS);
	}

	while(result.size() > 0 && result.back() == 0)
		result.pop_back();
	
	return Big(result);
}


Big Big::operator/ (const Big& r) const
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


Big Big::operator% (const Big& r) const
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


////////////////////////////////////////////////////////////////////////////////


pair<Big, Big> Big::quot_rem_small(const Big& r) const
{
	auto d = r.m_arr[0];
	auto quot_i  = vector<cell>();
	auto current = d_cell(0);

	for (size_t index = m_cell_amount; index > 0; --index)
	{
		auto i = index - 1;

		current *= bitmodule(CELL_BITS);
		current += m_arr[i];

		quot_i.push_back(current / d);
		current -= quot_i.back() * d;
	}

	auto b = quot_i.rend();
	//drop all zeroes in m_positiveificant positions
	while (b != quot_i.rbegin() && *--b == 0) {}
	//b should point to right after what should be inside
	++b;

	//invert the quotient
	auto q_vec = vector<cell>(quot_i.rbegin(), b);
	auto quot  = Big(q_vec);
	Big rem;
	rem = current;
	return make_pair( quot, rem );
}


pair<Big, Big> Big::quot_rem_big  (const Big& divider) const
{
	//simple case: divisor is smaller than divider
	if (*this < divider)
		return make_pair(Big(0), *this);
	//normalization
	auto d = bitmodule(CELL_BITS) / ( divider.m_arr[divider.m_cell_amount-1] + 1 );
	auto u = *this * d;   //normalized divident
	auto v = divider * d; //normalized divisor

	if (debug)
	{
		std::cout << "normalized by " << hex << d << " resulting in " << u.dump(false) << " / " << v.dump(false) <<endl;
	}

	//initialization
	auto quot = vector<cell>();       //result vector
	auto b    = bitmodule(CELL_BITS); //oftenly used module
	auto n    = v.m_cell_amount;
	auto m    = u.m_cell_amount - n;
	//get like in our written algorithm
	auto u_ini_size = u.m_cell_amount;
	auto get_u = [&u_ini_size, &u](const size_t& i)      -> d_cell{
		if (i == 0)
			return 0;
		else
			return u.m_arr[ u_ini_size - i ];
	};
	auto get   = [](const Big& a, const size_t& i) -> d_cell{
		if(i == 0)
			return 0;
		else
			return a.m_arr[a.m_cell_amount - i];
	};

	//main cycle
	for (size_t j = 0; j <= m; ++j)
	{
		if (debug)
		{
			std::cout << "iteration corresponding to " << j <<endl;
			std::cout << "\tcurrent u =\t" << u.dump(false) <<endl;
		}

		d_cell q;
		if (get_u(j) == get(v, 1))
			q = b - 1;
		else
			q = (get_u(j)*b + get_u(j+1)) / get(v, 1);
		if (debug)
		{
			std::cout << "calculated q = " << hex << q <<endl;
		}

		//improving accuracy of q (fucking Knuth)
		while ( get(v,2) * q > ( get_u(j)*b + get_u(j+1) - q*get(v, 1) )*b + get_u(j+2) )
		{
			if (debug)
			{
				std::cout << "\tjust for ruby:\n\t\t0x"
				          <<hex<<get(v,2)<<" * 0x"<<hex<<q<<" > "
				          <<"( 0x"<<hex<<get_u(j)<<"*0x"<<hex<<b
				          <<" + 0x"<<hex<<get_u(j+1)<<" - 0x"<<hex<<q<<"*0x"<<hex<<get(v, 1)<<" )*0x"
				          <<hex<<b<<" + 0x"<<hex<<get_u(j+2)
				          <<endl;
				std::cout << "\trecalculated q as 0x"
				          << get(v,2) * q << " > 0x" << ( get_u(j)*b + get_u(j+1) - q*get(v, 1) )*b + get_u(j+2) <<endl;
			}
			q -= 1;
			assert(q >= 1 && q < b);
		}
		if (debug)
		{
			std::cout << "recalculated q = " << hex << q <<endl;
		}

		auto shiftam = m - j; // amount to shift by; inductibly proved to be correct
		auto slice = (v*q).shift(shiftam);

		//further improve accuracy of q
		//if subtraction of v * q from u[j - v.m_cell_amount ... j]
		//would require borrowing
		if (q != 0 && u < slice)
		{
			if (debug)
			{
				std::cout << "further reduced q as\t" << (v*q).dump(false) << " GT slice\n";
			}
			q -= 1;
			slice = (v*q).shift(shiftam);
		}

		//subtraction
		if (debug)
		{
			std::cout << "about to subtract " <<v.dump(false) << " * " << hex << q
				<< " =\n\t\t\t" << (v*q).dump(false) <<endl;
			std::cout << "subtracting from\t" << u.dump(false) << " a slice shifted by " << shiftam <<endl;
		}
		u = u - slice;
		if (debug)
		{
			std::cout << "subtracted to become\t" << u.dump(false) <<endl;
		}

		//pushing the quotient
		//q :: d_cell, but can fit into cell
		quot.push_back( static_cast<cell>(q) );
		if (debug)
		{
			std::cout << "pushed " << hex << quot.back() << " to result\n";
			std::cout <<endl;
		}
	}


	//drop all zeroes in m_positiveificant positions of quotient
	auto q_rend = quot.rend();
	while (q_rend != quot.rbegin() && *--q_rend == 0) {}
	//b should point to right after what should be inside
	++q_rend;

	//invert the quotient
	auto r_quot = vector<cell>(quot.rbegin(), q_rend);


	//denormalization
	auto quotient  = Big(r_quot);
	if (debug)
	{
		std::cout <<"calling " << u << " / " << hex <<d <<endl;
	}
	auto remainder = u / d;

	return make_pair(quotient, remainder);
}


pair<Big, Big> Big::quot_rem (const Big& divider) const
{
	if (this->is_nil())
		return make_pair(*this, *this);
	assert(!divider.is_nil());
	if (divider.m_cell_amount == 1)
		return quot_rem_small(divider);
	else
		return quot_rem_big(divider);
}


////////////////////////////////////////////////////////////////////////////////


vector<string> split_all(string s, size_t n)
{
	vector<string> result;

	while (s.size() > 0)
	{
		//cut string from string char by char
		string rev_slice;
		for (size_t i = 0; i < n; ++i)
		{
			if (s.size() > 0)
			{
				auto t = s.back();
				rev_slice.push_back(t);
				s.pop_back();
			}
			else
				break;
		}
		//it was reversed; reverse it back
		string slice (rev_slice.rbegin(), rev_slice.rend());

		result.push_back(slice);
	}
	return result;
}

inline cell unhex(char c)
{
	if (c >= 'a' && c <= 'f')
		return 10 + c - 'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c - 'A';
	if (c >= '0' && c <= '9')
		return c - '0';
	assert(false);
}


cell unhex(const string& s)
{
	cell r = 0;
	for (auto i : s)
	{
		r *= 16;
		r += unhex(i);
	}
	return r;
}


ostream& operator << (ostream& out, const Big& number)
{
	if (number == 0)
	{
		out << 0;
		return out;
	}
	out << hex;
	for (size_t i = number.m_cell_amount; i > 0; --i)
	{
		//don't pad if it's the first symbol
		if (i != number.m_cell_amount)
			out.width(CELL_LENGTH * 2);

		out.fill('0');
		out << number.m_arr[i-1];
	}
	out << dec;
	return out;
}


istream& operator >> (istream& in, Big& number)
{
	string s; in >> s;
	if (s == "0")
	{
		number = 0;
		return in;
	}

	auto m_cell_amount = split_all(s, CELL_LENGTH * 2);

	vector<cell> r;
	for (auto i = m_cell_amount.begin(); i != m_cell_amount.end(); ++i)
	{
		r.push_back(unhex(*i));
	}
	number = Big(r);

	return in;
}
