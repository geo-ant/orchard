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
game_state perform_single_turn(strategy_t strategy, const game_state & game,  dice_result dice);
}


#endif // _PLAY_HPP_INCLUDED
