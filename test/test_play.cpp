#include "catch2/catch.hpp"

#include "orchard.hpp"
#include "dice.hpp"
#include "play.hpp"
#include <algorithm>

/**
 * Test the game_state and related functions
 */

/**
 * Helper function for a strategy that picks two
 * fruits from tree index zero. Will throw an
 * exception if the tree does not have 2 or more
 * fruits.
 * @return game state with two fruits less on tree
 * with index zero.
 */
orchard::tree_array_t pick_zeroth_tree(const orchard::tree_array_t & fruit_count)
{
	auto new_array = fruit_count;
	new_array.at(0) -= 2;
	return new_array;
}

TEST_CASE("Playing a single turn", "[dice_result]")
{
	using namespace orchard;
	game_state g;

	strategy_t strat(pick_zeroth_tree);

	REQUIRE(g.turn_count==0);
	REQUIRE(g.raven_count==0);
	REQUIRE(g.get_total_fruit_count()==TREE_COUNT*INITIAL_FRUIT_COUNT);

	SECTION("Exactly one raven is added when a raven die is thrown")
	{
		dice_result dice(dice_result::RAVEN);
		REQUIRE(dice.is_raven() == true);

		auto g2 = perform_single_turn(strat, g, dice);

		REQUIRE(g2.get_total_fruit_count() == g.get_total_fruit_count());
		REQUIRE(g2.turn_count == g.turn_count+1);
		REQUIRE(g2.raven_count == g.raven_count+1);
	}

	SECTION("Exactly one fruit at the correct position is added when a tree index die is thrown")
	{
		for(int idx = 0; idx < orchard::TREE_COUNT; ++idx)
		{
			dice_result dice(idx);
			REQUIRE(dice.is_tree_index() == true);
			auto g2 = perform_single_turn(strat, g, dice);

			REQUIRE(g2.get_total_fruit_count() == g.get_total_fruit_count()-1);
			REQUIRE(g2.turn_count == g.turn_count+1);
			REQUIRE(g2.raven_count == g.raven_count);
			REQUIRE(g2.fruit_count.at(idx) == g.fruit_count.at(idx)-1);
		}
	}

	SECTION("User strategy is correctly applied when fruit basket die is thrown")
	{
		dice_result dice(dice_result::FRUIT_BASKET);
		REQUIRE(dice.is_fruit_basket() == true);
		auto g2 = perform_single_turn(strat, g, dice);

		REQUIRE(g2.get_total_fruit_count() == g.get_total_fruit_count()-2);
		REQUIRE(g2.turn_count == g.turn_count+1);
		REQUIRE(g2.raven_count == g.raven_count);
		REQUIRE(g2.fruit_count.at(0) == g.fruit_count.at(0)-2);
	}

}
