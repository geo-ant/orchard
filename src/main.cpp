
#include <iostream>
#include <algorithm>
#include "orchard.hpp"
#include "strats.hpp"
#include "analysis.hpp"

int main() {
	using std::cout;
	using std::endl;

	orchard::game_state g;
	orchard::in_order_picking_strategy order_strat;
	orchard::random_picking_strategy rnd_strat;
	orchard::game_state g_finish = orchard::play_to_finish(std::move(g), order_strat);


	cout << g_finish << endl;
	return 0;
}
