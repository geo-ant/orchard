#ifndef _STRATS_HPP_INCLUDED
#define _STRATS_HPP_INCLUDED

#include "orchard.hpp"

namespace orchard
{

class in_order_picking_strategy : public picking_strategy
{
public:

	game_state pick_fruits(game_state &&) const override;

};

}
#endif //_STRATS_HPP_INCLUDED
