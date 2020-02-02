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
	win_loss_statistic() : win_loss_statistic(0,0) {}
	win_loss_statistic(const game_state & g)
	: wins( (is_won(g))?1:0), losses( (is_lost(g)) ? 1 : 0) {}
	
	win_loss_statistic operator+(const win_loss_statistic & other) const 
	{
		return win_loss_statistic(wins+other.wins, losses+other.losses);
	}	
		
	long get_wins() {return wins;}
	long get_losses() {return losses;}
	
private:
	win_loss_statistic(long w, long l) : wins(w), losses(l) {}
	
	long wins;
	long losses;
	
};


}
#endif // _STATISTICS_HPP_INCLUDED