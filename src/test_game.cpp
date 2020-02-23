#ifdef MAKE_TESTS
#include "catch2/catch.hpp"

#include "orchard.hpp"
#include <algorithm>

/**
 * Test the game_state and related functions
 */

TEST_CASE("Adding ravens and picking fruit", "[game_state]")
{
	using namespace orchard;
	game_state g;

	REQUIRE(g.turn_count==0);
	REQUIRE(g.raven_count==0);
	REQUIRE(g.get_total_fruit_count()==TREE_COUNT*INITIAL_FRUIT_COUNT);

	SECTION("adding a raven increases raven count and turn count")
	{
		auto g2 = g.add_raven();
		REQUIRE(g2.raven_count==1);
		REQUIRE(g2.turn_count == g.turn_count+1);
	}

	SECTION("picking fruit removes one piece of fruit from the correct index and increases turn count")
	{
		for(int index = 0 ; index < TREE_COUNT; ++index)
		{
			auto g2 = g.pick_fruit(index);
			REQUIRE(g2.fruit_count.at(index) == g.fruit_count.at(index)-1);
			REQUIRE(g2.get_total_fruit_count() == g.get_total_fruit_count()-1);
			REQUIRE(g2.turn_count == g.turn_count+1);
		}
	}

}

TEST_CASE("Adding ravens and picking fruit: exceptions","[game_state]")
{
	using namespace orchard;
	game_state g (std::array<int, TREE_COUNT>{},MAX_RAVEN_COUNT,0);

	REQUIRE(g.turn_count==0);
	REQUIRE(g.raven_count==MAX_RAVEN_COUNT);
	REQUIRE(g.get_total_fruit_count()==0);

	SECTION("picking fruit on empty tree throws an exception")
	{
		REQUIRE_THROWS(g.pick_fruit(0));
	}

	SECTION("adding more than max number of ravens throws exception")
	{
		REQUIRE_THROWS(g.add_raven());
	}

}

TEST_CASE("Construction of illegal game states throws exceptions", "[game_state]")
{
	using namespace orchard;
	std::array<int, TREE_COUNT> legal_fruit_count{}; //will be all zero

	std::array<int, TREE_COUNT> illegal_fruit_count{};
	illegal_fruit_count.at(0)=-1;

	REQUIRE_NOTHROW(game_state(legal_fruit_count,0,0));

	SECTION("constructing array with negative fruit count throws exception")
	{
		REQUIRE_THROWS(game_state(illegal_fruit_count,0,0));
	}

	SECTION("constructing array with illegal raven count throws exception")
	{
		REQUIRE_THROWS(game_state(illegal_fruit_count,-1,0));

		REQUIRE_NOTHROW(game_state(legal_fruit_count,MAX_RAVEN_COUNT,0));
		REQUIRE_THROWS(game_state(illegal_fruit_count,MAX_RAVEN_COUNT+1,0));
	}

	SECTION("constructing array with negative turn count throws exception")
	{
		REQUIRE_THROWS(game_state(legal_fruit_count,0,-1));
	}
}

TEST_CASE("Classification of games as won / lost / over", "[game_state]")
{
	using namespace orchard;
	std::array<int, TREE_COUNT> zero_fruit_count{}; //will be all zero

	std::array<int, TREE_COUNT> nonzero_fruit_count;
	std::fill(nonzero_fruit_count.begin(), nonzero_fruit_count.end(),1);

	SECTION("won game is correctly classified as won and over")
	{
		game_state g(zero_fruit_count,1,0);
		REQUIRE(is_won(g)==true);
		REQUIRE(is_lost(g)==false);
		REQUIRE(is_over(g)==true);
	}

	SECTION("lost game is correctly classified as lost and over")
	{
			game_state g(nonzero_fruit_count,MAX_RAVEN_COUNT,0);
			REQUIRE(is_won(g)==false);
			REQUIRE(is_lost(g)==true);
			REQUIRE(is_over(g)==true);
	}

	SECTION("intermediary game is not won, lost, or over")
	{
			game_state g(nonzero_fruit_count,MAX_RAVEN_COUNT-1,100);
			REQUIRE(is_won(g)==false);
			REQUIRE(is_lost(g)==false);
			REQUIRE(is_over(g)==false);
	}

}


#endif
