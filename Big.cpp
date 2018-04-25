#include "Big.h"

#include <iomanip>
#include <sstream>
#include <cassert>

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
		return (value >> Big::CellBits) != 0;
	}
}

//////////////////////////////////////////////////////////////////////////////


bool Big::is_power_of_2() const
{
	// famous algorith: x & (x-1) == 0 if and only if x is a power 2,
	// given positive x
	return (*this & (*this - 1)) == 0;
}


size_t Big::last_bit_index() const
{
	if (is_nil()) return 0;

	size_t pre_answer = (m_cell_amount - 1) * CellBits;
	cell last = at(m_cell_amount - 1);

	// a simple algorithm as i can't be bothered now

	cell current_mask = CellMaxValue;
	// + 1 as it only stops decrementing after reaching zero
	size_t last_index = CellBits + 1;
	while ((last & current_mask) != 0)
	{
		current_mask >>= 1;
		last_index -= 1;
		if (last_index > CellBits)
		{
			throw std::runtime_error(
				"Big: encountered a number with leading zero");
		}
	}

	size_t answer = pre_answer + last_index;
	return answer;
}


//////////////////////////////////////////////////////////////////////////////


//shifts left by amount (right ig amount < 0)
Big Big::shift(int amount) const
{
	if (amount == 0) return *this;

	if (amount > 0)
	{
		return this->shift_l(amount);
	}
	else
	{
		return this->shift_r(-amount);
	}
}


Big Big::generate(size_t size, distribution_type& d, generator_type& e)
{
	Big::init_vect r;
	r.reserve(size);
	// pushing tail digints, they can be zero
	while (size --> 1)
	{
		r.push_back(d(e));
	}
	// pushing the leading non-zero digit
	cell t = d(e);
	while (t == 0)
	{
		t = d(e);
	}
	r.push_back(t);

	return Big(std::move(r));
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

	dumpstream << std::hex << std::setfill('0') << std::setw(CellLength*2);

	for (size_t i = m_cell_amount - 1; i > 0; --i)
	{
		// as digit may be a char type, we cast it to a d_cell which is
		// certainly a number type
		d_cell digit = static_cast<d_cell>(at(i));
		dumpstream << digit <<'_'
			<< std::setfill('0') << std::setw(CellLength*2);;
	}
	d_cell digit = static_cast<d_cell>(at(0));
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
		if (at(i) > r.at(i))
		{
			return Comp::LeftGreater;
		}
		if (at(i) < r.at(i))
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


Big Big::atomic_plus(const Big& r) const
{
	const Big& bigger_number =
		(m_cell_amount > r.m_cell_amount) ? *this : r;

	auto high_index = std::max(m_cell_amount, r.m_cell_amount);
	auto low_index  = std::min(m_cell_amount, r.m_cell_amount);
	auto result = vector<d_cell>(high_index+1, 0);

	size_t i;
	for (i = 0; i < low_index; ++i)
	{
		result[i] += static_cast<d_cell>(at(i))
		           + static_cast<d_cell>(r.at(i));
		if (overflown(result[i]))
		{
			result[i+1] += 1;
			result[i]   %= CellModulo;
		}
	}
	for (; i < high_index; ++i)
	{
		result[i] += static_cast<d_cell>(bigger_number.at(i));
		if (overflown(result[i]))
		{
			result[i+1] += 1;
			result[i]   %= CellModulo;
		}
	}

	return Big(result.begin(), result.end());
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

	// create a further deleted copy of this so we can modify its digits
	Big&& this_copy = this->copy();

	//now it's just a subtraction a - b with a >= 0, b >= 0 and a > b
	auto result = Big::init_vect(m_cell_amount);
	size_t i;
	for (i = 0; i < r.m_cell_amount; ++i)
	{
		if (this_copy.at(i) < r.at(i))
		{
			result.at(i) = CellModulo + static_cast<d_cell>(this_copy.at(i)) - r.at(i);

			//lend the 1
			for (auto j = i + 1; ; ++j)
			{
				if (this_copy.at(j) != 0)
				{
					this_copy.mut_ref_at(j) -= 1;
					break;
				}
				else
				{
					this_copy.mut_ref_at(j) = CellMaxValue;
				}
			}
		}
		else
		{
			result.at(i) = this_copy.at(i) - r.at(i);
		}
	}
	for (; i < m_cell_amount; ++i)
		result.at(i) = this_copy.at(i);

	while (result.size() > 0 && result.back() == 0)
		result.pop_back();
	
	if (result.size() == 0)
		return Big ();
	return Big (std::move(result));
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
			t = static_cast<d_cell>(at(i)) * static_cast<d_cell>(r.at(j))
			    + t / CellModulo + result.at(i + j);
			result.at(i + j) = t % CellModulo;
		}
		result.at(i + r.m_cell_amount) = t / CellModulo;
	}

	return Big(result.begin(), result.end());
}


Big Big::molecular_product(const Big& r) const
{
	size_t all_length = m_cell_amount > r.m_cell_amount ?
		m_cell_amount : r.m_cell_amount;

	size_t slice_length = all_length / 2;
	if (all_length % 2 != 0)
	{
		slice_length += 1;
	}

	//this = qN + w
	//r    = aN + s
	//tr = qaNN + qsN + awN + sw
	// C = (q+w)(a+s) = qa + wa + qs + ws
	// A = qa, B = ws
	// tr = ANN + (C - A - B)N + B

	Big&& this_lower  = this->slice(0, slice_length);
	Big&& this_higher = this->slice(slice_length, all_length);
	Big&& r_lower     = r.slice(0, slice_length);
	Big&& r_higher    = r.slice(slice_length, all_length);

	Big&& quad_product =
		(this_lower + this_higher) *
		(r_lower + r_higher);

	Big&& high_product = this_higher * r_higher;
	Big&& low_product  = this_lower * r_lower;
	Big&& middle = quad_product - high_product - low_product;

	Big&& result =
		high_product.shift(slice_length*2) +
		middle.shift(slice_length) +
		low_product;

	return result;
}


//////////////////////////////////////////////////////////////////////////////


Big Big::operator& (const Big& r) const
{
	if (this->is_nil() or r.is_nil())
	{
		return Big(0);
	}
	init_vect result;

	size_t min_length = std::min(m_cell_amount, r.m_cell_amount);
	result.reserve(min_length);
	for (size_t i = 0; i < min_length; ++i)
	{
		result.push_back(at(i) & r.at(i));
	}

	return Big(std::move(result));
}


Big Big::operator| (const Big& r) const
{
	if (this->is_nil()) return r;
	if (r.is_nil())     return *this;

	// it's easier to work when you know which is bigger
	if (r.m_cell_amount > m_cell_amount)
	{
		return r | *this;
	}

	init_vect result;
	result.reserve(m_cell_amount);

	for (size_t i = 0; i < r.m_cell_amount; ++i)
	{
		result.push_back(at(i) | r.at(i));
	}
	for (size_t i = r.m_cell_amount; i < m_cell_amount; ++i)
	{
		result.push_back(at(i));
	}

	return Big(std::move(result));
}


Big Big::operator^ (const Big& r) const
{
	if (this->is_nil()) return r;
	if (r.is_nil())     return *this;

	// it's easier to work when you know which is bigger
	if (r.m_cell_amount > m_cell_amount)
	{
		return r ^ *this;
	}

	init_vect result;
	result.reserve(m_cell_amount);

	for (size_t i = 0; i < r.m_cell_amount; ++i)
	{
		result.push_back(at(i) ^ r.at(i));
	}
	for (size_t i = r.m_cell_amount; i < m_cell_amount; ++i)
	{
		result.push_back(at(i));
	}

	return Big(std::move(result));
}


Big Big::operator~ () const
{
	if (this->is_nil())
	{
		// in the spirit of the last cell being negated as a whole,
		// we negate the cell with the sole zero as if it existed
		return Big(CellMaxValue);
	}

	init_vect result;
	result.reserve(m_cell_amount);

	for (size_t i = 0; i < m_cell_amount; ++i)
	{
		result.push_back(~at(i));
	}

	return Big(std::move(result));
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
	constexpr size_t atomic_threshold = 72;

	if (r.is_nil() || this->is_nil())
		return Big(0);

	if (   (!m_positive &&  r.m_positive)
		|| ( m_positive && !r.m_positive))
	{
		// molecular is faster but doesn't work for length of 1
		if (m_cell_amount <= atomic_threshold or r.m_cell_amount <= atomic_threshold)
		{
			auto&& t = this->atomic_product(r);
			t.negate_this();
			return t;
		}
		else
		{
			auto&& t = this->molecular_product(r);
			t.negate_this();
			return t;
		}
	}
	if (!m_positive && !r.m_positive) {}
		//the same as if both positive
	
	// molecular is faster but doesn't work for length of 1
	if (m_cell_amount <= atomic_threshold or r.m_cell_amount <= atomic_threshold)
	{
		return this->atomic_product(r);
	}
	else
	{
		return this->molecular_product(r);
	}
}


//////////////////////////////////////////////////////////////////////////////


pair<Big, Big> Big::quot_rem_small(const Big& r) const
{
	d_cell d = r.at(0);
	Big::init_vect quot_i;
	quot_i.reserve(m_cell_amount);
	d_cell current = 0;

	if (at(m_cell_amount - 1) == 0)
	{
		throw "checking for leading zero";
	}

	for (size_t index = m_cell_amount; index > 0; --index)
	{
		auto i = index - 1;

		current *= CellModulo;
		current += at(i);

		quot_i.push_back(current / d);
		current -= quot_i.back() * d;
	}

	Big quot (quot_i.rbegin(), quot_i.rend());
	Big&& rem = current;
	return std::make_pair( quot, rem );
}


pair<Big, Big> Big::quot_rem_big  (const Big& divider) const
{
	//simple case: divisor is smaller than divider
	if (*this < divider)
		return std::make_pair(Big(0), *this);
	//normalization
	d_cell d = CellModulo / ( divider.at(divider.m_cell_amount-1) + 1 );
	Big&& u = *this * d;   //normalized divident
	Big&& v = divider * d; //normalized divisor

	if (debug)
	{
		std::cout << "normalized by " << hex << d << " resulting in " << u.dump(false) << " / " << v.dump(false) <<endl;
	}

	//initialization
	Big::init_vect quot;       //result vector
	d_cell b    = CellModulo; //oftenly used module
	size_t n    = v.m_cell_amount;
	size_t m    = u.m_cell_amount - n;
	//get like in our written algorithm
	const size_t u_ini_size = u.m_cell_amount;
	auto get_u = [&u_ini_size, &u](const size_t& i) -> d_cell{
		if (i == 0)
			return 0;
		else
			return u.at( u_ini_size - i );
	};
	auto get   = [](const Big& a, const size_t& i) -> d_cell{
		if(i == 0)
			return 0;
		else
			return a.at(a.m_cell_amount - i);
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
		Big&& slice = (v*q).shift(shiftam);

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


	//denormalization
	Big quotient (quot.rbegin(), quot.rend());
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
			out.width(Big::CellLength * 2);

		out.fill('0');
		out << number.at(i-1);
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

	auto m_cell_amount = split_all(s, Big::CellLength * 2);

	Big::init_vect r;
	for (auto i = m_cell_amount.begin(); i != m_cell_amount.end(); ++i)
	{
		r.push_back(unhex(*i));
	}
	number = Big(std::move(r));

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

	size_t bit_amount = r.get_bit_amount();
	for (size_t bit_index = 0; bit_index < bit_amount; ++bit_index)
	{
		if (r.bit_at(bit_index) == 1)
		{
			result = (result * this_power) % modulo;
		}
		this_power = (this_power * this_power) % modulo;
	}
	return result;
}


//////////////////////////////////////////////////////////////////////////////


Big Big::operator>> (size_t amount) const
{
	if (this->is_nil())
	{
		return Big(0);
	}
	if (amount % CellBits == 0)
	{
		return this->shift_r(amount / CellBits);
	}

	// shift all cells so remaining shifts are only to neighbooring cells
	Big&& temp = this->shift_r(amount / CellBits);
	if (temp.is_nil())
	{
		return temp;
	}

	amount %= CellBits;

	init_vect result;
	result.reserve(temp.m_cell_amount);
	for (size_t i = 0; i < temp.m_cell_amount - 1; ++i)
	{
		// put the highest bits of current to lower positions
		cell lower = temp.at(i) >> amount;
		// put the lower bits of next to higher positions
		cell higher = temp.at(i+1) << (CellBits - amount);
		// jamble them together
		result.push_back(higher | lower);
	}
	// put the highest bits of last digit to lower positions
	cell lower = temp.at(temp.m_cell_amount - 1) >> amount;
	result.push_back(lower);

	return Big(std::move(result));
}


Big Big::operator<< (size_t amount) const
{
	if (this->is_nil())
	{
		return Big(0);
	}
	if (amount % CellBits == 0)
	{
		return this->shift_l(amount / CellBits);
	}

	// shift all cells so remaining shifts are only to neighbooring cells
	Big&& temp = this->shift_l(amount / CellBits);
	amount %= CellBits;

	// TODO: this can be optimized with virtual zeroes
	// but requires writing a new constructor

	init_vect result;
	result.reserve(temp.m_cell_amount);
	result.push_back(temp.at(0) << amount);

	for (size_t i = 1; i < temp.m_cell_amount; ++i)
	{
		// put the highest bits of previous to lower positions
		cell lower = temp.at(i - 1) >> (CellBits - amount);
		// put the lowest bits of current to higher positions
		cell higher = temp.at(i) << amount;
		// jamble them together
		result.push_back(higher | lower);
	}
	cell lower = temp.at(temp.m_cell_amount - 1) >> (CellBits - amount);
	result.push_back(lower);

	return Big(std::move(result));
}


//////////////////////////////////////////////////////////////////////////////


auto barrett_reduce (const Big& modulo)
    -> std::function< Big(const Big& value) >
{
}
