#ifndef _PLAY_HPP_INCLUDED
#define _PLAY_HPP_INCLUDED

#include "orchard.hpp"
#include "dice.hpp"

namespace orchard
{
/**
 * Performs a single turn of the game with the given strategy and dice result
 * @param strategy the chosen strategy
 * @param game current state of the game which the turn is based on
 * @param dice result of the dice
 * @returns a game state representing the next state or the same game state if the
 * game is over. If dice result is a tree index, this tree gets picked (if it has fruit left)
 * if the dice result is a raven, a raven card is added. If the dice result is a
 */
//game_state perform_single_turn(strategy_t strategy, const game_state & game,  dice_result dice);
game_state perform_single_turn(strategy_t strategy, const game_state & game,  dice_result_variant dice);

/**
 * A visitor that overloads operator() for
 * the possible dice results to be applied
 * to the returned variant. Used together
 * with std::visit to apply the correct
 * turn to the game given the strategy and
 * dice result.
 */
class single_turn
{
public:
	/**
	 * Create turn with given game state and
	 * strategy
	 */
	single_turn(strategy_t strat,  game_state game);

	single_turn(single_turn &&) = delete;
	single_turn & operator=(single_turn &&) = delete;

	/**
	 * Return game state after the dice result was applied
	 */
	game_state operator()(dice_result<dice_t::FRUIT_BASKET>);
	game_state operator()(dice_result<dice_t::RAVEN>);
	game_state operator()(dice_result<dice_t::TREE_INDEX> dice);

private:
	game_state game;
	strategy_t strategy;
};

}


#endif // _PLAY_HPP_INCLUDED
