#ifndef _ANALYSIS_HPP_INCLUDED
#define _ANALYSIS_HPP_INCLUDED

#include "orchard.hpp"
#include "generator.hpp"
#include <numeric>
#include <functional>
#include <vector>
#include <thread>
#include <future>
#include <execution>
#include <exception>

namespace orchard
{

namespace detail
{
	/**
	 * Helper method that divides a number
	 * a number N into a vector of numbers n_j
	 * with j=1,...,M that
	 * add up to N. All n_j are equal if N%M==0
	 * otherwise the last element is largest
	 * @param N number N
	 * @param M number M
	 * @return a partition as described above
	 */
	template<typename IntegralType>
	std::enable_if_t<std::is_integral_v<IntegralType>,std::vector<IntegralType> > create_partition(IntegralType N, IntegralType M)
	{
		if(M>=N)
		{
			throw std::logic_error("Cannot partition into N into M elements with N <= M");
		}

		std::vector<IntegralType> partition;
		partition.reserve(M);

		if(N%M == 0)
		{
			partition = std::vector<IntegralType>(M,N/M);
		}
		else
		{
			//partition = std::vector<IntegralType>(M-1,N/M);
			//partition.push_back(N/M+N%M);
			partition = std::vector<IntegralType>(M-1,N/(M-1));
			partition.push_back(N%(M-1));

		}
		return partition;
	}


	//! true iff the given execution policy is of class std::execution::parallel_policy or parallel_unsequenced_policy
	template<typename T>
	constexpr bool is_parallel_execution_policy_v = std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>,std::execution::parallel_policy> || std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>,std::execution::parallel_unsequenced_policy>;

	//! true iff the given execution policy is of class std::execution::sequenced_policy
	template<typename T>
	constexpr bool is_sequenced_execution_policy_v = std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>,std::execution::sequenced_policy>;

}


/**
* Calculate the statistics for a given amount of
* random games independently and accumulate the statistics
* for all games.
* 
* See sample statistics for further examples.
* @param strategy The strategy with which to play the game
* @param amount The total number of games to simulate.
* @param initial_statistics An intitial statistic that is 
* the first element of the generalized zum of statistics.
* A statistic_t must satisfy the following:
* -have a constructor taking a game (passed as const lvalue ref)
* -have a an operator + defined which is associative and commutative
* -have a default constructor that returns an "zero" statistic
* which acts as a neutral element for operator +. 
* @param initial The initial game state from which all random
* games are generated.
*/
template<typename statistic_t, typename ExecutionPolicy>
std::enable_if_t<detail::is_sequenced_execution_policy_v<ExecutionPolicy>,statistic_t> accumulate_statistics(ExecutionPolicy && policy, strategy_t strategy, size_t amount, statistic_t initial_statistics, const game_state initial = game_state())
{
	game_generator games(strategy, initial, amount);
	auto result = std::transform_reduce(std::execution::seq,games.cbegin(),games.cend(), initial_statistics, std::plus<statistic_t>(), [](const game_state & g){return statistic_t(g);});
	return result;
}

//! parallel version of the above
template<typename statistic_t, typename ExecutionPolicy>
std::enable_if_t<detail::is_parallel_execution_policy_v<ExecutionPolicy>,statistic_t> accumulate_statistics(ExecutionPolicy && policy, strategy_t strategy, size_t amount, statistic_t initial_statistics, const game_state initial = game_state())
{
	auto hardware_threads = std::thread::hardware_concurrency();
	size_t nthreads = hardware_threads!=0 ? hardware_threads : 1;

	if(nthreads >= amount || nthreads <= 1)
	{
		return accumulate_statistics<statistic_t>(std::execution::seq,strategy,amount,initial_statistics,initial);
	}
	//1) more functional style (~850 ms for 10^6 elements with pick in order)
//	auto amount_per_thread = detail::create_partition(amount, nthreads);
//	auto accumulate_statistics_for_amount = [strategy,initial_statistics,initial](size_t count){return accumulate_statistics<statistic_t>(std::execution::seq,strategy,count,initial_statistics,initial);};
//
//	statistic_t result_stat = std::transform_reduce(policy, amount_per_thread.begin(), amount_per_thread.end(), initial_statistics, std::plus<statistic_t>(), accumulate_statistics_for_amount);
//
//	return result_stat;


//  //2) for loop with std::async (~380 ms for 10^6 elements with pick in order)
//	std::vector<std::future<statistic_t>> results;
//	for(size_t j = 0; j < nthreads-1; ++j)
//	{
//		results.push_back(std::async(std::launch::async,accumulate_statistics<statistic_t,std::execution::sequenced_policy>,std::execution::seq,strategy,amount/(nthreads-1),initial_statistics,initial));
//	}
//	results.push_back(std::async(std::launch::async,accumulate_statistics<statistic_t,std::execution::sequenced_policy>,std::execution::seq,strategy,amount%(nthreads-1),initial_statistics,initial));
//
//	statistic_t result_stat = std::transform_reduce(results.begin(),results.end(),initial_statistics,std::plus<statistic_t>(),[](std::future<statistic_t> & f){return f.get();});
//	return result_stat;


//	//3) solution using openmp: (~370 ms for 10^6 elements with pick in order)
//	std::vector<statistic_t> results(nthreads);
//	#pragma omp parallel for
//	for(size_t j = 0; j < nthreads-1; ++j)
//	{
//			results[j] = accumulate_statistics<statistic_t>(std::execution::seq,strategy,amount/(nthreads-1),initial_statistics,initial);
//	}
//	results[nthreads-1] =accumulate_statistics<statistic_t>(std::execution::seq,strategy,amount%(nthreads-1),initial_statistics,initial);
//	statistic_t result_stat = std::reduce(results.begin(),results.end(), initial_statistics);
//	return result_stat;

	//4) separate transform and reduce and cache the results (~380 ms for 10^6 elements with pick in order)
	// see here https://mariusbancila.ro/blog/2018/05/22/transform-and-reduce-alternatives/
	//for some benchmarks on transform_reduce vs transform then reduce
	auto amount_per_thread = detail::create_partition(amount, nthreads);
	auto accumulate_statistics_for_amount = [strategy,initial_statistics,initial](size_t count){return accumulate_statistics<statistic_t>(std::execution::seq,strategy,count,initial_statistics,initial);};

	std::vector<statistic_t> result_vector(nthreads);
	std::transform(policy, amount_per_thread.cbegin(), amount_per_thread.cend(), result_vector.begin(), accumulate_statistics_for_amount);

	auto result_stat = std::reduce(policy, result_vector.begin(), result_vector.end(), initial_statistics);

	return result_stat;

}
	 
} //namespace orchard

#endif //_ANALYSIS_HPP_INCLUDED
