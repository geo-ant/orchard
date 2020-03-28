#include "orchard.hpp"
#include <iostream>
#include "dice.hpp"
#include <stdexcept>
#include "play.hpp"

namespace orchard
{


	game_state perform_single_turn(strategy_t strategy, const game_state & game,  dice_result dice)
	{
		if(is_over(game))
		{
			return game;
		}

		if(dice.is_tree_index())
		{
			int tree_index = dice.get_tree_index().value();
			return (game.fruit_count.at(tree_index)>0) ? game.pick_fruit(tree_index) : game;
		}
		else if(dice.is_raven())
		{
			return game.add_raven();
		}
		else if(dice.is_fruit_basket())
		{
			return game.apply_strategy(strategy);
		}
		else
		{
			throw std::logic_error("Impossible dice result occurred!");
		}
	}

	game_state play_to_finish(strategy_t strategy, const game_state & game)
	{
		if(is_over(game))
		{
			return game;
		}

		const auto dice = dice_result::create_random();
		game_state next = perform_single_turn(strategy, game, dice);

#ifdef DEBUG
		std::cout << "(" << game << ")";
		std::cout << " => [" << dice.to_string() << "] => ";
		std::cout << "(" << next << ")" << std::endl;
#endif

		return play_to_finish(strategy, next);

	}

}
