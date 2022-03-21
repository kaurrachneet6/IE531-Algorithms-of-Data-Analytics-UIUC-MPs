//Rachneet Kaur
//IE 531 Assignment 4
// A-B Testing
// Beta(3,25) as informed Prior, Binomial as the generative model to estimate the posterior
//3rd Coloumn in the output_file denotes Profit B - Profit A

#include <boost/math/distributions.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <algorithm>
#include <fstream>
#include <random>
#include <chrono>
using namespace std;

//Random No. generator
unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);

//Beta distribution for Informed Prior Beta(3,25)
double get_beta(double alpha, double beta)
{
	std::gamma_distribution<double> distribution1(alpha, 1);
	std::gamma_distribution<double> distribution2(beta, 1);
	double number1 = distribution1(generator);
	double number2 = distribution2(generator);
	return (number1 / (number1 + number2));
}

// Binomial as the genrative model 
long get_binomial(long range, double prob_of_success)
{
	std::binomial_distribution<long> distribution(range, prob_of_success);
	double number = distribution(generator);
	return (number);
}

// If the generative model produces subscriptions same as data provided, we accept the rate 
bool generative_model(long range_of_sample_trial, long no_of_subscriptions, double prob_of_success)
{
	bool result = false;
	for (int i = 0; i < range_of_sample_trial; i++)
		if (no_of_subscriptions == get_binomial(range_of_sample_trial, prob_of_success))
			result = true;
	return (result);
}


int main(int argc, char* argv[])
{
	long range_of_sample_trial, no_of_subscriptions_on_sample_trialA, no_of_subscriptions_on_sample_trialB, no_of_trials;
	double profitA, profitB; //Profits for methods A and B
	//profit A = 1000 * rateA - 30, Profit B = 1000 * rateB - 300
	double prior_alpha = 1, prior_beta = 1; // Informative is Beta (3,25)
	sscanf(argv[1], "%ld", &range_of_sample_trial); //n = 16
	sscanf(argv[2], "%ld", &no_of_subscriptions_on_sample_trialA); // Given subscription data for Method A = 6
	sscanf(argv[3], "%ld", &no_of_subscriptions_on_sample_trialB);  // Given subscription data for Method B = 10
	sscanf(argv[4], "%ld", &no_of_trials); // Large no. of trails = 10000000
	ofstream output_file(argv[5]); 
	// Output file containing profit A = 1000*rateA-30, Profit B = 1000*rateB-300 and Profit B - Profit A

	for (long i = 0; i < no_of_trials; i++)
	{
	// Prior Beta distribution generating theta1 and theta2 to be fed to generative model
		double rateA = get_beta(prior_alpha, prior_beta), rateB = get_beta(prior_alpha, prior_beta); //Informative Prior = Beta(3,25)
	//If Generative model produces same subscriptions as given data for both Methods A and B, then we write the corresponding Profits to file
		if (generative_model(range_of_sample_trial, no_of_subscriptions_on_sample_trialA, rateA) && generative_model(range_of_sample_trial, no_of_subscriptions_on_sample_trialB, rateB))
		{
			profitA = rateA * 1000 - 30;
			profitB = rateB * 1000 - 300;
			output_file << profitA << "," << profitB << "," << profitB - profitA << endl;
			// Writing profits for both methods and the difference to .csv file
		}
	}
}