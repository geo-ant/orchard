#include <algorithm>
#include "catch2/catch.hpp"
#include "orchard.hpp"
#include "strategies.hpp"

using namespace orchard;


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
