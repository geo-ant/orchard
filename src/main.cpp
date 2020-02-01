
#include <iostream>
#include <algorithm>
#include "orchard.hpp"
#include "strats.hpp"
#include "generator.hpp"

int main() {
	using std::cout;
	using std::endl;

	orchard::game_state g;
	//orchard::game_state g_finish = orchard::play_to_finish(orchard::pick_fruit_in_order, std::move(g));
	
	auto gen = orchard::game_generator(orchard::pick_fruit_in_order,g,3);
	
	std::for_each(gen.cbegin(), gen.cend(), [](const auto & g){cout<<g<<endl;});
	

	//cout << g_finish << endl;
	return 0;
} 
