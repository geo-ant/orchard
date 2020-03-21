#include "dice.hpp"
#include "strategies.hpp"
#include <optional>
#include <algorithm>
#include <functional>

namespace orchard
{

//using index_function_t = std::function<std::optional(const game_state&)>;


std::optional<size_t> find_first_non_empty_tree_index(const tree_array_t & fruit_count)
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

tree_array_t pick_fruit_in_order_impl(const tree_array_t & fruit_count, const size_t amount)
{
	std::optional<size_t> opt_idx = find_first_non_empty_tree_index(fruit_count);

	if(amount == 0 || !opt_idx.has_value())
	{
		return fruit_count;
	}

	auto new_count = fruit_count;
	new_count.at(opt_idx.value()) -= 1;

	return pick_fruit_in_order_impl(new_count,amount-1);
}

tree_array_t pick_fruit_in_order(const tree_array_t & fruit_count)
{
	return pick_fruit_in_order_impl(fruit_count,2);
}

/*
game_state pick_fruit_at_random_impl(const game_state & g, const size_t amount)
{
	if(amount == 0 || is_over(g))
	{
		return g;
	}

	size_t fruit_index = get_uniform_random_number<size_t>(0,TREE_COUNT-1);

	if(g.fruit_count.at(fruit_index)>0)
	{
		auto g_next = g.pick_fruit(fruit_index);
		return pick_fruit_at_random_impl(g_next, amount -1);
	}
	else
	{
		return pick_fruit_at_random_impl(g, amount);
	}
}

game_state pick_fruit_at_random(game_state && g)
{
	return pick_fruit_at_random_impl(g,2);
}
*/

}//namespace orchard
