//Rachneet Kaur
//IE 531 HW 7
// Flajolet Martin Algorithm to count no. of distinct words in Twitter stream

#include <cstdlib>
#include <cmath>
#include <functional>
#include <string>
#include <iostream>
#include <map>
#include <chrono>
#include <vector>
#include <algorithm>
#include <ctime>
#include "hashes.h"
using namespace std;

void set_bit(unsigned long long &bit_str, long long pos)
{
	long long mask = 0x1 << pos;
	bit_str = bit_str | mask;
}

unsigned long long rho(unsigned long hashVal)
{
	// computing the tail of 0s
	unsigned long long rho = 0;
	int mask = 0x1;
	if (hashVal != 0)
	{
		// while-loop is executed only of the least-significant-bit of hashVal is 0
		while ((hashVal & mask) == 0)
		{
			rho++;
			hashVal = hashVal >> 1; // right-shift the hashVal by 1 bit position
		}
	}
	return rho;
}

unsigned long long get_estimated_distinct_nums(unsigned long long bit_str)
{
	double phi = 0.77351;
	unsigned long long pos = 0;
	int mask = 0x1;

	while ((bit_str & mask) != 0)
	{
		bit_str = bit_str >> 1;
		pos++;
	}
	return (pow(2, pos)/phi);
}

int main(int argc, char *argv[])
{
	vector<int> avgs_bin;                     //Bin Averages
	unsigned long long random_seed[1000];     //Seeds for Hash functions
	unsigned long long bit_fields[1000];     //Max length of tail of 0s
	for (int i = 0; i < 1000; ++i)			 
	{
		bit_fields[i] = 0;
	}
	int distinct_estimate[1000];             //Estimated Distinct words 
	unsigned int bit_limit = 0;
	string a;

	typedef std::chrono::high_resolution_clock myclock;
	myclock::time_point beginning = myclock::now();
	for (int i = 0; i < 1000; ++i)    // Taking 1000 seeds each for a hash function
	{
		myclock::duration d = myclock::now() - beginning;
		random_seed[i] = d.count();
	}
	string hashFunc = "mm2";
	ios::sync_with_stdio(false);
	
	
	while (getline(cin, a))                            //read the stream
	{
		if (a.empty())
		{
			a = "\n";       
		}
		for (int i = 0; i < 1000; ++i)               // Update max value of tail 0s for each hash function
		{
			set_bit(bit_fields[i], int(rho(hashstr(hashFunc, a, bit_limit, random_seed[i], random_seed[i]))));
		}
	}
	
	for (int i = 0; i < 1000; ++i)
	{
		distinct_estimate[i] = get_estimated_distinct_nums(bit_fields[i]);    // No. off distinct words as computed by each hash function 
	}
	
	for (int i = 0; i < 1000; i += 50)
	{
		int avg = 0;                                // Average for each bin needs to be computed
		for (int j = 0; j < 50; j++)
			avg = avg + distinct_estimate[i+j];    // Computing average for bins with 50 elements each
		avg = avg / 50;
		avgs_bin.push_back(avg);                   // Appending the average for each bin in the vector
	}
	sort(avgs_bin.begin(), avgs_bin.end());        // Median for the vector with bin's averages

	int MedianOfAvgs;
	if (avgs_bin.size() % 2 == 0) 
		MedianOfAvgs = (avgs_bin[avgs_bin.size() / 2] + avgs_bin[avgs_bin.size() / 2 - 1]) / 2;
	else
	MedianOfAvgs = (avgs_bin[(avgs_bin.size() - 1) / 2]);
	cout << "Flajolet Martin estimate of distinct elements in the stream = " << MedianOfAvgs << endl;
	return 0;
}

