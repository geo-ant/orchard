#include "catch2/catch.hpp"

#include "orchard.hpp"
#include "dice.hpp"
#include <algorithm>
#include <type_traits>

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

/**
 * helper struct for next test case
 */
struct dice_counter
{

	void operator()(const dice_result<dice_t::TREE_INDEX> &)
	{
		++tree_index_counts;
	}

	void operator()(const dice_result<dice_t::FRUIT_BASKET> &)
	{
		++other_counts;
	}

	void operator()(const dice_result<dice_t::RAVEN> &)
	{
		++other_counts;
	}


	float tree_index_counts = 0;
	float other_counts = 0;
};

TEST_CASE("Creating random dice throws")
{
	/**
	 * This is a VERY CRUDE test of randomness:
	 * we only remember two outcomes:
	 * 1) dice throw is TREE_INDEX
	 * 2) dice throw is not TREE_INDEX
	 * then we compare that the ratio of these
	 * two numbers is approximately TREE_COUNT/2
	 * and we just assume the rest of the rng
	 * works fine by inference :)
	 */

	dice_counter counter;

	for(int j = 0; j < 1000000; ++j)
	{
		auto dice = create_random_dice_result();
		std::visit(counter,dice);
	}

	// it should be approximately counter.tree_index_counts/counter.other_counts = (TREE_COUNT)/2
	REQUIRE(counter.tree_index_counts/counter.other_counts > 0.8f*0.5f*(TREE_COUNT));
	REQUIRE(counter.tree_index_counts/counter.other_counts < 1.2f*0.5f*(TREE_COUNT));

}
