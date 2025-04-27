#ifndef __FUNC_TABLE
#define __FUNC_TABLE

#include <boost/dynamic_bitset.hpp>
#include <vcruntime.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <exception>
#include "func.h"
#include "xvar.h"

template<typename _Chank = std::size_t, typename _Ix_Size = int>
class func_table : public func<_Chank>
{
public:
	using _xvarset = std::vector<xvar<_Ix_Size>>;
	using _func = func<_Chank>;

	// requires _TyF is uintXX_t type from std::
	template<typename _TyF = std::uint16_t>
	func_table(_TyF f) : _func(f) {
		_initial_default_table();
		_index_vars = default_index_var_set<_Ix_Size>(this->rang());
	}
	// requires _TyF is uintXX_t type from std::
	// throws: std::invalid_argument
	template<typename _TyF = std::uint16_t>
	func_table(_TyF f, const _xvarset& xvars) : _func(f) {
		_check_xvarset(xvars);
		_initial_default_table();
		_index_vars = to_index_var_set(xvars);
	}

	// requires prefix 0x for hexadecimal notation or 0b for binary notation
	func_table(std::string f) : _func(f) {
		_initial_default_table();
		_index_vars = default_index_var_set<_Ix_Size>(this->rang());
	}
	// requires prefix 0x for hexadecimal notation or 0b for binary notation
	// throws: std::invalid_argument
	func_table(std::string f, const _xvarset& xvars) : _func(f) {
		_check_xvarset(xvars);
		_initial_default_table();
		_index_vars = to_index_var_set(xvars);
	}

	func_table(func f) : _func(f) noexcept {
		_initial_default_table();
		_index_vars = default_index_var_set<_Ix_Size>(this->rang());
	}
	// throws: std::invalid_argument
	func_table(func f, const _xvarset& xvars) : _func(f) {
		_check_xvarset(xvars);
		_initial_default_table();
		_index_vars = default_index_var_set<_Ix_Size>(this->rang());
	}

public:

	_bitset raw_table() const noexcept { return this->_bit_table; }
	_func func() const noexcept { return _func(this->raw_func()); }

	template<std::size_t _size_box = 4>
	void print_table() const noexcept {
		const std::size_t size = this->size();
		const std::size_t rang = this->rang();

		std::size_t text_box = std::to_string(rang).size() + 1;
		for (std::size_t i = rang; i != 0; i--) {
			std::cout << std::setw((_size_box - text_box) / 2 + text_box) << "x"
				+ std::to_string(_index_vars[i - 1]) << std::setw((_size_box - text_box) / 2) << " " << "|";
		}
		std::cout << std::setw((_size_box - text_box) / 2 + text_box) << " F" << std::endl;
		std::cout << std::string((_size_box + 1) * (rang + 1) - 1, '-') << std::endl;

		for (std::size_t i = 0; i < size; i++) {
			for (std::size_t j = rang; j != 0; j--) {
				std::cout << std::setw((_size_box - text_box) / 2 + text_box) <<
					_bit_table[i * rang + (j - 1)] << std::setw((_size_box - text_box) / 2) << " " << "|";
			}
			std::cout << std::setw((_size_box - text_box) / 2 + text_box) << this->_bit_func[this->size() - i - 1] << std::endl;
		}
	}

	// swaps variables with specified indexes in a table and normalizes the table
	// throws: std::invalid_argument
	void reorder_vars(const std::size_t index1, const std::size_t index2) {
		if (index1 > this->rang())
			throw std::invalid_argument("first index is greater than the rank of the function: "  + std::to_string(index1));
		if (index2 > this->rang())
			throw std::invalid_argument("second index is greater than the rank of the function: " + std::to_string(index2));
		if (index1 == index2)
			return;

		_xvarset xvar_set = defalut_vars_set(this->rang());
		_bitset new_bit_func(this->size());
		std::swap(xvar_set[index1], xvar_set[index2]);
		std::swap(_index_vars[index1], _index_vars[index2]);
		
		for (std::size_t i = 0; i < this->size(); i++) {
			for (std::size_t j = 0; j < xvar_set.size(); j++) {
				xvar_set[j].val = i & static_cast<std::size_t>(std::pow(2, j));
			}
			new_bit_func[i] = this->result_on_set(xvar_set);
		}
		this->_bit_func = new_bit_func;
	}
	// swaps columns of variables at given indexes.
	// throws: std::invalid_argument
	void exchange_vars(const std::size_t index1, const std::size_t index2) {
		if (index1 > this->rang())
			throw std::invalid_argument("first index is greater than the rank of the function: "  + std::to_string(index1));
		if (index2 > this->rang())
			throw std::invalid_argument("second index is greater than the rank of the function: " + std::to_string(index2));
		if (index1 == index2)
			return;

		std::swap(_index_vars[index1], _index_vars[index2]);
		for (std::size_t i = 0; i < this->size(); i++) {
			bool tmp = _bit_table[i * this->rang() + index1];
			_bit_table[i * this->rang() + index1] = _bit_table[i * this->rang() + index2];
			_bit_table[i * this->rang() + index2] = tmp;
		}
	}
	// normalizes the table without changing the order of variables
	void normalize() noexcept {
		_bitset new_bit_func(this->size());
		std::size_t func_address;

		for (std::size_t i = 0; i < this->size(); i++) {
			func_address = 0;
			for (std::size_t j = 0; j < _index_vars.size(); j++) {
				func_address += _bit_table[i * this->rang() + j] * static_cast<std::size_t>(std::pow(2, j));
			}
			new_bit_func[func_address] = _bit_func[i];
		}
		this->_bit_func = new_bit_func;

		// normalize table (rebuild all table)
		for (std::size_t i = 0; i < this->size(); i++)
			for (std::size_t j = this->rang(); j != 0; j--) {
				_bit_table[i * this->rang() + (j - 1)] = i & static_cast<std::size_t>(std::pow(2, (j - 1)));
			}
	}
	// Returns a function equivalent to a column of a variable with the specified index on little-endian bits order
	// throws: std::invalid_argument
	_func func_var(const std::size_t index) const {
		if (_index_vars.size() < index)
			throw std::invalid_argument("index is greater than the rank of the function");

		std::size_t _index = 0;
		for (std::size_t i = 0; i < _index_vars.size(); i++)
			if (index == _index_vars[i])
				_index = i;

		_func fvar(_bitset(this->size()));
		for (std::size_t i = 0; i < this->size(); i++)
			fvar.set(i, _bit_table[this->rang() * i + _index]);
		fvar.reverse();
		return fvar;
	}
	//
	// throws: std::invalid_argument
	std::size_t order_index_var(std::size_t index) const {
		if (_index_vars.size() - 1 < index)
			throw std::invalid_argument("index is greater than the rank of the function");
		return _index_vars[index];
	}
	//
	_xvarset xvarset() const noexcept {
		return to_xvar_set(_index_vars);
	}

private:
	// initialization of the normalized truth table for the entered function
	void _initial_default_table() noexcept {
		const std::size_t size = this->size();
		const std::size_t rang = this->rang();
		_bit_table.resize(size * rang);

		for (std::size_t i = 0; i < size; i++)
			for (std::size_t j = rang; j != 0 ; j--) {
				_bit_table[i * rang + (j - 1)] = i & static_cast<std::size_t>(std::pow(2, (j - 1)));
			}
	}
	// throws: std::invalid_argument
	void _check_xvarset(const _xvarset& xvarset) const {
		if (xvarset.size() != this->rang())
			throw std::invalid_argument("incorrect set size");
		for (auto iter : xvarset)
			if (iter.index > this->rang())
				throw std::invalid_argument("incorrect argument on set: x" + std::to_string(iter.index));
	}

private:
	boost::dynamic_bitset<_Chank> _bit_table;
	std::vector<_Ix_Size> _index_vars;
};

#endif