#include <algorithm>
#include "catch2/catch.hpp"
#include "orchard.hpp"
#include "strategies.hpp"
#include <vector>

using namespace orchard;

TEST_CASE("Test general statements about all strategies", "[strat]")
{
	std::vector<strategy_t> strategies;
	strategies.push_back(&pick_fruit_in_order);
	strategies.push_back(&pick_fruit_at_random);
	strategies.push_back(&pick_fruit_from_fullest_tree);
	strategies.push_back(&pick_fruit_from_emptiest_tree);

	SECTION("Picks two fruits if enough are there")
	{
		game_state g;
		for(const auto & strat : strategies)
		{
			auto g2 = g.apply_strategy(strat);
			REQUIRE(g2.get_total_fruit_count() == g.get_total_fruit_count()-2);
		}
	}

	SECTION("Picks only one fruit if only one is left")
	{
		tree_array_t fruit_count;
		std::fill(fruit_count.begin(), fruit_count.end(),0);
		fruit_count.at(TREE_COUNT/2) = 1;
		game_state g(fruit_count, 0,0);
		REQUIRE(g.get_total_fruit_count() == 1);

		for(const auto & strat : strategies)
		{
			auto g2 = g.apply_strategy(strat);
			REQUIRE(g2.get_total_fruit_count() == g.get_total_fruit_count()-1);
		}
	}
}


TEST_CASE("Test the in order picking strategy", "[in_order_strat]")
{
	SECTION("Picks two for full tree index")
	{
		tree_array_t fruit_count;
		for(auto iter = fruit_count.begin(); iter != fruit_count.end(); ++iter)
		{
			//fill with 0 up to iter
			std::fill(fruit_count.begin(), iter,0);
			//from iter on fill with 2
			std::fill(iter,fruit_count.end(),2);

			game_state g(fruit_count, 0,0);

			auto g2 = g.apply_strategy(pick_fruit_in_order);
			int index = std::distance(fruit_count.begin(), iter);
			REQUIRE(g2.fruit_count.at(index) == g.fruit_count.at(index)-2);
			REQUIRE(g2.get_total_fruit_count() == g.get_total_fruit_count()-2);
		}
	}

	SECTION("Picks only one fruit if only one is left")
	{
		tree_array_t fruit_count;
		for(auto iter = fruit_count.begin(); iter != fruit_count.end(); ++iter)
		{
			//fill with only 1 at iter, others 0
			std::fill(fruit_count.begin(), fruit_count.end(),0);
			*iter = 1;

			game_state g(fruit_count, 0,0);

			REQUIRE(g.get_total_fruit_count() == 1);

			REQUIRE_NOTHROW(g.apply_strategy(pick_fruit_in_order));

			auto g2 = g.apply_strategy(pick_fruit_in_order);
			REQUIRE(g2.get_total_fruit_count() == 0);
		}
	}
	
	SECTION("Picks two different fruits if only one fruit on first tree")
	{
		tree_array_t fruit_count;
		std::fill(fruit_count.begin(), fruit_count.end(),0);
		fruit_count.at(0) = 1;
		fruit_count.at(1) = 1;

		game_state g(fruit_count, 0,0);
		REQUIRE(g.get_total_fruit_count() == 2);

		REQUIRE_NOTHROW(g.apply_strategy(pick_fruit_in_order));

		auto g2 = g.apply_strategy(pick_fruit_in_order);
		REQUIRE(g2.get_total_fruit_count() == 0);
	}

}

TEST_CASE("Test the random picking strategy","[random_strat]")
{
	/**
	 * Tests involving randomness are very crude
	 * basically just to show that not all results
	 * are the same
	 */
	//other test cases are done by general case above
	//just test that not always the same fruits are picked
	game_state g;
	tree_array_t first_pick = g.apply_strategy(pick_fruit_at_random).fruit_count;

	bool picks_are_different = false;

	for(int n = 0; n < 1000; ++n)
	{
		tree_array_t nth_pick =g.apply_strategy(pick_fruit_at_random).fruit_count;
		if(first_pick != nth_pick)
		{
			picks_are_different = true;
			break;
		}
	}
	REQUIRE(picks_are_different == true);
}

TEST_CASE("Test the emptiest tree strategy","[emptiest_strat]")
{

	SECTION("Stragety correctly finds empties tree")
	{
		game_state g;

		for(size_t index = 0; index < TREE_COUNT; ++index)
		{
			auto g2 = g.pick_fruit(index);
			auto g3 = g2.apply_strategy(pick_fruit_from_emptiest_tree);

			REQUIRE(g3.get_total_fruit_count() == g2.get_total_fruit_count()-2);
			REQUIRE(g3.fruit_count.at(index) == g2.fruit_count.at(index)-2);
		}
	}
}

TEST_CASE("Test the fullest tree strategy","[fullest_strat]")
{
	SECTION("Maximum tree index correctly identified in consecutive rounds")
	{
		//here we test that for all identical trees the maximum of
		//two consecutive rounds is chosen
		game_state g;
		auto g2 = g.apply_strategy(pick_fruit_from_fullest_tree);
		auto min_it = std::min_element(g2.fruit_count.cbegin(), g2.fruit_count.cend());
		REQUIRE(*min_it == INITIAL_FRUIT_COUNT-1);
		REQUIRE(g2.get_total_fruit_count() == g.get_total_fruit_count()-2);
	}

	SECTION("Maximum tree is correctly identified")
	{
		tree_array_t fruit_count;
		std::fill(fruit_count.begin(),fruit_count.end(),INITIAL_FRUIT_COUNT-2);
		fruit_count.at(0) = INITIAL_FRUIT_COUNT;
		game_state g(fruit_count,0,0);
		auto g2 = g.apply_strategy(pick_fruit_from_fullest_tree);
		REQUIRE(g2.fruit_count.at(0) == g.fruit_count.at(0)-2);
	}
}

