#ifdef MAKE_TESTS
#include "catch2/catch.hpp"

#include "orchard.hpp"

/**
 * Test the game_state and related functions
 */

TEST_CASE("Modification of game_state", "[game_state]")
{
	using namespace orchard;
	game_state g;

	REQUIRE(g.turn_count==0);
	REQUIRE(g.raven_count==0);
	REQUIRE(g.get_total_fruit_count()==TREE_COUNT*INITIAL_FRUIT_COUNT);

	SECTION("adding a raven increases raven count")
	{
		auto g2 = g.add_raven();
		REQUIRE(g2.raven_count==1);
	}

	//SECTION("")


}

#endif
