#include "orchard.hpp"
#include <iostream>
#include "dice.hpp"
#include <stdexcept>
#include "play.hpp"

namespace orchard
{

	game_state perform_single_turn(strategy_t strategy, const game_state & game,  dice_result_variant dice)
	{
		return std::visit(single_turn(strategy,game),dice);
	}

	game_state play_to_finish(strategy_t strategy, const game_state & game)
	{
		if(is_over(game))
		{
			return game;
		}

		const auto dice = create_random_dice_result();
		game_state next = perform_single_turn(strategy, game, dice);

	#ifdef DEBUG
		std::cout << "(" << game << ")";
		std::cout << " => [" << to_string(dice) << "] => ";
		std::cout << "(" << next << ")" << std::endl;
	#endif

		return play_to_finish(strategy, next);
	}

	/****
	 * implementation of the visitor
	 */
	single_turn::single_turn(strategy_t strat, game_state g):game(g), strategy(strat)
	{}

	game_state single_turn::operator()(dice_result<dice_t::FRUIT_BASKET>)
	{
		return game.apply_strategy(strategy);
	}

	game_state single_turn::operator()(dice_result<dice_t::RAVEN>)
	{
		return game.add_raven();
	}

	game_state single_turn::operator()(dice_result<dice_t::TREE_INDEX> dice)
	{
		return (game.fruit_count.at(dice.tree_index)>0) ? game.pick_fruit(dice.tree_index) : game;
	}


}
