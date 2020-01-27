#ifndef _ANALYSIS_HPP_INCLUDED
#define _ANALYSIS_HPP_INCLUDED

#include "orchard.hpp"
#include <atomic>


namespace orchard
{

class game_generator;

class game_iterator
{

	/**
	 * create iterator with position 0 and
	 * default constructed intial game state
	 */
	game_iterator();

	/**
	 * Returns a game_state that is played to
	 * finish from the initial state. Each invokation
	 * of the operator* will probably return a random
	 * game_state that is played to finish
	 */
	game_state operator*() const;

	//! prefix increment: ++it
	game_iterator& operator++();

	//! postfix increment: it++
	game_iterator operator++(int);



private:
	/**
	 * Initialize game iterator with initial state and position 0
	 */
	game_iterator(const game_state initial_state, size_t pos = 0);

	long position;
	game_state state;
	std::atomic<bool> was_played_to_finish;

	friend class game_generator;
};

class game_generator
{
	/**
	 * Create a game generator
	 * that generates a number of games
	 * @param cnt number of games to create
	 */
	game_generator(size_t cnt);




private:
	const size_t count;

};

}

#endif //_ANALYSIS_HPP_INCLUDED
