#include "orchard.hpp"
#include <iostream>
#include <chrono>
#include <random>


namespace orchard
{

	constexpr size_t BASKET = TREE_COUNT;
	constexpr size_t RAVEN  = TREE_COUNT+1;

	bool is_fruit(size_t dice_result)
	{
		return dice_result < TREE_COUNT;
	}

	bool is_basket(size_t dice_result)
	{
		return dice_result == BASKET;
	}

	bool is_raven(size_t dice_result)
	{
		return dice_result == RAVEN;
	}



	/*
	 * Roll the game die. The numbers 0,..., TREE_COUNT-1 represent rolled fruits.
	 * Those can be directly used as an index.
	 * the next number is the basket (BASKET), then comes the raven (RAVEN).
	 * see: https://stackoverflow.com/questions/21237905/how-do-i-generate-thread-safe-uniform-random-numbers?answertab=active#tab-top
	 * @return uniform random number from 0-RAVEN
	 */
	size_t roll_die()
	{
	    static thread_local std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
	    std::uniform_int_distribution<size_t> distribution(0,RAVEN);
	    return distribution(generator);
	}

	/**
	 * performs a game turn depending on dice result and user strategy
	 *
	 */
	inline game_state perform_single_turn(game_state && game,  const picking_strategy & strategy, size_t dice_result)
	{

		if(is_fruit(dice_result))
		{
			return game.pick_fruit(dice_result);
		}
		else if(is_raven(dice_result))
		{
			return game.add_raven();
		}
		else
		{
			return strategy.pick_fruits(std::move(game));
		}
	}

	game_state play_to_finish(game_state && game, const picking_strategy & strategy)
	{
#ifdef DEBUG
		std::cout << game << std::endl;
#endif


		if(is_over(game))
		{
			return game;
		}

		const auto dice_result = roll_die();
		game_state next = perform_single_turn(std::move(game), strategy, dice_result);

#ifdef DEBUG
		std::cout << "dice = " << dice_result << std::endl;
#endif

		return play_to_finish(std::move(next), strategy);

	}

}
