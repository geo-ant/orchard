#ifndef _ORCHARD_HPP_INCLUDED
#define _ORCHARD_HPP_INCLUDED

#include <array>
#include <functional>

//forward declaration
class ostream;

namespace orchard
{

//! number of trees with fruits
constexpr unsigned int TREE_COUNT = 4;
//! number of raven cards at which the game_state is lost
constexpr unsigned int MAX_RAVEN_COUNT = 9;
//! number of initial fruits
constexpr unsigned int INITIAL_FRUIT_COUNT = 10;
//! number of initial ravens
constexpr unsigned int INITIAL_RAVEN_COUNT = 0;



class game_state
{
public:


	/**
	 * construct game_state with INITIAL_FRUIT_COUNT fruits on each
	 * tree and zero ravens. This is the start configuration
	 */
	game_state();

	/** 
	 * construct new game_state with given amount of fruits and ravens
	 * */
	game_state(std::array<unsigned int, TREE_COUNT> && fruits, unsigned int ravens);
	game_state(const std::array<unsigned int, TREE_COUNT> & fruits, unsigned int ravens);

	/*
	 * Pick a number of fruits from a tree. The amount of fruit never drops
	 * below 0 bu nothing happens if the request is made on a tree with 0 fruit.
	 * @returns new game_state state with the correct amount of fruits after picking
	 */
	game_state pick_fruit(size_t tree_index, unsigned int amount=1) const &;
	game_state pick_fruit(size_t tree_index, unsigned int amount=1) const &&;
	
	/*
	 * Add a raven card to the game_state
	 * @returns new game_state state with the number of ravens decreased by one
	 */
	game_state add_raven() const &;
	game_state add_raven() const &&;
	
	//! get number of raven cards
	unsigned int get_raven_count() const;

	//! get number of fruit on given tree
	unsigned int get_fruit_count_at(size_t tree_index) const;

	//! constant iterators for the fruit count
	auto fruit_cbegin() const
	{
		return fruit_count.cbegin();
	}
	auto fruit_cend() const
	{
		return fruit_count.cend();
	}

private:
	//! the number of fruits on the trees
	mutable std::array<unsigned int, TREE_COUNT> fruit_count;

	//! number of raven cards on board
	unsigned int raven_count;
};

using strategy_t = std::function<game_state(game_state&&)>;


/**
 * abstract class that represents a picking strategy
 * that comes into play if (and only if) the basket
 * was rolled with the die.
 */
class picking_strategy
{
public:
	virtual ~picking_strategy() {};
	/**
	 * This method of a picking strategy should
	 * pick the fruits according to the strategy
	 * and return the new game state.
	 */
	virtual game_state pick_fruits(game_state &&) const = 0;
};

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
game_state play_to_finish(strategy_t strategy, game_state &&);

}

//for printing the game state to console
std::ostream & operator<<(std::ostream & os, const orchard::game_state & g);

#endif // _ORCHARD_HPP_INCLUDED
