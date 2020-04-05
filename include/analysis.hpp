#ifndef _ANALYSIS_HPP_INCLUDED
#define _ANALYSIS_HPP_INCLUDED

#include "orchard.hpp"
#include "generator.hpp"
#include <numeric>
#include <functional>
#include <vector>
#include <thread>
#include <future>
#include <execution>
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
statistic_t accumulate_statistics(strategy_t strategy, size_t amount, statistic_t initial_statistics, const game_state initial = game_state())
{
	game_generator games(strategy, initial, amount);
	auto result = std::transform_reduce(games.cbegin(),games.cend(), initial_statistics, std::plus<statistic_t>(), [](const game_state & g){return statistic_t(g);});
	return result;
}

//! parallel version of the above
//todo: make better with execution policy as parameter
//todo: ugly and not functional programming -> make better (divide range in number of elements)
template<typename statistic_t>
statistic_t accumulate_statistics_par(strategy_t strategy, size_t amount, statistic_t initial_statistics, const game_state initial = game_state())
{
	size_t nthreads = std::thread::hardware_concurrency();

	if(nthreads >= amount || nthreads <= 1)
	{
		return accumulate_statistics<statistic_t>(strategy,amount,initial_statistics,initial);
	}

	std::vector<std::future<statistic_t>> results;
	for(size_t j = 0; j < nthreads-1; ++j)
	{
		results.push_back(std::async(std::launch::async,accumulate_statistics<statistic_t>,strategy,amount/(nthreads-1),initial_statistics,initial));
	}
	results.push_back(std::async(std::launch::async,accumulate_statistics<statistic_t>,strategy,amount%(nthreads-1),initial_statistics,initial));


	statistic_t result_stat = std::transform_reduce(results.begin(),results.end(),initial_statistics,std::plus<statistic_t>(),[](std::future<statistic_t> & f){return f.get();});
	return result_stat;
}
	 
} //namespace orchard

#endif //_ANALYSIS_HPP_INCLUDED
