#ifndef _RANDOM_NUMBERS_HPP_INCLUDED
#define _RANDOM_NUMBERS_HPP_INCLUDED

#include "orchard.hpp"

#include <chrono>
#include <random>
#include <thread>
#include <string>

class ostream; //forward declaration

namespace orchard
{

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


class dice_result
{
public:
	static constexpr int FRUIT_BASKET = TREE_COUNT;
	static constexpr int RAVEN = TREE_COUNT+1;

	/**
	 * construct a dice result corresponding
	 * to a certain number. The numbers [0, TREE_COUNT-1]
	 * correspond to indices, the number TREE_COUNT is the
	 * fruit basket and the number TREE_COUNT+1 is the raven.
	 * Constructor throws if the given number is illegal.
	 *
	 * Use the static member create_random() to obtain a random
	 * dice result.
	 *
	 * This constructor is handy for testing.
	 */
	dice_result(int res);

	//!returns true iff the dice result is a tree index
	bool is_tree_index() const;

	//!returns true iff the dice result is the fruit basket
	bool is_fruit_basket() const;

	//!returns true iff the dice result is a raven
	bool is_raven() const;

	//!create a random dice result
	static dice_result create_random();

	/**
	 * Returns the index of the tree that the dice picked
	 * if is_tree_index()==true. Otherwise throws an exception
	 * @throws std::logic_error if is_tree_index()==false
	 * @return the tree index
	 */
	int get_tree_index() const;

	std::string to_string() const;

private:

	const int result;
};

//std::ostream & operator<<(std::ostream & os, const orchard::dice_result & d);


}//namespace orchard

//! operator for stream output



#endif //_MISC_HPP_INCLUDED
