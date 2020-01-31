#include "analysis.hpp"

namespace orchard
{
game_generator::game_generator(strategy_t strat, game_state initial, size_t cnt)
:begin_it(game_iterator::create_begin_iterator(strat,initial)),
end_it(game_iterator::create_end_iterator(cnt))
{}

game_iterator game_generator::cbegin()
{
	return begin_it;
}
	
game_iterator game_generator::cend()
{
	return end_it;
}
}