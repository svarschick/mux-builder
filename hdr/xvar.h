#ifndef __XVAR
#define __XVAR

#include <vector>

template<typename _Ix_Size = int>
struct xvar {
	_Ix_Size index;
	bool val;

	inline bool operator<(const xvar& val) const noexcept {
		return this->index < val.index;
	}
};

template<typename _Ix_Size = int>
std::vector<xvar<_Ix_Size>> defalut_vars_set(const std::size_t rang, bool default_val = false) noexcept {
	std::vector<xvar<_Ix_Size>> vars(rang);
	for (std::size_t i = 0; i < rang; i++)
		vars[i] = { static_cast<_Ix_Size>(i), default_val };
	return vars;
}
template<typename _Ix_Size = int>
std::vector<_Ix_Size> default_index_var_set(const std::size_t rang) noexcept {
	std::vector<_Ix_Size> vars(rang);
	for (std::size_t i = 0; i < rang; i++)
		vars[i] = { static_cast<_Ix_Size>(i) };
	return vars;
}
template<typename _Ix_Size = int>
std::vector<_Ix_Size> to_index_var_set(const std::vector<xvar<_Ix_Size>>& xvarset) noexcept {
	std::vector<_Ix_Size> vars(xvarset.size());
	for (std::size_t i = 0; i < xvarset.size(); i++)
		vars[i] = xvarset[i].index;
	return vars;
}
template<typename _Ix_Size = int>
std::vector<xvar<_Ix_Size>> to_xvar_set(const std::vector<_Ix_Size>& index_var_set) noexcept {
	std::vector<xvar<_Ix_Size>> vars = defalut_vars_set(index_var_set.size());
	for (std::size_t i = 0; i < index_var_set.size(); i++)
		vars[i].index = index_var_set[i];
	return vars;
}

#endif