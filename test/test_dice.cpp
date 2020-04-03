#include "catch2/catch.hpp"

#include "orchard.hpp"
#include "dice.hpp"
#include <algorithm>

using namespace orchard;


/**
 * Test the game_state and related functions
 */

TEST_CASE("Creating deterministic dice results", "[dice_result]")
{

	SECTION("Constructor for tree index dice result assigns correct index")
	{
		for(int index = 0; index < TREE_COUNT; ++index)
		{
			dice_result<dice_t::TREE_INDEX> dice(index);
			REQUIRE(dice.tree_index == index);
		}
	}

	SECTION("Creating illegal dice results for tree index throws exception")
	{
		auto make_dice_result = [](int i){return dice_result<dice_t::TREE_INDEX>(i);};
		REQUIRE_THROWS(make_dice_result(-1));
		REQUIRE_THROWS(make_dice_result(TREE_COUNT));
		REQUIRE_THROWS(make_dice_result(TREE_COUNT+1));
		REQUIRE_THROWS(make_dice_result(TREE_COUNT+2));
	}

}

//TEST_CASE("Creating random dice throws")
//{
//	/**
//	 * This is a very crude test of randomness
//	 * in so far that we generate a histogram
//	 * but only perform the most basic tests
//	 *(such that every possible dice result shoud
//	 * at least exist once).
//	 */
//	//constexpr int LARGE_ISH_NUMBER = std::max(dice_result::RAVEN, dice_result::FRUIT_BASKET)*500;
//	//std::map<int, int> histogram;
//	//@todo: weitermachen
//
//
//}
