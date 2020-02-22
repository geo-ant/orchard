#ifndef MAKE_TESTS


#include <iostream>
#include <algorithm>
#include "orchard.hpp"
#include "strategies.hpp"
#include "generator.hpp"
#include "analysis.hpp"
#include "statistics.hpp"


int main() {
	using std::cout;
	using std::cin;
	using std::endl;
	using namespace orchard;
	
	cout << "Number of Games to Simulate:" << endl;
	cout << "N = ";
	size_t cnt = 0;
	cin >> cnt;

	auto wlstat = accumulate_statistics<win_loss_statistic>(pick_fruit_at_random,cnt);
	
	cout << "STATISTICS:" << endl;
	cout << "Wins: " << wlstat.get_wins() << " Losses: " << wlstat.get_losses() << endl;

	//cout << g_finish << endl;
	return 0;
} 
#else
#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#endif
