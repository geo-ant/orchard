#include "catch2/catch.hpp"

#include "orchard.hpp"
#include "dice.hpp"
#include <algorithm>

/**
 * Test the game_state and related functions
 */

TEST_CASE("Creating deterministic dice results", "[dice_result]")
{
	using namespace orchard;

	SECTION("Dice results corresponding to tree indices deliver expected results")
	{
		for(int index = 0; index < TREE_COUNT; ++index)
		{
			dice_result dice(index);
			REQUIRE(dice.is_tree_index() == true);
			REQUIRE(dice.is_fruit_basket() == false);
			REQUIRE(dice.is_raven() == false);
			REQUIRE(dice.get_tree_index().value() == index);
		}
	}

	SECTION("Dice result corresponding to fuit basket")
	{
		dice_result dice(dice_result::FRUIT_BASKET);
		REQUIRE(dice.is_tree_index() == false);
		REQUIRE(dice.is_fruit_basket() == true);
		REQUIRE(dice.is_raven() == false);
	}

	SECTION("Dice result corresponding to raven")
	{
		dice_result dice(dice_result::RAVEN);
		REQUIRE(dice.is_tree_index() == false);
		REQUIRE(dice.is_fruit_basket() == false);
		REQUIRE(dice.is_raven() == true);
	}

	SECTION("Creating illegal dice results throws exception")
	{
		auto make_dice_result = [](int i){return dice_result(i);};
		REQUIRE_THROWS(make_dice_result(-1));
		REQUIRE_NOTHROW(make_dice_result(TREE_COUNT+1));
		REQUIRE_THROWS(make_dice_result(TREE_COUNT+2));
	}

}
