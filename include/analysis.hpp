#ifndef _ANALYSIS_HPP_INCLUDED
#define _ANALYSIS_HPP_INCLUDED

#include "orchard.hpp"
#include "generator.hpp"
#include <numeric>
#include <functional>
//#include <execution>

namespace orchard
{

/**
* Calculate the statistics for a given amount of
* random games independently and accumulate the statistics
* for all games.
* 
* See sample statistics for further examples.
* @param strategy The strategy with which to play the game
* @param amount The total number of games to simulate.
* @param initial_statistics An intitial statistic that is 
* the first element of the generalized zum of statistics.
* A statistic_t must satisfy the following:
* -have a constructor taking a game (passed as const lvalue ref)
* -have a an operator + defined which is associative and commutative
* -have a default constructor that returns an "zero" statistic
* which acts as a neutral element for operator +. 
* @param initial The initial game state from which all random
* games are generated.
*/
template<typename statistic_t>
statistic_t accumulate_statistics(strategy_t strategy, size_t amount, const game_state & initial = game_state(), statistic_t initial_statistics = statistic_t())
{
	game_generator games(strategy, initial, amount);
	auto result = std::transform_reduce(games.cbegin(),games.cend(), initial_statistics, std::plus<statistic_t>(), [](const game_state & g){return statistic_t(g);});
	return result;
}
	 
} //namespace orchard

#endif //_ANALYSIS_HPP_INCLUDED
