#ifndef _GENERATOR_HPP_INCLUDED
#define _GENERATOR_HPP_INCLUDED

#include "orchard.hpp"
#include "strategies.hpp"
#include <atomic>
#include <optional>
#include <iterator>
#include <functional>


namespace orchard
{

class game_generator;

class game_iterator
{
public:
	/**
	 * create iterator with invalid position and
	 * invalid game state. This iterator
	 * is not dereferencable. This is to
	 * comply to DefaultConstructible requirement
	 * of LegacyInputIterator
	 */
	game_iterator() = default;

	game_iterator(const game_iterator & ) = default;
	game_iterator & operator=(const game_iterator &) = default;
	game_iterator(game_iterator &&) = default;
	game_iterator & operator=(game_iterator &&) = default;

	/**
	 * Compare equality
	 * @return true iff iterators have same position or if both
	 * have invalid position. The game state is discarded in this
	 * copmarison.
	 */
	bool operator==(const game_iterator &) const;
	bool operator!=(const game_iterator &) const;


	/**
	 * Returns a game_state that is played to
	 * finish from the initial state.
	 * @throws if the iterator is default
	 * constructed or if it is an end iterator
	 */
	game_state operator*() const;

	/**! prefix increment: ++it
	 * @throws if it the iterator has an
	 * invalid position (meaning it was
	 * default constructed) or is an end iterator
	 */
	game_iterator operator++();

	/**
	 * postfix increment it++
	 * @throws if it the iterator has an
	 * invalid position (meaning it was
	 * default constructed) or is an end iterator
	 */
	game_iterator operator++(int);

	/*
	 * Create an end iterator with no valid game state
	 * and invalid strategy at position pos.
	 */
	static game_iterator create_end_iterator(unsigned int pos);

	/**
	 * Create a begin iterator at position 0 (zero) with a given
	 * initial state.
	 */
	static game_iterator create_begin_iterator(strategy_t strategy, const game_state & initial);

private:


	/**
	 * Create game iterator with given intial state and
	 * position.
	 */
	game_iterator(strategy_t strategy, std::optional<game_state>  initial, unsigned int position);

	//! function pointer to picking strategy
	strategy_t strategy;

	//! optional because end iterators have no game state
	std::optional<game_state> initial_state = std::nullopt;
	std::optional<game_state> final_state = std::nullopt;


	//! position. Is optional because default constructed iterator has no valid pos
	std::optional<long> position = std::nullopt;


};

class game_generator
{
public:
	/**
	 * Create a game generator
	 * that generates a number of games
	 * @param cnt number of games to create
	 */
	game_generator(strategy_t strat, game_state initial, size_t cnt);

	game_iterator cbegin();
	
	game_iterator cend();

private:
	game_iterator begin_it;
	game_iterator end_it;

};




}//namespace orchard

namespace std
{
	template<>
	struct iterator_traits<::orchard::game_iterator>
	{
		using difference_type = long;
		using value_type = ::orchard::game_state;
		using reference = ::orchard::game_state; //my iterator does not return a true reference
		using pointer   = const ::orchard::game_state &;
		using iterator_category = std::input_iterator_tag;
	};

} //namespace std


#endif //_GENERATOR_HPP_INCLUDED
