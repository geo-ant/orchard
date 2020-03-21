#include "orchard.hpp"
#include "generator.hpp"
#include <new>

namespace orchard
{

constexpr long INVALID_POSITION = -1;

//!helper function for initialization of finished game state upon construction
std::optional<game_state> play_to_finish_wrapper(strategy_t strat, const std::optional<game_state> & initial)
{
	if(initial.has_value() && strat)
	{
		return play_to_finish(strat,initial.value());
	}
	else
	{
		return {};
	}
}

game_iterator::game_iterator(strategy_t strat, std::optional<game_state> initial, unsigned int pos)
:strategy(strat), initial_state(initial), final_state(play_to_finish_wrapper(strat,initial)), position(pos)
{}


bool game_iterator::operator ==(const game_iterator& other)
{
	return position.value_or(INVALID_POSITION) == other.position.value_or(INVALID_POSITION);
}

bool game_iterator::operator !=(const game_iterator& other)
{
	return !(*this==other);
}

game_state game_iterator::operator*()
{
	return final_state.value();
}

//prefix: ++it
game_iterator game_iterator::operator++()
{
	final_state.emplace(play_to_finish(strategy,initial_state.value()));
	position = position.value()+1;
	return *this;
}

//postfix: it++
game_iterator game_iterator::operator++(int)
{
	game_iterator copy(*this);
	++(*this);
	return copy;
}

game_iterator game_iterator::create_end_iterator(unsigned int pos)
{
	return game_iterator(strategy_t(), std::nullopt, pos);
}

game_iterator game_iterator::create_begin_iterator(strategy_t strategy, const game_state & initial)
{
	return game_iterator(strategy, initial, 0);
}






} //namespace orchard
