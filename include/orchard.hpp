#ifndef _ORCHARD_HPP_INCLUDED
#define _ORCHARD_HPP_INCLUDED

#include <array>
#include <functional>

//forward declaration
class ostream;

namespace orchard
{

//! number of trees with fruits
constexpr int TREE_COUNT = 4;
//! number of raven cards at which the game_state is lost
constexpr int MAX_RAVEN_COUNT = 9;
//! number of initial fruits
constexpr int INITIAL_FRUIT_COUNT = 10;
//! number of initial ravens
constexpr int INITIAL_RAVEN_COUNT = 0;



class game_state
{
public:

	/**
	 * construct game_state with INITIAL_FRUIT_COUNT fruits on each
	 * tree and zero ravens. This is the start configuration
	 */
	game_state();

	/** 
	 * construct new game_state with given amount of fruits, ravens and turns
	 * @throws underflow_error or overflow_error if values are outside legal ranges
	 * */
	game_state(const std::array<int, TREE_COUNT> & fruits, int ravens, int turns);

	/*
	 * Pick one piece of fruit from a given tree.
	 * @throws std::underflow_error if fruit count would drop below 0
	 * @returns new game_state state with the correct amount of fruits after picking
	 */
	game_state pick_fruit(size_t tree_index) const;
	
	/*
	 * Add a raven card to the game_state
	 * @throws std::overflow error if raven count increases beyond MAX_RAVEN_COUNT
	 * @returns new game_state state with the number of ravens decreased by one
	 */
	game_state add_raven() const;
	
	//! get total number of fruit summed over all trees
	int get_total_fruit_count() const;

	//! the number of fruits on the trees
	const std::array<int, TREE_COUNT> fruit_count;

	//! number of raven cards on board
	const int raven_count;
	
	// number of turns in the game
	const int turn_count;
};

using strategy_t = std::function<game_state(const game_state&)>;

/**
 * Free functions
 */

//! return true iff the game_state is over (won or lost)
bool is_over(const game_state &);
//! return true iff the game_state was won by the players
bool is_won(const game_state &);
//! return true iff the game_state was won by the raven
bool is_lost(const game_state &);



/**
 * Plays the game to finish and employs the given
 * picking strategy if and only if the die roll indicates it.
 * Otherwise plays according to the rules
 * @returns the game state on win or lose
 */
game_state play_to_finish(strategy_t strategy, const game_state &);

} //namespace orchard

//for printing the game state to console
std::ostream & operator<<(std::ostream & os, const orchard::game_state & g);

#endif // _ORCHARD_HPP_INCLUDED
