#include "orchard.hpp"

#include <stdexcept>


/**
 * create an array std::array<T,N> filled with the
 * given value (at compile time or runtime)
 */
template<typename T, size_t N>
constexpr std::array<T,N> create_filled_array(T value)
{
	std::array<T,N> arr;
	for(auto & elem : arr)
	{
		elem = value;
	}
	return arr;
}

namespace orchard
{

game_state::game_state() : fruit_count(create_filled_array<unsigned int, TREE_COUNT>(INITIAL_FRUIT_COUNT)), raven_count(INITIAL_RAVEN_COUNT)
{}

game_state::game_state(std::array<unsigned int, TREE_COUNT> && fruits, unsigned int ravens, unsigned int turns)
: fruit_count(std::move(fruits)),
raven_count(std::move(ravens)),
turn_count(turns)
{}

game_state::game_state(const std::array<unsigned int, TREE_COUNT> & fruits, unsigned int ravens, unsigned int turns)
: fruit_count(fruits)
,raven_count(std::move(ravens)),
turn_count(turns)
{}

/*
 * Pick a number of fruits from a tree
 * @returns new game_state state with the correct amount of fruits after picking
 */
game_state game_state::pick_fruit(size_t tree_index, unsigned int amount) const &
{

	std::array<unsigned int, TREE_COUNT> new_fruit_count(fruit_count);

	if(new_fruit_count.at(tree_index)>= amount)
	{
		new_fruit_count[tree_index]-= amount;
	}
	else
	{
		new_fruit_count[tree_index] = 0;
	}

	return game_state(std::move(new_fruit_count), raven_count, turn_count+1);
}

const game_state & game_state::pick_fruit(size_t tree_index, unsigned int amount) const &&
{
	if(fruit_count.at(tree_index)>= amount)
	{
		fruit_count[tree_index]-= amount;
	}
	else
	{
		fruit_count[tree_index] = 0;
	}
	++turn_count;
	return *this;
}

/*
 * Add a raven card to the game_state
 * @returns new game_state state with the number of ravens decreased by one
 */
game_state game_state::add_raven() const &
{
	if(raven_count +1 > MAX_RAVEN_COUNT)
	{
		throw std::overflow_error("Amount of higher than max number of raven cards!");
	}
	return game_state(std::move(fruit_count), raven_count+1);
}

const game_state & game_state::add_raven() const &&
{
	if(raven_count +1 > MAX_RAVEN_COUNT)
	{
		throw std::overflow_error("Amount of higher than max number of raven cards!");
	}
	++turn_count;
	++raven_count;
	return *this;
}

//! get number of raven cards
unsigned int game_state::get_raven_count() const
{
	return raven_count;
}

//get number of fruit on given tree
unsigned int game_state::get_fruit_count_at(size_t tree_index) const
{
	return fruit_count.at(tree_index);
}

}//namespace
