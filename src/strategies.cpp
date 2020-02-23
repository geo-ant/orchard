#include "random_numbers.hpp"
#include "strategies.hpp"
#include <optional>
#include <algorithm>
#include <functional>

namespace orchard
{

//using index_function_t = std::function<std::optional(const game_state&)>;


std::optional<size_t> find_first_non_empty_tree_index(const game_state & g)
{
	auto iter = std::find_if(g.fruit_count.cbegin(),g.fruit_count.cend(), [](auto val){return val>0;});
	if(iter != g.fruit_count.cend())
	{
		return std::distance(g.fruit_count.cbegin(), iter);
	}
	else
	{
		return std::nullopt;
	}
}

game_state pick_fruit_in_order_impl(const game_state & g, const size_t amount)
{
	if(amount == 0 || is_over(g))
	{
		return g;
	}

	auto g_next = g.pick_fruit(find_first_non_empty_tree_index(g).value());

	return pick_fruit_in_order_impl(std::move(g_next),amount-1);
}

game_state pick_fruit_in_order(const game_state & g)
{
	return pick_fruit_in_order_impl(g,2);
}


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

}//namespace orchard
