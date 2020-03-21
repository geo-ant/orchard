#include "orchard.hpp"

#include <stdexcept>
#include <numeric>
#include <algorithm>

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

game_state::game_state() : game_state(create_filled_array<int, TREE_COUNT>(INITIAL_FRUIT_COUNT), INITIAL_RAVEN_COUNT, 0)
{}

game_state::game_state(const std::array<int, TREE_COUNT> fruits, int ravens, int turns)
: fruit_count(fruits)
,raven_count(ravens),
turn_count(turns)
{
	auto less_than_zero = [](const auto num){return num<0;};
	if(std::find_if(fruit_count.cbegin(),fruit_count.cend(),less_than_zero)!=fruit_count.cend())
	{
		throw std::underflow_error("Fruit count must not be less than zero!");
	}
	if(raven_count < 0)
	{
		throw std::underflow_error("Raven count must not be less than zero!");
	}
	if(turn_count < 0)
	{
		throw std::underflow_error("Turn count must not be less than zero!");
	}
	if(raven_count > MAX_RAVEN_COUNT)
	{
		throw std::overflow_error("Raven count above legal maximum!");
	}


}

game_state game_state::pick_fruit(size_t tree_index) const
{
	std::array<int, TREE_COUNT> new_fruit_count(fruit_count);
	if(new_fruit_count.at(tree_index)>= 1)
	{
		new_fruit_count[tree_index]-= 1;
	}
	else
	{
		throw std::underflow_error("Error: cannot pick fruit from empty tree!");
	}

	return game_state(new_fruit_count, raven_count, turn_count+1);
}

game_state game_state::add_raven() const
{
	if(raven_count +1 > MAX_RAVEN_COUNT)
	{
		throw std::overflow_error("Amount of higher than max number of raven cards!");
	}
	return game_state(fruit_count, raven_count+1, turn_count+1);
}

int game_state::get_total_fruit_count() const
{
	return std::accumulate(fruit_count.cbegin(), fruit_count.cend(), 0);
}

game_state game_state::apply_strategy(strategy_t strat) const
{
	auto new_fruit_count = strat(fruit_count);
	return game_state(new_fruit_count, raven_count, turn_count+1);
}


}//namespace
