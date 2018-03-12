#ifndef COMPARE_HPP
# define COMPARE_HPP

namespace		fx
{
	template <typename T>
	using Compare = bool (*)(const T&, const T&);
};

#endif