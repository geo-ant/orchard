#ifndef _ANALYSIS_HPP_INCLUDED
#define _ANALYSIS_HPP_INCLUDED

#include "orchard.hpp"
#include "generator.hpp"

namespace orchard
{

template<typename statistic_t>
statistic_t accumulate_statistic(strategy_t strategy, const game_state & initial, size_t amount)
{
	game_generator games(strategy, initial, amount);
	
	
}
	 
}

#endif //_ANALYSIS_HPP_INCLUDED