#include "orchard.hpp"
#include "dice.hpp"
#include <stdexcept>
#include <iostream>
#include <string>

namespace orchard
{
dice_result::dice_result(int res): result(res)
{
	if(result < 0 || result > TREE_COUNT+1)
	{
		throw std::logic_error("Dice result outside allowed range!");
	}
}

dice_result dice_result::create_random()
{
    return dice_result(get_uniform_random_number<size_t>(0,TREE_COUNT+1));
}

bool dice_result::is_tree_index() const
{
	return result < TREE_COUNT;
}

bool dice_result::is_fruit_basket() const
{
	return result == FRUIT_BASKET;
}

bool dice_result::is_raven() const
{
	return result == RAVEN;
}

std::optional<int> dice_result::get_tree_index() const
{
	if(is_tree_index())
	{
		return result;
	}
	else
	{
		return {};
	}
}

std::string dice_result::to_string() const
{
	if(is_raven())
	{
		return std::string("R");
	}
	else if(is_fruit_basket())
	{
		return std::string("F");
	}
	else
	{
		return std::to_string(result);
	}
}

}//namespace orchard


