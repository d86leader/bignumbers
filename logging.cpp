#include <iostream>

template<typename A>
auto old_basic_print(std::ostream& out, const A& last) -> std::ostream&
{
	return out << last;
}
template<typename A, typename... Args>
auto old_basic_print(std::ostream& out, const A& first, Args&&... rest) -> std::ostream&
{
	out << first;
	return old_basic_print(out, std::forward<Args>(rest)...);
}
auto old_basic_print(std::ostream& out) -> std::ostream&
{
	return out;
}

template<typename... Args>
auto print(Args&&... args) -> std::ostream&
{
	return old_basic_print(std::cout, std::forward<Args>(args)...) << std::endl;
}
template<typename... Args>
auto err(Args&&... args) -> std::ostream&
{
	return old_basic_print(std::cerr, std::forward<Args>(args)...) << std::endl;
}
