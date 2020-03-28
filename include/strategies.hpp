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
tree_array_t pick_fruit_in_order(const tree_array_t & fruit_count);

/**
 * Picking strategy that picks 2 fruits consequtively
 * and for each pick chooses the tree with the maximum
 * amount of fruit
 */
tree_array_t pick_fruit_from_fullest_tree(const tree_array_t & fruit_count);

/**
 * Picking strategy that picks 2 fruits consequtively
 * and for each pick chooses the emptiest (but nonzero)
 * tree (i.e. the one with the least amount of fruit).
 */
tree_array_t pick_fruit_from_emptiest_tree(const tree_array_t & fruit_count);


/**
 * Picking 2 fruits at random
 */
tree_array_t pick_fruit_at_random(const tree_array_t & fruit_count);

}
#endif //_STRATS_HPP_INCLUDED
