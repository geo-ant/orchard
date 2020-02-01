#include "orchard.hpp"
#include "generator.hpp"


namespace orchard
{

constexpr long INVALID_POSITION = -1;


game_iterator::game_iterator(std::function<game_state(game_state&&)> strat, std::optional<game_state> initial, unsigned int pos)
:strategy(strat), initial_state(initial), position(pos)
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
	return play_to_finish(strategy, std::move(initial_state.value()));
}

game_iterator& game_iterator::operator++()
{
	position = position.value()+1;
	return *this;
}

game_iterator game_iterator::operator++(int) const
{
	return game_iterator(strategy, initial_state.value(), position.value()+1);
}

game_iterator game_iterator::create_end_iterator(unsigned int pos)
{
	return game_iterator(std::function<game_state(game_state&&)>(), std::nullopt, pos);
}

game_iterator game_iterator::create_begin_iterator(std::function<game_state(game_state&&)> strategy, const game_state & initial)
{
	return game_iterator(strategy, initial, 0);
}






} //namespace orchard
