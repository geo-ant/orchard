#ifndef _RANDOM_NUMBERS_HPP_INCLUDED
#define _RANDOM_NUMBERS_HPP_INCLUDED

#include <chrono>
#include <random>
#include <thread>

/**
 * generate uniform random numbers in a thread safe fashion
 * see: https://stackoverflow.com/questions/21237905/how-do-i-generate-thread-safe-uniform-random-numbers?answertab=active#tab-top
*/
template<typename T>
T get_uniform_random_number(T minimum, T maximum)
{
	using thread_id_hash = std::hash<std::thread::id>;
	static thread_local std::random_device rd;
	static thread_local T seed = rd() + std::chrono::system_clock::now().time_since_epoch().count() + thread_id_hash()(std::this_thread::get_id());
	static thread_local std::mt19937 generator(seed);
	std::uniform_int_distribution<T> distribution(minimum,maximum);
	return distribution(generator);
}


#endif //_MISC_HPP_INCLUDED
