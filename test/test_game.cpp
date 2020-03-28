#include "catch2/catch.hpp"

#include "orchard.hpp"
#include <algorithm>

/**
 * Test the game_state and related functions
 */
using namespace orchard;


TEST_CASE("Adding ravens and picking fruit", "[game_state]")
{
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

TEST_CASE("Constructors of game_state","[game_state]")
{
	SECTION("Default constructing a game state produces standard game")
	{
		tree_array_t expected_fruit_count;
		std::fill(expected_fruit_count.begin(), expected_fruit_count.end(), INITIAL_FRUIT_COUNT);

		auto expected_raven_count = INITIAL_RAVEN_COUNT;
		int expected_turn_count = 0;

		game_state g;
		REQUIRE(g.turn_count == expected_turn_count);
		REQUIRE(g.raven_count == expected_raven_count);
		REQUIRE(g.fruit_count == expected_fruit_count);
	}

	SECTION("Constructor taking all member fields produces correct state")
	{
		tree_array_t fruit_count;
		std::fill(fruit_count.begin(), fruit_count.end(), 1);

		auto raven_count = 0;
		int turn_count = 1234;

		game_state g(fruit_count, raven_count, turn_count);

		REQUIRE(g.turn_count == turn_count);
		REQUIRE(g.raven_count ==raven_count);
		REQUIRE(g.fruit_count == fruit_count);

	}
}

TEST_CASE("Testing games for equality == and !=","[game_state]")
{
	tree_array_t fruit_A;
	std::fill(fruit_A.begin(),fruit_A.end(),1);

	tree_array_t fruit_B;
	std::fill(fruit_B.begin(),fruit_B.end(),2);

	int ravens_A = 1;
	int ravens_B = 2;

	int turns_A = 1337;
	int turns_B = 123;

	REQUIRE(fruit_A != fruit_B);
	REQUIRE(turns_A != turns_B);
	REQUIRE(ravens_A != ravens_B);

	SECTION("Equal states compare equal")
	{
		game_state g1 (fruit_A, ravens_A, turns_A);
		game_state g2 (fruit_A, ravens_A, turns_A);
		REQUIRE(g1 == g2);
		REQUIRE((g1!=g2)==false);
	}

	SECTION("Different fruit lead to unequality")
	{
		game_state g1 (fruit_A, ravens_A, turns_A);
		game_state g2 (fruit_B, ravens_A, turns_A);
		REQUIRE(g1 != g2);
		REQUIRE((g1==g2)==false);
	}

	SECTION("Different ravens lead to unequality")
	{
		game_state g1 (fruit_A, ravens_A, turns_A);
		game_state g2 (fruit_A, ravens_B, turns_A);
		REQUIRE(g1 != g2);
		REQUIRE((g1==g2)==false);
	}

	SECTION("Different turns lead to unequality")
	{
		game_state g1 (fruit_A, ravens_A, turns_A);
		game_state g2 (fruit_A, ravens_A, turns_B);
		REQUIRE(g1 != g2);
		REQUIRE((g1==g2)==false);
	}

}
