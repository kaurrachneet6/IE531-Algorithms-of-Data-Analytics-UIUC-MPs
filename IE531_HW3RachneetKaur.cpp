//Rachneet Kaur
// IE 531 HW 3
// Deterministic Select Algorithm to find 'k'th smallest no. for any value of 'm' (Size of the sublist) in 'n' long list
// Command Line Arguments - n(Size of the list), m(Size of the sublist), k(for the Kth smallest no.)

#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <algorithm>
#include <random>
#include <chrono>
#include<fstream>
using namespace std;

unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);
//default_random_engine generator;

// Generates uniform random numbers
double get_uniform()
{
	std::uniform_real_distribution <double> distribution(0.0, 1.0);
	double number = distribution(generator);
	return (number);
}
// Generates Cauchy Random Variables
double get_standard_cauchy()
{
	return(tan(-1.570796327 + (get_uniform()*3.141592654)));
}
// Bubble Sort
double selection_from_bubble_sort(vector <double> & list, long k)
{
	int flag = 1;
	float temp;
	for (int i = 1; (i <= list.size()) && flag; i++) {
		flag = 0;
		for (int j = 0; j < (list.size() - 1); j++)
			if (list[j + 1] < list[j]) {
				temp = list[j];
				list[j] = list[j + 1];
				list[j + 1] = temp;
				flag = 1;
			}
	}
	//kTh emlement from the sorted list
	return (list[k - 1]);
}

//Deterministic Select with user defined size of sublist sz
double deterministic_selection(vector <double> & list, long k, int sz)
{
	double median_of_medians;
	vector <double> list_of_medians;
	if (list.size() <= sz)
		return (selection_from_bubble_sort(list, k));
	else {
		for (int i = 0; i < list.size(); i = i + sz)
		{   vector <double> size_element_long_segment;
			size_element_long_segment.push_back(list[i]);
			for (int j = 1;j < sz;j++)
				if ((i + j) < list.size())
					size_element_long_segment.push_back(list[i + j]);
			// pick the median of the sz (User Defined)-element list
			//if (size_element_long_segment.size() == 1) list_of_medians.push_back(size_element_long_segment[0]);
		      if (size_element_long_segment.size() % 2 != 0) {
				list_of_medians.push_back(deterministic_selection(size_element_long_segment, (size_element_long_segment.size() + 1) / 2, sz));
			}
			else {
				list_of_medians.push_back((deterministic_selection(size_element_long_segment, size_element_long_segment.size() / 2, sz) + deterministic_selection(size_element_long_segment, size_element_long_segment.size()/2 + 1, sz))/2.0);
			}
		}
		}
		if (list_of_medians.size() % 2 == 1) // list of medians is odd length, choose the middle element
			median_of_medians = deterministic_selection(list_of_medians, ((list_of_medians.size() - 1) / 2) + 1,sz);
		else // list of medians is even length, choose the mean if the 2 middle elements
			median_of_medians =
			(deterministic_selection(list_of_medians, (list_of_medians.size()) / 2,sz) +
			(deterministic_selection(list_of_medians, ((list_of_medians.size()) / 2) + 1, sz))) / 2.0;

		// split list into three parts: less_than_median_of_medians, equal_to_median_of_medians, and greater-than-median_of_medians 
		vector <double> less_than_median_of_medians;
		vector <double> equal_to_median_of_medians;
		vector <double> greater_than_median_of_medians;
		for (int i = 0; i < list.size(); i++) {
			if (list[i] < median_of_medians)
				less_than_median_of_medians.push_back(list[i]);
			if (list[i] == median_of_medians)
				equal_to_median_of_medians.push_back(list[i]);
			if (list[i] > median_of_medians)
				greater_than_median_of_medians.push_back(list[i]);
		}
		if (k <= less_than_median_of_medians.size())
			return (deterministic_selection(less_than_median_of_medians, k,sz));
		else if (k >(less_than_median_of_medians.size() + equal_to_median_of_medians.size()))
			return (deterministic_selection(greater_than_median_of_medians, (k - less_than_median_of_medians.size() - equal_to_median_of_medians.size()),sz));
		else
			return (median_of_medians);
	}

int main(int argc, char* argv[])
{
	int no_of_trials, size;
	clock_t time_before, time_after;
	float diff;
	sscanf(argv[1], "%d", &no_of_trials); // Size of the list
	sscanf(argv[2], "%d", &size);         // Size of the sublists
    vector <double> my_list;
	vector <double> copy_of_my_list;
	for (int i = 0; i < no_of_trials; i++) 
	{
		double x = get_standard_cauchy();
		my_list.push_back(x);
		copy_of_my_list.push_back(x);
	}
	//k = my_list.size() / 2 + 1;
	cout << "Number of trials = " << no_of_trials << endl;
	cout << "-------------" << endl;
	cout << "Size of sublists = " << size << endl;
	cout << "-------------" << endl;
	cout << "Deterministic Selection Based Method" << endl;
	time_before = clock(); // recording time before Deterministic Selection algorithm starts
	cout << "The (" << my_list.size() / 2 +1<< ")-th smallest element in a " <<
		my_list.size() << "-long list is " << deterministic_selection(my_list, my_list.size() / 2 +1 , size) << endl;
	time_after = clock(); // recording time after Deterministic Selection algorithm finishes
	diff = ((float)time_after - (float)time_before);
	cout << "It took " << diff / CLOCKS_PER_SEC << " seconds to complete" << endl;
}