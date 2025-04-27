#ifndef __ELEMENT
#define __ELEMENT

#include <utility>

static std::size_t GLOBAL_ELEMENT_INDEX = 0;
class element
{
public:
	element() : INDEX(++GLOBAL_ELEMENT_INDEX) {}
	const std::size_t index() const noexcept { return INDEX; }

private:
	const std::size_t INDEX;
};

#endif