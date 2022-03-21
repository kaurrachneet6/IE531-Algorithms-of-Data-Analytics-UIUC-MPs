// Rachneet Kaur
// IE531 HW 1 Ques 2
// Generating IID Unit Normal RVs from 3 sided unfair die by making it fair 
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <random>
#include <chrono>
#include <cmath>
#include <fstream>

#define HEADS 1
#define TAILS 0
#define EVENT1 1
#define EVENT2 2
#define EVENT3 3
using namespace std;

double prob1, prob2, prob3;
unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);

// Generating a Uniform Random No.
double get_uniform()
{
    std::uniform_real_distribution <double> distribution(0.0,1.0);
    double number = distribution(generator);
    return (number);
}

// Defining the probabilities for 3 sides of the die
void assign_random_probabilities_to_three_events()
{
    prob1 = get_uniform();
    prob2 = get_uniform();
    while (prob2 >= 1-prob1)
    {
        prob2 = get_uniform();
    }
    prob3 = 1-prob1-prob2;
    }

//Throwing the unfair 3 sided die and recording its outcome
int three_event_generator(double prob1, double prob2, double prob3)
{
    float k = get_uniform();
    if (k <= prob1)
        return (EVENT1);
    else if (k <= (prob1+prob2))
        return(EVENT2);
    else
        return(EVENT3);
}

// Converting an Unfair Die into a fair coin toss
int simulate_fair_coin_toss_from_three_event_generator(double prob1, double prob2, double prob3)
{
    int i,j;
    i = three_event_generator(prob1,prob2,prob3);
    j = three_event_generator(prob1,prob2,prob3);
    if ((i==EVENT1 && j==EVENT2) || (i==EVENT1 && j==EVENT3) || (i==EVENT2 && j==EVENT3))
        return (HEADS);
    else if ((i==EVENT2 && j==EVENT1) || (i==EVENT3 && j==EVENT1) || (i==EVENT3 && j==EVENT2))
        return(TAILS);
    else
        return simulate_fair_coin_toss_from_three_event_generator(prob1,prob2,prob3);
    
}

//Converting 32 bits of 0/1 to an integer
double get_uniform_from_three_event_generator(double prob1, double prob2)
{
    int k;
    double x,sum=0;
    
    for (int n=0; n<32; n++)
    {
        k = simulate_fair_coin_toss_from_three_event_generator(prob1,prob2,prob3);
        sum+=pow(2,n)*k;
    }
    x = sum/pow(2,32);
    return x;
}

// Generating Exponential Random Variables using Unfair Coin Toss
double get_exp_from_unfair_coin(double lambda, double prob1, double prob2)
{
    return ((-1.0/lambda)*log(get_uniform_from_three_event_generator(prob1, prob2)));
}

// Generating IID Unit Normal Random Varibles
double get_gaussian_from_unfair_coin(double prob1, double prob2)
{
    return (sqrt(-2.0*log(get_uniform_from_three_event_generator(prob1, prob2)))*cos(2*3.141592654*get_uniform_from_three_event_generator(prob1, prob2)));
}

int main (int argc, char* argv[])
{
    
    double lambda;
    long no_of_trials;
    
    sscanf (argv[1], "%ld", &no_of_trials);
    sscanf (argv[2], "%lf", &lambda);
    ofstream outfile_1(argv[3]);
    ofstream outfile_2(argv[4]);
    
    assign_random_probabilities_to_three_events();
    
    cout << "Generating i.i.d. Unit-Normals and Exponentials from a 3-Event Generator" << endl;
    cout << "Probability of EVENT1                     = " << prob1 << endl;
    cout << "Probability of EVENT2                     = " << prob2 << endl;
    cout << "Probability of EVENT3                     = " << prob3 << endl;
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
        double y = get_exp_from_unfair_coin(lambda, prob1, prob2);
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
        double y = get_gaussian_from_unfair_coin(prob1, prob2);
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