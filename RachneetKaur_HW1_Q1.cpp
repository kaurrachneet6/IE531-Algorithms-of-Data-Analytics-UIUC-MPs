//  IE 531 HW 1 Ques 1
//  Rachneet Kaur
//  Making an Unfair Coin Fair and generating Unit Normal IID RVs
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>

#define HEADS 1
#define TAILS 0
using namespace std;

unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator(seed);

// Generating Uniform Random No.
double get_uniform()
{
    std::uniform_real_distribution <double> distribution(0.0,1.0);
    double number = distribution(generator);
    return (number);
}

// Tossing an Unfair Coin and recording Heads or Tails
int simulate_coin_toss(double heads_probability)
{
    if (get_uniform() < heads_probability)
        return HEADS;
    else
        return TAILS;
}

// Converting an Unfair Coin to a Fair Coin
int simulate_fair_coin_toss_from_unfair_coin(double heads_probability)
{
    int i=simulate_coin_toss(heads_probability);
    int j=simulate_coin_toss(heads_probability);
    if (i==HEADS && j==TAILS)
        return HEADS;
    else if (i==TAILS && j ==HEADS)
        return TAILS;
    else
        return simulate_fair_coin_toss_from_unfair_coin(heads_probability);
}

//Converting 32 bits of 0/1 to an integer
double get_uniform_from_unfair_coin(double heads_probability)
{
    int k;
    double x,sum=0;  
    for (int n=0; n<32; n++)
    {
        k=simulate_fair_coin_toss_from_unfair_coin(heads_probability);
        sum+=pow(2,n)*k;
    }
    x = sum/pow(2,32);
    return x;
}

// Generating Exponential Random Variables using Unfair Coin Toss
double get_exp_from_unfair_coin(double lambda, double heads_probability)
{
    return ((-1.0/lambda)*log(get_uniform_from_unfair_coin(heads_probability)));
}

// Generating IID Unit Normal Random Varibles
double get_gaussian_from_unfair_coin(double heads_probability)
{
    return (sqrt(-2.0*log(get_uniform_from_unfair_coin(heads_probability)))*cos(2*3.141592654*get_uniform_from_unfair_coin(heads_probability)));
}

int main (int argc, char* argv[])
{
    
    double probability_of_heads, lambda;
    long no_of_trials;
    
    probability_of_heads = get_uniform();   // Defining the probability of Heads in an Unfair Coin
    sscanf (argv[1], "%ld", &no_of_trials);
    sscanf (argv[2], "%lf", &lambda);
    ofstream outfile_1(argv[3]);
    ofstream outfile_2(argv[4]);
    
    cout << "Generating i.i.d. Unit-Normals and Exponentials from an Unfair-Coin Toss" << endl;
    cout << "Probability of Heads of the unfair coin   = " << probability_of_heads << endl;
    cout << "Number of trials                          = " << no_of_trials << endl;
    cout << "Output File Name for the Unit-Normal Data = " << argv[3] << endl;
    cout << "Output File Name for the Exponential Data = " << argv[4] << endl;
    cout << "Rate of Exponential RVs                   = " << lambda << endl;
    
    // Generating IID Exponential RVs
    int count[100];
    for (int i = 0; i < 100; i++)
        count[i] = 0;
    
    for (int i = 0; i < no_of_trials; i++)
    {
        double y = get_exp_from_unfair_coin(lambda, probability_of_heads);
        for (int j = 0; j < 100; j++)
            if (y < ((double) j/10))
                count[j]++;
    }
    
    // Exponential CDF -- Experimental and Theoretical
    for (int j = 0; j < 100; j++)
        outfile_2 << ((double) j/10) << ", " << ((double) count[j])/((double) no_of_trials) << ", " << (1.0-exp(-1.0*lambda*((double) j/10))) << endl;
    
    // Generating IID Standard Normal RVs
    double cdf_gaussian[100];
    for (int i = 0; i < 100; i++)
    {
        count[i] = 0;
        cdf_gaussian[i] = 0.0;
    }
    
    for (int i = 0; i < no_of_trials; i++)
    {
        double y = get_gaussian_from_unfair_coin(probability_of_heads);
        for (int j = 0; j < 100; j++)
            if (y < ((float) (j-50)/10))
                count[j]++;
    }
    // Unit-Normal CDF -- Experimental and Theoretical
    for (int j = 1; j < 100; j++)
        cdf_gaussian[j] = cdf_gaussian[j-1] +
        ((1/sqrt(2*3.141592654))*exp(-1.0*(((double) (j-50)/10)*
                                           ((double) (j-50)/10))/2)/10.0);
    
    for (int j = 0; j < 100; j++)
        outfile_1 << ((double) (j-50)/10) << ", " <<
        ((double) count[j])/((double) no_of_trials) << ", " << cdf_gaussian[j]
        << endl;
    
}
