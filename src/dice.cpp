#include "orchard.hpp"
#include "dice.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

namespace orchard
{

dice_result_variant create_random_dice_result()
{
	auto number = get_uniform_random_number<size_t>(0,(TREE_COUNT-1)+2);
	if(number < TREE_COUNT)
	{
		return dice_result<dice_t::TREE_INDEX>(number);
	}
	else if(number == TREE_COUNT)
	{
		return dice_result<dice_t::RAVEN>();
	}
	else
	{
		return dice_result<dice_t::FRUIT_BASKET>();
	}

}

std::string to_string(const dice_result_variant & dice)
{
	auto call_to_string = [](auto && d) -> std::string {return d.to_string();};
	return std::visit(call_to_string,dice);
}


}//namespace orchard


