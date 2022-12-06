#ifndef VARIANT_UTILS_HPP
#define VARIANT_UTILS_HPP

template <class... Ts> struct overloaded : Ts... {
	using Ts::operator()...;
};
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
#endif