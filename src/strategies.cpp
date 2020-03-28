#include "dice.hpp"
#include "strategies.hpp"
#include <optional>
#include <algorithm>
#include <functional>
#include <limits>

namespace orchard
{

/**
 * Helper function to generate picking strategies
 * @param Func a function that chooses an index in the array by some
 * means. Can be of type Func: const tree_array_t & -> size_t
 * or Func: const tree_array_t & -> std::optional<size_t>
 * @returns if the amount is zero, or the total fruit count
 * is zero the choose_index function will not be invoked and
 * fruit_count will be returned. If choose_index returns a
 * nullopt then also fruit_count will be returned. Otherwise
 * returns the result of the picking strategy applied twice.
 * If choose_index points to an empty tree, the choose_index
 * is applied again. So returning the index of an empty tree
 * for choose index is fine as long as one of the next invocations
 * returns a different index (useful for random). If choose_index
 * always points to an empty tree but the other trees are not empty,
 * this will lead to an infinite loop, possibly a stack overflow.
 */
template<typename Func>
tree_array_t pick_fruit_impl(Func && choose_index, const tree_array_t & fruit_count, const size_t amount)
{
	auto total_fruit_count = std::accumulate(fruit_count.cbegin(), fruit_count.cend(),0);
	if(amount==0 || total_fruit_count<=0)
	{
		return fruit_count;
	}

	std::optional<size_t> opt_idx = choose_index(fruit_count);

	if(!opt_idx.has_value())
	{
		return fruit_count;
	}

	tree_array_t new_fruit_count = fruit_count;
	size_t new_amount = amount;
	if(fruit_count.at(*opt_idx)>0)
	{
		new_fruit_count.at(*opt_idx) -= 1;
		new_amount -=1;
	}
	return pick_fruit_impl(std::forward<Func>(choose_index),new_fruit_count, new_amount);
}

/**
 * Index chooser functions
 */


std::optional<size_t> choose_first_non_empty_tree_index(const tree_array_t & fruit_count)
{
	auto iter = std::find_if(fruit_count.cbegin(),fruit_count.cend(), [](auto val){return val>0;});
	if(iter != fruit_count.cend())
	{
		return std::distance(fruit_count.cbegin(), iter);
	}
	else
	{
		return std::nullopt;
	}
}


size_t choose_random_tree_index(const tree_array_t fruit_count)
{
	return get_uniform_random_number<size_t>(0,fruit_count.size()-1);
}

size_t choose_fullest_tree_index(const tree_array_t & fruit_count)
{
	auto iter = std::max_element(fruit_count.cbegin(), fruit_count.cend());
	return std::distance(fruit_count.cbegin(), iter);
}

//chose tree with least amount of fruit BUT NONZERO!
size_t choose_emptiest_tree_index(const tree_array_t & fruit_count)
{
	tree_array_t temp;
	using number_type = tree_array_t::value_type;
	std::transform(fruit_count.cbegin(), fruit_count.cend(), temp.begin(),[](number_type num){return (num > 0)? num : std::numeric_limits<number_type>::max();});
	auto iter = std::min_element(temp.cbegin(), temp.cend());
	return std::distance(temp.cbegin(), iter);
}

/**
 * Implementations for the picking strategies
 */

tree_array_t pick_fruit_in_order(const tree_array_t & fruit_count)
{
	return pick_fruit_impl(&choose_first_non_empty_tree_index,fruit_count,2);
}


tree_array_t pick_fruit_at_random(const tree_array_t & fruit_count)
{
	return pick_fruit_impl(&choose_random_tree_index,fruit_count,2);
}

tree_array_t pick_fruit_from_fullest_tree(const tree_array_t & fruit_count)
{
	return pick_fruit_impl(&choose_fullest_tree_index,fruit_count,2);
}

tree_array_t pick_fruit_from_emptiest_tree(const tree_array_t & fruit_count)
{
	return pick_fruit_impl(&choose_emptiest_tree_index,fruit_count,2);
}


}//namespace orchard
