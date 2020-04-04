#include <iostream>
#include <algorithm>
#include <numeric>
#include "orchard.hpp"


namespace orchard
{
	//! return true iff the game was won by the players
	bool is_won(const game_state & g)
	{
		return g.get_total_fruit_count()==0;
	}

	//! return true iff the game was won by the raven
	bool is_lost(const game_state& g)
	{
		return g.raven_count >= MAX_RAVEN_COUNT && !is_won(g);
	}

	//! return true iff the game is over (won or lost)
	bool is_over(const game_state & g)
	{
		return is_won(g) || is_lost(g);
	}

}

std::ostream & operator<<(std::ostream & os, const orchard::game_state & g)
{
	std::cout << "Fruits: [";
	std::for_each(g.fruit_count.cbegin(), g.fruit_count.cend()-1, [](const auto v){std::cout << v << " ";});
	std::cout << *(g.fruit_count.cend()-1);
	std::cout << "], Raven: " << g.raven_count;
	return os;
}
