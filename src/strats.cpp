#include "strats.hpp"
#include <optional>
#include <algorithm>

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


game_state in_order_picking_strategy::pick_fruits(game_state && g) const
{
	for(int n = 0; n < 2; n++)
	{
		std::optional<size_t> idx = find_first_non_empty_tree_index(g);
		if(idx)
		{
			g = g.pick_fruit(idx.value());
		}
		else
		{
			break;
		}
	}

	return g;
}

}
