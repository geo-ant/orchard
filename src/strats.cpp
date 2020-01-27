#include "strats.hpp"
#include <optional>
#include <algorithm>
#include "random_numbers.hpp"

namespace orchard
{

std::optional<size_t> find_first_non_empty_tree_index(const game_state & g)
{
	auto iter = std::find_if(g.fruit_cbegin(),g.fruit_cend(), [](auto val){return val>0;});
	if(iter != g.fruit_cend())
	{
		return std::distance(g.fruit_cbegin(), iter);
	}
	else
	{
		return std::nullopt;
	}
}

game_state pick_fruit_in_order(game_state && g, const size_t amount)
{
	if(amount == 0 || is_over(g))
	{
		return g;
	}

	auto g_next = g.pick_fruit(find_first_non_empty_tree_index(g).value());

	return pick_fruit_in_order(std::move(g_next),amount-1);
}

game_state pick_fruit_at_random(game_state && g, const size_t amount)
{
	if(amount == 0 || is_over(g))
	{
		return g;
	}

	size_t fruit_index = get_uniform_random_number<size_t>(0,TREE_COUNT-1);

	if(g.get_fruit_count_at(fruit_index)>0)
	{
		auto g_next = g.pick_fruit(fruit_index);
		return pick_fruit_at_random(std::move(g_next), amount -1);
	}
	else
	{
		return pick_fruit_at_random(std::move(g), amount);
	}
}

game_state in_order_picking_strategy::pick_fruits(game_state && g) const
{
	return pick_fruit_in_order(std::move(g),2);
}

game_state random_picking_strategy::pick_fruits(game_state && g) const
{
	return pick_fruit_at_random(std::move(g),2);
}

}
