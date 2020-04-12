#include "catch2/catch.hpp"
#include <algorithm>

#include "orchard.hpp"
#include "statistics.hpp"
#include "strategies.hpp"
#include "analysis.hpp"

using namespace orchard;

TEST_CASE("Win Loss Statistic", "[win_loss_stat]")
{
	//construct a won and a lost game
	tree_array_t fruit_count_zero;
	std::fill(fruit_count_zero.begin(), fruit_count_zero.end(),0);
	game_state g_won(fruit_count_zero,1,1337);
	REQUIRE(is_won(g_won)==true);
	REQUIRE(is_lost(g_won)==false);

	tree_array_t fruit_count;
	std::fill(fruit_count.begin(), fruit_count.end(),INITIAL_FRUIT_COUNT);
	game_state g_lost(fruit_count,MAX_RAVEN_COUNT,1234);
	REQUIRE(is_lost(g_lost) == true);
	REQUIRE(is_won(g_lost) == false);


	SECTION("Default constructed w/l statistic has all zero members")
	{
		win_loss_statistic wlstat;
		REQUIRE(wlstat.get_wins() == 0);
		REQUIRE(wlstat.get_losses() == 0);
		REQUIRE(wlstat.get_cumulated_turn_count() == 0);
		REQUIRE(wlstat.get_game_count() == 0);
	}

	SECTION("Constructing statistic from game has correct turn and game count")
	{
		game_state g = game_state{}.pick_fruit(0).add_raven().add_raven();

		win_loss_statistic wlstat(g);

		REQUIRE(wlstat.get_cumulated_turn_count() == g.turn_count);
		REQUIRE(wlstat.get_game_count() == 1);
	}

	SECTION("Constructing statistic from won game")
	{
		win_loss_statistic wlstat(g_won);
		REQUIRE(wlstat.get_losses() == 0);
		REQUIRE(wlstat.get_wins() == 1);
		REQUIRE(wlstat.get_cumulated_turn_count() == g_won.turn_count);
		REQUIRE(wlstat.get_game_count() == 1);
	}

	SECTION("Constructing statistic from lost game")
	{
		win_loss_statistic wlstat(g_lost);

		REQUIRE(wlstat.get_losses() == 1);
		REQUIRE(wlstat.get_wins() == 0);
		REQUIRE(wlstat.get_cumulated_turn_count() == g_lost.turn_count);
		REQUIRE(wlstat.get_game_count() == 1);
	}

	SECTION("Addition of two statistics")
	{
		auto wlstat = win_loss_statistic(g_won) + win_loss_statistic(g_lost);
		REQUIRE(wlstat.get_losses() == 1);
		REQUIRE(wlstat.get_wins() == 1);
		REQUIRE(wlstat.get_cumulated_turn_count() == g_lost.turn_count + g_won.turn_count);
		REQUIRE(wlstat.get_game_count() == 2);
	}

	SECTION("Accumulation of w/l statistic")
	{
		size_t count = 1000;
		auto wlstat = accumulate_statistics(std::execution::seq,&pick_fruit_in_order,count,win_loss_statistic());
		REQUIRE(wlstat.get_game_count() == count);
		REQUIRE(wlstat.get_wins() + wlstat.get_losses() == wlstat.get_game_count());
	}
}
