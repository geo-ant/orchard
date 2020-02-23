#ifndef _STRATS_HPP_INCLUDED
#define _STRATS_HPP_INCLUDED

#include "orchard.hpp"
#include <functional>

namespace orchard
{


/**
 * Picking strategy that picks 2 fruits in ascending
 * order of index (starting with 0).
 */
game_state pick_fruit_in_order(const game_state & g);
/**
 * Picking 2 fruits at random
 */
game_state pick_fruit_at_random(const game_state & g);

}
#endif //_STRATS_HPP_INCLUDED
