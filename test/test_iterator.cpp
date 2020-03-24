#include "catch2/catch.hpp"
#include "orchard.hpp"
#include "strategies.hpp"
#include "generator.hpp"

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

TEST_CASE("Iterators produce the correct ranges","[game_iterator]")
{
	constexpr int SIZE = 10;
	auto begin_it = game_iterator::create_begin_iterator(pick_fruit_in_order, game_state());
	auto end_it = game_iterator::create_end_iterator(SIZE);
	
	constexpr long LARGE_ISH_NUMBER = 1000;
	
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
		int count = 0;
		for(auto it = begin_it; it != end_it; ++it)
		{
			++count;
		}
		REQUIRE(count == SIZE);
	}
	
	SECTION("Dereferencing the same iterator multiple times produces the same result")
	{
		
	}
	
}