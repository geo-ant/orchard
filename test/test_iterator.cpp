#include "catch2/catch.hpp"
#include "orchard.hpp"
#include "strategies.hpp"
#include "generator.hpp"

#include <algorithm>
#include <iterator>

using namespace orchard;



TEST_CASE("Creating an end iterator","[end_iterator]")
{
	auto end_it = game_iterator::create_end_iterator(123);
	
	SECTION("End iterator cannot be incremented (prefix)")
	{
		REQUIRE_THROWS(++end_it);
	}
	
	SECTION("End iterator cannot be incremented (postfix)")
	{
		REQUIRE_THROWS(end_it++);
	}
	
}

TEST_CASE("Dereferencing iterators", "[game_iterator]")
{
	constexpr int LARGE_ISH_NUMBER = 1000;
	auto begin_it = game_iterator::create_begin_iterator(pick_fruit_in_order, game_state());
	auto end_it = game_iterator::create_end_iterator(LARGE_ISH_NUMBER);

	
	SECTION("Dereferencing the same iterator multiple times produces the same result")
	{
		/** 
		 * this is a statistical test, but it is highly unlikely
		 * that the test will past juss because of coincidence.
		 * So if this passes we can be reasonably sure that the
		 * condition indeed holds
		 */
		for(int n = 0; n < 10; ++n)
		{
			auto it = game_iterator::create_begin_iterator(pick_fruit_in_order, game_state());
			const game_state first = *it;
			for(int m = 0; m < 100; ++m)
			{
				REQUIRE(first == *it);
			}
		}
	}
	
	SECTION("Dereferencing a default constructed iterator throws")
	{
		game_iterator it;
		REQUIRE_THROWS(*it);
	}
	
	SECTION("Dereferencing an end iterator throws")
	{
		REQUIRE_THROWS(*end_it);
	}
	
	SECTION("Dereferencing all iterators in a series except end iterator does not throw")
	{
		for(auto it = begin_it; it != end_it; ++it)
		{
			REQUIRE_NOTHROW(*it);
		}
	}
	
	SECTION("Game iterators within a range produce random results")
	{
		/**
		 * This is also a statistical test, but since the 
		 * number of tested iterators is relatively large
		 * we can be reasonably sure that the tests truly
		 * "proves" what it should. We test that not all
		 * games compare equal to the initial state, which
		 * is somewhat sloppy but good enough for our purposes
		 */
		auto first = *begin_it;
		bool games_are_random = false;
		for(auto it = begin_it; it!= end_it; ++it)
		{
			if(first != *it)
			{
				games_are_random = true;
				break;
			}
		}
		REQUIRE(games_are_random == true);
	}
	
	SECTION("game_iterator applies the correct strategy")
	{
		bool strat_was_applied = false;
		auto my_strat = [&strat_was_applied](const tree_array_t & arr) -> tree_array_t
						{
							strat_was_applied = true;
							return pick_fruit_in_order(arr);
						};
		auto it =  game_iterator::create_begin_iterator(my_strat, game_state());
		REQUIRE(strat_was_applied == true);
	}
	
	SECTION("game_iterator uses the correct initial game")
	{
		//we just pass a game which is already won into
		//the iterator and expect the same game_state to
		//be returned. So we can be reasonably sure that
		//it has used the exact game state
		
		tree_array_t fruit_count;
		std::fill(fruit_count.begin(), fruit_count.end(),0);
		int ravens = 2;
		int turns  = 1337;
		game_state initial(fruit_count, ravens, turns);
		game_iterator it = game_iterator::create_begin_iterator(pick_fruit_in_order, initial);
		REQUIRE(*it == initial);
	}
	

}



TEST_CASE("Game generator produces correct sequences","[game_generator]")
{
	constexpr int LARGE_ISH_NUMBER = 1000;
	game_generator gen(pick_fruit_in_order, game_state(), LARGE_ISH_NUMBER);
	
	SECTION("Ranges are of specified length")
	{
		auto count = std::distance(gen.cbegin(), gen.cend());
		REQUIRE(count == LARGE_ISH_NUMBER);
	}
	
	SECTION("Produces a random collection of games that are always over")
	{
		//as in the case of iterators. Just test
		//that not all games are the same
		game_state first = *(gen.cbegin());
		bool games_are_random = false;
		for(auto it = gen.cbegin(); it!= gen.cend(); ++it)
		{
			if(first != *it)
			{
				games_are_random = true;
			}
			REQUIRE(is_over(*it) == true);
		}
		REQUIRE(games_are_random == true);
	}
}

TEST_CASE("Producing ranges with iterators","[game_iterator]")
{
	constexpr int SIZE = 10;
	auto begin_it = game_iterator::create_begin_iterator(pick_fruit_in_order, game_state());
	auto end_it = game_iterator::create_end_iterator(SIZE);
		
	SECTION("Prefix and postfix increment sets iterator to same position")
	{
		auto copy_one = begin_it;
		auto copy_two = begin_it;
		
		copy_one++;
		++copy_two;
		REQUIRE(copy_one == copy_two);
	}
	
	SECTION("Comparing direct return values of prefix and postfix incremented iterators produces unequal result")
	{
		auto prefix = begin_it++;
		auto postfix = ++begin_it;
		REQUIRE(prefix != postfix);
	}
	
	SECTION("Combination of begin and end iterators produces length of correct size")
	{
		int count = std::distance(begin_it, end_it);
		REQUIRE(count == SIZE);
	}

}
