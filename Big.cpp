#include "Big.h"

#include <iomanip>
#include <algorithm>
#include <cinttypes>
#include <sstream>
#include <ctime>

using std::pair;
using std::string;
using std::vector;
using std::hex;
using std::endl;
using std::dec;

const bool debug = false;

using cell = Big::cell;
using d_cell = Big::d_cell;

namespace
{
	inline bool overflown(const d_cell& value)
	{
		return (value >> Big::CELL_BITS) != 0;
	}
}

//////////////////////////////////////////////////////////////////////////////


//shifts left by amount (right ig amount < 0)
Big Big::shift(int amount) const
{
	if (amount == 0) return *this;

	if (amount > 0)
	{
		//shifting left (adding zeroes)
		auto r = vector<cell>(0);
		while (amount --> 0)
			r.push_back(0);
		for (size_t i = 0; i < m_cell_amount; ++i)
			r.push_back(m_arr[i]);
		return Big(r);
	}
	else
	{
		//shifting right (removing digits)
		amount = -amount;
		// FIXME: what should be done here?
		if (amount > (long long)m_cell_amount)
		{
			return Big(0);
		}

		vector<cell> r;
		for (size_t index = amount; index < m_cell_amount; ++index)
		{
			r.push_back(m_arr[index]);
		}
		return Big(r);
	}
}


Big Big::generate(size_t size)
{
	vector<cell> r;
	// pushing tail digints, they can be zero
	while (size --> 1)
	{
		r.push_back(rand() % Big::bitmodule(Big::CELL_BITS));
	}
	// pushing the leading non-zero digit
	cell t = rand();
	while (t == 0)
	{
		t = rand();
	}
	r.push_back(t);

	return Big(r);
}


//////////////////////////////////////////////////////////////////////////////


string Big::dump(bool print_sign) const
{
	if (m_cell_amount == 0)
		return string("0x0");

	std::stringstream dumpstream;

	if (print_sign)
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
		// as digit may be a char type, we cast it to a d_cell which is
		// certainly a number type
		d_cell digit = static_cast<d_cell>(m_arr[i]);
		dumpstream << digit <<'_'
			<< std::setfill('0') << std::setw(CELL_LENGTH*2);;
	}
	d_cell digit = static_cast<d_cell>(m_arr[0]);
	dumpstream << digit << std::dec;

	string r {dumpstream.str()};
	return r;
}

namespace
{
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
}


//////////////////////////////////////////////////////////////////////////////


Big::Comp Big::atomic_compare(const Big& r) const
{
	if (m_cell_amount > r.m_cell_amount)
	{
		return Comp::LeftGreater;
	}
	if (m_cell_amount < r.m_cell_amount)
	{
		return Comp::RightGreater;
	}
	for (size_t pre_i = m_cell_amount; pre_i > 0; --pre_i)
	{
		size_t i = pre_i - 1;
		if (m_arr[i] > r.m_arr[i])
		{
			return Comp::LeftGreater;
		}
		if (m_arr[i] < r.m_arr[i])
		{
			return Comp::RightGreater;
		}
	}
	return Comp::Equal;
}


Big::Comp Big::compare(const Big& r) const
{
	if (m_positive && !r.m_positive)
	{
		return Comp::LeftGreater;
	}
	if (!m_positive && r.m_positive)
	{
		return Comp::RightGreater;
	}
	if (!m_positive && !r.m_positive)
	{
		return r.atomic_compare(*this);
	}
	return this->atomic_compare(r);
}


//////////////////////////////////////////////////////////////////////////////


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


//////////////////////////////////////////////////////////////////////////////


Big Big::atomic_plus(const Big& r) const
{
	auto bigger_m_array =
		(m_cell_amount > r.m_cell_amount) ? m_arr : r.m_arr;
	auto high_index = std::max(m_cell_amount, r.m_cell_amount);
	auto low_index  = std::min(m_cell_amount, r.m_cell_amount);
	auto result = vector<d_cell>(high_index+1, 0);

	size_t i;
	for (i = 0; i < low_index; ++i)
	{
		result[i] += static_cast<d_cell>(m_arr[i])
		           + static_cast<d_cell>(r.m_arr[i]);
		if (overflown(result[i]))
		{
			result[i+1] += 1;
			result[i]   %= Big::bitmodule(Big::CELL_BITS);
		}
	}
	for (; i < high_index; ++i)
	{
		result[i] += static_cast<d_cell>(bigger_m_array[i]);
		if (overflown(result[i]))
		{
			result[i+1] += 1;
			result[i]   %= Big::bitmodule(Big::CELL_BITS);
		}
	}
	if (result.back() == 0)
	{
		result.pop_back();
	}

	return Big {result};
}


Big Big::atomic_minus(const Big& r) const
{
	auto comparison = this->atomic_compare(r);

	if (comparison == Comp::Equal)
	{
		return Big (0);
	}
	if (comparison == Comp::RightGreater)
	{
		auto&& t = r.atomic_minus(*this);
		t.negate_this();
		return t;
	}

	Big this_copy = *this;

	//now it's just a subtraction a - b with a >= 0, b >= 0 and a > b
	auto result = vector<cell>(m_cell_amount);
	size_t i;
	for (i = 0; i < r.m_cell_amount; ++i)
	{
		if (this_copy.m_arr[i] < r.m_arr[i])
		{
			result.at(i) = Big::bitmodule(Big::CELL_BITS) + static_cast<d_cell>(this_copy.m_arr[i]) - r.m_arr[i];

			//lend the 1
			for (auto j = i + 1; ; ++j)
			{
				if (this_copy.m_arr[j] != 0)
				{
					this_copy.m_arr[j] -= 1;
					break;
				}
				else
				{
					this_copy.m_arr[j] = CELL_MAXVALUE;
				}
			}
		}
		else
		{
			result.at(i) = this_copy.m_arr[i] - r.m_arr[i];
		}
	}
	for (; i < m_cell_amount; ++i)
		result.at(i) = this_copy.m_arr[i];

	while (result.size() > 0 && result.back() == 0)
		result.pop_back();
	
	if (result.size() == 0)
		return Big ();
	return Big {result};
}


Big Big::atomic_product(const Big& r) const
{
	//both non-zero as ensured by operator

	auto result = vector<d_cell>(r.m_cell_amount + m_cell_amount + 2);

	for (size_t i = 0; i < m_cell_amount; ++i)
	{
		d_cell t = 0;
		for (size_t j = 0; j < r.m_cell_amount; ++j)
		{
			t = static_cast<d_cell>(m_arr[i]) * static_cast<d_cell>(r.m_arr[j])
			    + t / Big::bitmodule(Big::CELL_BITS) + result.at(i + j);
			result.at(i + j) = t % Big::bitmodule(Big::CELL_BITS);
		}
		result.at(i + r.m_cell_amount) = t / Big::bitmodule(Big::CELL_BITS);
	}

	while(result.size() > 0 && result.back() == 0)
		result.pop_back();
	
	return Big(result);
}


Big Big::molecular_product(const Big& ) const
{
	//this = qN + w
	//r    = aN + s
	//N = 2 ^ (r.bits / 1)
	return Big(0);
}


//////////////////////////////////////////////////////////////////////////////


Big Big::operator+ (const Big& r) const
{
	if (!m_positive && !r.m_positive)
	{
		auto&& t = this->atomic_plus(r);
		t.negate_this();
		return t;
	}
	if (!m_positive)
	{
		return r.atomic_minus(*this);
	}
	if (!r.m_positive)
	{
		return this->atomic_minus(r);
	}

	return this->atomic_plus(r);
}


Big Big::operator- (const Big& r) const
{
	if (m_positive && !r.m_positive)
	{
		return this->atomic_plus(r);
	}
	if (!m_positive && r.m_positive)
	{
		auto&& t = this->atomic_plus(r);
		t.negate_this();
		return t;
	}
	if (!m_positive && !r.m_positive)
	{
		return r.atomic_minus(*this);
	}

	return this->atomic_minus(r);
}


Big Big::operator* (const Big& r) const
{
	if (r.is_nil() || this->is_nil())
		return Big(0);
	if (   (!m_positive &&  r.m_positive)
		|| ( m_positive && !r.m_positive))
	{
		auto&& t = this->atomic_product(r);
		t.negate_this();
		return t;
	}
	if (!m_positive && !r.m_positive) {}
		//the same as if both positive
	
	return this->atomic_product(r);
}


//////////////////////////////////////////////////////////////////////////////


pair<Big, Big> Big::quot_rem_small(const Big& r) const
{
	d_cell d = r.m_arr[0];
	vector<cell> quot_i;
	d_cell current = 0;

	if (m_arr[m_cell_amount - 1] == 0)
	{
		throw "checking for leading zero";
	}

	for (size_t index = m_cell_amount; index > 0; --index)
	{
		auto i = index - 1;

		current *= Big::bitmodule(Big::CELL_BITS);
		current += m_arr[i];

		quot_i.push_back(current / d);
		current -= quot_i.back() * d;
	}

	auto b = quot_i.rend();
	//drop all zeroes in significant positions
	while (b != quot_i.rbegin() && *--b == 0) {}
	//b should point to right after what should be inside
	++b;

	//invert the quotient
	auto q_vec = vector<cell>(quot_i.rbegin(), b);
	auto quot  = Big(q_vec);
	Big rem;
	rem = current;
	return std::make_pair( quot, rem );
}


pair<Big, Big> Big::quot_rem_big  (const Big& divider) const
{
	//simple case: divisor is smaller than divider
	if (*this < divider)
		return std::make_pair(Big(0), *this);
	//normalization
	d_cell d = Big::bitmodule(Big::CELL_BITS) / ( divider.m_arr[divider.m_cell_amount-1] + 1 );
	Big u = *this * d;   //normalized divident
	Big v = divider * d; //normalized divisor

	if (debug)
	{
		std::cout << "normalized by " << hex << d << " resulting in " << u.dump(false) << " / " << v.dump(false) <<endl;
	}

	//initialization
	vector<cell> quot;       //result vector
	d_cell b    = Big::bitmodule(Big::CELL_BITS); //oftenly used module
	size_t n    = v.m_cell_amount;
	size_t m    = u.m_cell_amount - n;
	//get like in our written algorithm
	const size_t u_ini_size = u.m_cell_amount;
	auto get_u = [&u_ini_size, &u](const size_t& i) -> d_cell{
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

	assert(get_u(1) != 0);

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
			assert(q >= 1);
			assert(q < b);
		}
		if (debug)
		{
			std::cout << "recalculated q = " << hex << q <<endl;
		}

		size_t shiftam = m - j; // amount to shift by; inductibly proved to be correct
		Big slice = (v*q).shift(shiftam);

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


	//drop all zeroes in significant positions of quotient
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

	return std::make_pair(quotient, remainder);
}


std::pair<Big, Big> Big::quot_rem (const Big& divider) const
{
	if (this->is_nil())
		return std::make_pair(*this, *this);
	assert(!divider.is_nil());

	if (divider.m_cell_amount == 1) //dividing by small number
	{
		if (!divider.m_positive)
		{
			auto t = this->quot_rem_small(divider);
			t.first.negate_this();
			return t;
		}
		if (!m_positive)
		{
			auto t = this->quot_rem_small(divider);
			t.first.negate_this();

			// make the remainder be positive and conforming to
			// q * b + r = a
			if (!t.second.is_nil())
			{
				t.first = t.first - 1;
				t.second = divider - t.second;
			}

			return t;
		}
		return this->quot_rem_small(divider);
	}
	else //division of big numbers
	{
		if (!divider.m_positive)
		{
			auto t = this->quot_rem_big(divider);
			t.first.negate_this();
			return t;
		}
		if (!m_positive)
		{
			auto t = this->quot_rem_big(divider);
			t.first.negate_this();

			// make the remainder be positive and conforming to
			// q * b + r = a
			if (!t.second.is_nil())
			{
				t.first = t.first - 1;
				t.second = divider - t.second;
			}

			return t;
		}
		return this->quot_rem_big(divider);
	}
}


//////////////////////////////////////////////////////////////////////////////


namespace
{
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
}


std::ostream& operator << (std::ostream& out, const Big& number)
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
			out.width(Big::CELL_LENGTH * 2);

		out.fill('0');
		out << number.m_arr[i-1];
	}
	out << dec;
	return out;
}


std::istream& operator >> (std::istream& in, Big& number)
{
	string s; in >> s;
	if (s == "0")
	{
		number = 0;
		return in;
	}

	auto m_cell_amount = split_all(s, Big::CELL_LENGTH * 2);

	vector<cell> r;
	for (auto i = m_cell_amount.begin(); i != m_cell_amount.end(); ++i)
	{
		r.push_back(unhex(*i));
	}
	number = Big(r);

	return in;
}


//////////////////////////////////////////////////////////////////////////////


Big Big::exp (const Big& r, const Big& modulo) const
{
	if (this->is_nil())
	{
		return *this;
	}
	if (r.is_nil())
	{
		return Big (1);
	}
	return this->exponentiate_rtl(r, modulo);
}


Big Big::exponentiate_rtl(const Big& r, const Big& modulo) const
{
	// r is nont-nil
	Big this_power = *this;
	Big result = 1;
	for (size_t bit_index = 0; bit_index < r.m_length*8; ++bit_index)
	{
		if (r.bit_at(bit_index) == 1)
		{
			result = (result * this_power) % modulo;
		}
		this_power = (this_power * this_power) % modulo;
	}
	return result;
}


Big Big::slice(size_t index, size_t length) const
{
	vector<cell> r;

	size_t end = index + length < m_cell_amount ?
		index + length : m_cell_amount - 1;

	for (; index < end; ++index)
	{
		r.push_back(m_arr[index]);
	}
	return Big(r);
}
