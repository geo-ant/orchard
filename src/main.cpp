
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <exception>
#include "orchard.hpp"
#include "strategies.hpp"
#include "generator.hpp"
#include "analysis.hpp"
#include "statistics.hpp"

struct named_strategy
{
	//!strategy function
	orchard::strategy_t strategy;
	//!human readable name
	std::string name;
};

//!let user choose a strategy from a vector of strats
orchard::strategy_t let_user_choose_strategy(const std::vector<named_strategy> &);

int main() {
	using std::cout;
	using std::cin;
	using std::endl;
	using namespace orchard;
	
	std::vector<named_strategy> strategies;
	strategies.push_back(named_strategy{&pick_fruit_in_order, "Pick in Order"});
	strategies.push_back(named_strategy{&pick_fruit_at_random, "Pick at Random"});
	strategies.push_back(named_strategy{&pick_fruit_from_fullest_tree, "Pick fullest tree"});
	strategies.push_back(named_strategy{&pick_fruit_from_emptiest_tree, "Pick emptiest tree (greedy)"});


	cout << "***Number of Games to Simulate:" << endl;
	cout << "N = ";
	size_t cnt = 0;
	cin >> cnt;

	auto strategy = let_user_choose_strategy(strategies);

	auto wlstat = accumulate_statistics<win_loss_statistic>(strategy,cnt);
	
	cout << "STATISTICS:" << endl;
	cout << "Wins: " << wlstat.get_wins() << " Losses: " << wlstat.get_losses() << endl;
	cout << "Average Turn count (rounded): " << wlstat.get_cumulated_turn_count()/wlstat.get_game_count() << endl;
	cout << "(Total cumulated turns: " << wlstat.get_cumulated_turn_count() << ")" << endl;
	//cout << g_finish << endl;
	return 0;
}

orchard::strategy_t let_user_choose_strategy(const std::vector<named_strategy> & strats)
{
	using std::cout;
	using std::cin;
	using std::endl;

	if(strats.size() == 0)
	{
		throw std::logic_error("Number of strategies to chose is zero!");
	}

	cout << "*** Chose Strategy: " << endl;
	for(size_t idx = 0; idx < strats.size(); ++idx)
	{
		cout << idx <<  ") " << strats.at(idx).name << endl;
	}

	size_t chosen_idx = strats.size();
	do
	{
		cout << "Strategy = ";
		cin >> chosen_idx;
	}while(chosen_idx >= strats.size());

	return strats.at(chosen_idx).strategy;
}

