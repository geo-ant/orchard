#ifndef _STATISTICS_HPP_INCLUDED
#define _STATISTICS_HPP_INCLUDED

#include "orchard.hpp"

namespace orchard
{

/** a simple statistic that counts
* won or lost games and can be used
* with the accumulate_statistics function
* from analysis.hpp
*/
struct win_loss_statistic
{
public:
	win_loss_statistic() {};

	explicit win_loss_statistic(const game_state & g)
	: wins( (is_won(g))?1:0),
	  losses( (is_lost(g)) ? 1 : 0),
	  games(1),
	  turns(g.turn_count)
	{}
	
	win_loss_statistic operator+(const win_loss_statistic & other) const 
	{
		return win_loss_statistic(wins+other.wins, losses+other.losses, games+other.games, turns+other.turns);
	}	
		
	long long get_wins() const {return wins;}
	long long get_losses() const {return losses;}
	long long get_cumulated_turn_count() const {return turns;}
	long long get_game_count() const {return games;}

private:
	win_loss_statistic(long long w, long long l, long long g, long long t) : wins(w), losses(l), games(g), turns(t) {}
	
	//!number of cumulated wins
	long long wins = 0;
	//!number of cumulated losses
	long long losses = 0;
	//!number of cumulated games
	long long games = 0;
	//!number of cumulated turns
	long long turns = 0;
	
};


}
#endif // _STATISTICS_HPP_INCLUDED
