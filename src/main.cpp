
#include <iostream>
#include <algorithm>
#include "orchard.hpp"
#include "strats.hpp"
#include "analysis.hpp"

int main() {
	using std::cout;
	using std::endl;

	orchard::game_state g;
	//orchard::game_state g_finish = orchard::play_to_finish(orchard::pick_fruit_in_order, std::move(g));


	auto begin_it = orchard::game_iterator::create_begin_iterator(orchard::pick_fruit_in_order, std::move(g));
	auto end_it = orchard::game_iterator::create_end_iterator(2);

	for(auto it = begin_it; it != end_it; ++it)
	{
		cout << "*** NEW GAME:" << endl;
		cout << (*it) << endl << endl;
	}


	//cout << g_finish << endl;
	return 0;
}
