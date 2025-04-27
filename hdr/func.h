#ifndef __FUNC
#define __FUNC

#include <boost/dynamic_bitset.hpp>
#include <stdint.h>
#include <vcruntime.h>
#include <iostream>
#include <string>
#include <exception>
#include <math.h>
#include <vector>
#include "xvar.h"

static constexpr const char* const PREFIX16 = "0x";
static constexpr const char* const PREFIX2 =  "0b";
static const char TABLE16[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
static const char  TABLE2[] = { '0', '1' };

template<typename _Chank = std::size_t>
class func
{
public:
	using _bitset = boost::dynamic_bitset<_Chank>;

	func(std::uint64_t f) noexcept : _bit_func(sizeof(std::uint64_t) * 8ui64, f) {}
	func(std::uint32_t f) noexcept : _bit_func(sizeof(std::uint32_t) * 8ui32, f) {}
	func(std::uint16_t f) noexcept : _bit_func(sizeof(std::uint16_t) * 8ui16, f) {}
	func(std::uint8_t  f) noexcept : _bit_func(sizeof(std::uint8_t ) * 8ui8 , f) {}

	// Requires prefix 0x for hexadecimal notation or 0b for binary notation
	// throws: std::invalid_argument
	func(std::string f) {
		std::size_t fsize = f.size() - 2;
		std::size_t f2size = static_cast<std::size_t>(std::log2(fsize));
		if (std::pow(2, f2size) != fsize)
			throw std::invalid_argument("incorrect function size");

		if (PREFIX16[0] == f[0] && PREFIX16[1] == f[1]) {
			_bit_func.resize(fsize * 4);
			for (std::size_t i = 2; i < f.size(); i++) {
				bool function_cast = false;
				for (std::size_t j = 0; j < sizeof(TABLE16); j++) {
					if (TABLE16[j] == f[i]) {
						_bit_func.set(4 * (fsize - i) + 4, 0b0001 & j);
						_bit_func.set(4 * (fsize - i) + 5, 0b0010 & j);
						_bit_func.set(4 * (fsize - i) + 6, 0b0100 & j);
						_bit_func.set(4 * (fsize - i) + 7, 0b1000 & j);
						function_cast = true;
						break;
					}
				}
				if (!function_cast)
					throw std::invalid_argument("incorrect function symbol");
			}
		}
		else if (PREFIX2[0] == f[0] && PREFIX2[1] == f[1]) {
			_bit_func.resize(fsize);
			for (std::size_t i = 2; i < f.size(); i++) {
				if (TABLE2[0] == f[i])
					_bit_func.set(fsize - i + 1, 0);
				else if (TABLE2[1] == f[i])
					_bit_func.set(fsize - i + 1, 1);
				else
					throw std::invalid_argument("incorrect function symbol");
			}
		}
		else throw std::invalid_argument("incorrect function prefix");
	}

	// The bitset size must be a power of 2
	// throws: std::invalid_argument
	func(const _bitset& f) {
		const std::size_t f2size = static_cast<std::size_t>(std::log2(f.size()));
		if (std::pow(2, f2size) != f.size())
			throw std::invalid_argument("incorrect set size");

		_bit_func = f;
	}

protected:
	// constructor without checking for the degree of 2. bool value may be any
	func(_bitset f, bool) noexcept : _bit_func(f) {}

public:

	func& reverse() noexcept {
		for (std::size_t i = 0; i < _bit_func.size() / 2; i++) {
			bool tmp = _bit_func[i];
			_bit_func[i] = _bit_func[_bit_func.size() - i - 1];
			_bit_func[_bit_func.size() - i - 1] = tmp;
		}
		return *this;
	}

	// Returns a copy of the bit representation of the function
	inline _bitset raw_func() const noexcept { return _bit_func; }
	inline std::size_t size() const noexcept { return _bit_func.size(); }
	inline std::size_t rang() const noexcept { return static_cast<std::size_t>(std::log2(_bit_func.size())); }
	inline bool operator[](const std::size_t i) const { return _bit_func[i]; }
	// throws: std::out_of_range
	bool at(const std::size_t i) const {
		if (0 > i || i > _bit_func.size())
			throw std::out_of_range("index out of range function");
		return _bit_func[i];
	}
	//
	inline func& set() noexcept {
		_bit_func.set();
		return *this;
	}
	//
	// throws: std::out_of_range
	inline func& set(std::size_t i, bool value = true) {
		if (0 > i || i > _bit_func.size())
			throw std::out_of_range("index out of range function");
		_bit_func.set(i, value);
		return *this;
	}
	//
	inline func& reset() noexcept {
		_bit_func.reset();
		return *this;
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! add left/right expansion function
	// The new size must be a number equal to the power of 2, otherwise an exception is std::invalid_argument. 
	// If the size is larger than the current one, then new bits are added on the left. 
	// If the size is smaller, then the bits are truncated on the left.
	// Note: value is a placeholder
	// throws: std::invalid_argument
	func& resize(const std::size_t sz, bool value = false) const {
		std::size_t f2size = static_cast<std::size_t>(std::log2(sz));
		if (std::pow(2, f2size) != sz)
			throw std::invalid_argument("incorrect function size");

		const_cast<_bitset*>(&_bit_func)->resize(static_cast<_Chank>(sz), value);
		return *const_cast<func*>(this);
	}

	// 
	func sub_func(const std::size_t index1, const std::size_t index2, bool defult_val = false) const noexcept {
		_bitset _bs(size());
		func f(_bs);
		for (std::size_t i = index1; i < index2 + 1; i++)
			f.set(i, _bit_func[i]);
		return f;
	}

	// 
	func right_half() const noexcept {
		func f(_bit_func, true);
		f.resize(f.size() / 2);
		for (std::size_t i = 0; i < _bit_func.size() / 2; i++) {
			f.set(i, _bit_func[i]);
		}
		return f;
	}
	// 
	func left_half() const noexcept {
		func f(_bit_func, true);
		f.resize(f.size() / 2);
		for (std::size_t i = _bit_func.size() / 2; i < _bit_func.size(); i++) {
			f.set(i - _bit_func.size() / 2, _bit_func[i]);
		}
		return f;
	}
	
	inline bool equal0() const noexcept { return _bit_func.none(); }
	inline bool equal1() const noexcept { return _bit_func.all(); }

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool operator==(const func& f) { return f._bit_func == _bit_func; }
	inline bool operator& (const func& f) { return f._bit_func &  _bit_func; }
	inline bool operator| (const func& f) { return f._bit_func |  _bit_func; }
	inline bool operator^ (const func& f) { return f._bit_func ^  _bit_func; }
	inline bool operator!=(const func& f) { return f._bit_func != _bit_func; }

	//
	func flip() noexcept {
		_bit_func.flip();
		return *this;
	}
	// The value of a function on a given set of variables (required to use xvar struct). The variables can be in any order.
	// throws: std::invalid_argument
	template<typename _Ix_Size = int>
	bool result_on_set(const std::vector<xvar<_Ix_Size>>& vars) const {
		const std::size_t f2size = static_cast<std::size_t>(std::log2(_bit_func.size()));
		if (std::pow(2, f2size) != _bit_func.size())
			throw std::invalid_argument("incorrect set size");

		std::size_t func_index = 0;
		for (std::size_t i = 0; i < vars.size(); i++) {
			if (vars[i].val) {
				if (f2size <= vars[i].index)
					throw std::invalid_argument("incorrect argument on set: x" + std::to_string(vars[i].index));
				func_index += static_cast<std::size_t>(std::pow(2, vars[i].index));
			}
		}

		return _bit_func[func_index];
	}

	std::string to_hex_string() {
		std::size_t num_bytes = (_bit_func.size() + 7) / 8;
		char* hex_str = new char[num_bytes * 2 + 1];
		hex_str[num_bytes * 2] = '\0';

		for (std::size_t i = 0; i < num_bytes * 8; i += 4) {
			std::uint8_t byte = 0;
			for (std::size_t j = 0; j < 4; j++)
				byte |= (_bit_func[_bit_func.size() - i - j - 1] << 3 - j);

			if (0 <= byte && 9 >= byte)
				hex_str[i / 4] = byte + static_cast<std::uint8_t>('0');
			else if (10 <= byte && 15 >= byte)
				hex_str[i / 4] = byte + static_cast<std::uint8_t>('A') - 10;
		}

		std::string hex_string(hex_str);
		delete[] hex_str;
		return hex_string;
	}

	friend std::ostream& operator<<(std::ostream& os, const func<_Chank>& f) {
		os << f._bit_func;
		return os;
	}

protected:
	_bitset _bit_func;
};

#endif // !__FUNC