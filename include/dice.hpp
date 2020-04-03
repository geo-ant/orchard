#ifndef _DICE_HPP_INCLUDED
#define _DICE_HPP_INCLUDED

#include "orchard.hpp"

#include <chrono>
#include <random>
#include <thread>
#include <string>
#include <optional>
#include <variant>

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


/**
 * template based implementation of dice result
 */

//possible result categories for a die
enum class dice_t {TREE_INDEX, FRUIT_BASKET,RAVEN};

/**
 * class template for a dice result
 * the specializations below cover all cases
 */
template<dice_t type>
struct dice_result
{

};

//! a dive throw with a raven
template<>
struct dice_result<dice_t::RAVEN>
{
	std::string to_string() const
	{
		return "R";
	}
};

//! a fruit basket throw
template<>
struct dice_result<dice_t::FRUIT_BASKET>
{
	std::string to_string() const
	{
		return "F";
	}
};

//! a throw corresponding to a tree index
template<>
struct dice_result<dice_t::TREE_INDEX>
{
	/**
	 * construct dice result with
	 * given tree index. If tree index
	 * is invalid then the constructor
	 * throws an exception;
	 */
	explicit dice_result(size_t idx) : tree_index(idx)
	{
		if(tree_index >= TREE_COUNT)
		{
			throw std::logic_error("Dice result outside allowed range!");
		}
	}

	std::string to_string() const
	{
		return std::to_string(tree_index);
	}

	//! the tree index
	const size_t tree_index;
};


using dice_result_variant = std::variant< dice_result<dice_t::RAVEN>,dice_result<dice_t::FRUIT_BASKET>,dice_result<dice_t::TREE_INDEX> >;

/**
 * Helper function that returns
 * the result of the member function
 * to_string for any stored variant
 */
std::string to_string(const dice_result_variant &);


/**
 * Creates a random throw of a die. Returns a variant of
 * dice result (either raven, fruit basket or a result
 * corresponding to a tree index).
 */
dice_result_variant create_random_dice_result();

}//namespace orchard

//! operator for stream output



#endif //_DICE_HPP_INCLUDED
