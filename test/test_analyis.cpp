#include <vector>
#include <cmath>

#include "catch2/catch.hpp"
#include "orchard.hpp"
#include "analysis.hpp"
#include "statistics.hpp"
#include <execution>

using namespace orchard;

/** estimate sigma of binomial distribution with given number of successes and trials
 * https://en.wikipedia.org/wiki/Binomial_distribution#Estimation_of_parameters
 * */
double estimate_sigma_binomial(size_t number_successes, size_t number_trials);

TEST_CASE("Results of Sequential vs Parallel analysis: WinLossStatistic","[analysis_wlstat]")
{
	/**
	 * For all strategies test that the win/loss statistic
	 * produce the same results within a couple of standard
	 * deviations with parallel and sequential implementations
	 * For that we estimate the variance
	 * for the win count from the Binomial Distribution
	 * (var= n*p*(1-p)) and the variance of the number of
	 * games from the Poisson Distribution (var = mu). Then we
	 * assume that the amount of trials is sufficiently big to
	 * make both distributions sufficiently gaussian that we
	 * can test that the results are within a certain sigma
	 * of each other.
	 */

	size_t amount = 300000;

	std::vector<strategy_t> strategies;
	strategies.push_back(&pick_fruit_in_order);
	strategies.push_back(&pick_fruit_at_random);
	strategies.push_back(&pick_fruit_from_fullest_tree);
	strategies.push_back(&pick_fruit_from_emptiest_tree);


	//! minimun turn count before a game can be theoretically finished
	const auto MINIMUM_TURN_COUNT = MAX_RAVEN_COUNT;

	for(auto strategy : strategies)
	{
		auto wlstat_par = accumulate_statistics(std::execution::par,strategy,amount,win_loss_statistic(), game_state());
		auto wlstat_seq = accumulate_statistics(std::execution::seq,strategy,amount,win_loss_statistic(), game_state());
		//auto wlstat_par = wlstat_seq;

		//! only sanity checks about the total amount of games and wins / losses
		auto sum_wl_equals_total = [](const win_loss_statistic & wls){return wls.get_wins()+wls.get_losses()==wls.get_game_count();};
		REQUIRE(sum_wl_equals_total(wlstat_par) == true);
		REQUIRE(sum_wl_equals_total(wlstat_seq) == true);
		REQUIRE(wlstat_par.get_game_count() == amount);
		REQUIRE(wlstat_seq.get_game_count() == amount);
		REQUIRE(wlstat_par.get_cumulated_turn_count() > amount*MINIMUM_TURN_COUNT);
		REQUIRE(wlstat_seq.get_cumulated_turn_count() > amount*MINIMUM_TURN_COUNT);

		/**
		 * Compare the number of wins
		 */

		double sigma_par = estimate_sigma_binomial(wlstat_par.get_wins(),wlstat_par.get_game_count());
		double sigma_seq = estimate_sigma_binomial(wlstat_par.get_wins(),wlstat_par.get_game_count());
		double difference_in_wins = wlstat_par.get_wins()-wlstat_seq.get_wins();

		REQUIRE(std::abs(difference_in_wins) < 6*sigma_par);
		REQUIRE(std::abs(difference_in_wins) < 6*sigma_seq);

		/**
		 * Compare the number of turns
		 * Estimate the distrubution (cumulated turns - number of games * min turns per game)
		 * as poisson and thus estimate the sigma as sigma = sqrt(mu)
		 * then again compare using gaussian approx
		 */

		double sigma_turns_par = std::sqrt(wlstat_par.get_cumulated_turn_count() - amount*MINIMUM_TURN_COUNT);
		double sigma_turns_seq = std::sqrt(wlstat_seq.get_cumulated_turn_count() - amount*MINIMUM_TURN_COUNT);

		double difference_turns = wlstat_seq.get_cumulated_turn_count()-wlstat_par.get_cumulated_turn_count();

		REQUIRE(std::abs(difference_turns) < 6*sigma_turns_seq);
		REQUIRE(std::abs(difference_turns) < 6*sigma_turns_par);

	}

}

double estimate_sigma_binomial(const size_t number_successes, const size_t number_trials)
{
	//variance var = n*p*q = n*p*n*q/n = number_successes*(number_trials-number_successes)/number_trials

	double Ns = number_successes;
	double Nt = number_trials;
	double var = Ns*(Nt-Ns)/Nt;
	return std::sqrt(var);
}

