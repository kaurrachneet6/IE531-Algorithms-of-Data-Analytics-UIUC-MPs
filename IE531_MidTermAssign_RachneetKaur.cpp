//Rachneet Kaur
// IE 531 Mid Term Assignment
// Finding how many pivot cuts are optimal in terms of running time
// Using the randomized version of median of medians algorithm to compute median
#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;
default_random_engine generator;

//Generating an uniform random no.
double get_uniform()
{
	std::uniform_real_distribution <double> distribution(0.0, 1.0);
	double number = distribution(generator);
	return (number);
}

//Cauchy distribution to generate Random variables
double get_standard_cauchy()
{
	return tan(-1.570796327 + (get_uniform()*3.141592654));
}

// nopivot - No. of pivots
double Randomized_selection(vector <double> & list, long k, int nopivot)
{
	double p;
	int i;
	long p1size = list.size();
	vector <double> shortest_one;
	for (int j = 0; j < nopivot; j++)
	{
		i = get_uniform()*(list.size() - 1);
		p = list[i];

		// split list into three parts, less_than_median_of_medians, equal_to_median_of_medians, and greater-than-median_of_medians 
		vector <double> less_than_p;
		vector <double> equal_to_p;
		vector <double> greater_than_p;
		// pushing elements in the appropriate lists
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i] < p)
				less_than_p.push_back(list[i]);
			if (list[i] == p)
				equal_to_p.push_back(list[i]);
			if (list[i] > p)
				greater_than_p.push_back(list[i]);
		}
		// Choosing the shortest list out of the one chosen from 'nopivot' no. of pivots
		if (k <= less_than_p.size())
		{
			if (p1size >= less_than_p.size()) // Comparing with the earlier chosen sublist size
			{
				p1size = k;    shortest_one = less_than_p;
			}
		}

		else if (k > (less_than_p.size() + equal_to_p.size()))
		{
			if (p1size >= k - less_than_p.size() - equal_to_p.size())
			{
				p1size = k - less_than_p.size() - equal_to_p.size();
				shortest_one = greater_than_p;
			}
		}
		else
			return (p);
	}
	return (Randomized_selection(shortest_one, p1size, nopivot));
}

int main(int argc, char* argv[])
{
	int m, n, nopivot;
	// n-List size, m- No. of trails, nopivot - No. of pivots
	clock_t time_before, time_after;
	double element;
	float diff;

	sscanf_s(argv[1], "%d", &n);          //List size
	sscanf_s(argv[2], "%d", &m);         //No. of trails
	sscanf_s(argv[3], "%d", &nopivot);    //No. of pivots
	ofstream outfile_1(argv[4]);        // File to store running times

	vector <double> my_list;
	vector <double> copy_of_my_list;
	for (int i = 0; i < n; i++)
	{
		double x = get_standard_cauchy();
		my_list.push_back(x);
		copy_of_my_list.push_back(x);
	}
	cout << "Number of elements in my_list = " << n << endl;
	cout << "Number of trials = " << m << endl;
	cout << "Number of pivots = " << nopivot << endl;
	cout << "-------------" << endl;
	cout << "Randomized Version of Median of Medians algorithm to find median" << endl;
	for (int j = 0; j < m; j++)
	{
		time_before = clock(); // recording time before Randomized Median of medians starts
		Randomized_selection(my_list, n / 2, nopivot);
		time_after = clock(); // recording time after Randomized Median of medians finishes
		diff = ((float)time_after - (float)time_before); // Running time
		outfile_1 << j << ", " << diff / CLOCKS_PER_SEC << endl; // Storing the running time in the file
	}
	cout << "The (" << n / 2 << ")-th smallest element in a " <<
		my_list.size() << "-long list is " << Randomized_selection(my_list, n / 2, nopivot) << endl;

}






