// Rachneet Kaur
// IE 531 HW 2
// Randomized selection of kth smallest number in a list

#include <iostream>
#include <cmath>
#include <vector>
#include <time.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
using namespace std;

 unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
default_random_engine generator (seed);
//Generates Uniform random variables
double get_uniform()
{
	std::uniform_real_distribution <double> distribution(0.0, 1.0);
	double number = distribution(generator);
	return (number);
}
// Cauchy distribution
double get_standard_cauchy()
{
	return tan(-1.570796327 + (get_uniform()*3.141592654));
}

//Quick Sort
double selection_from_quick_sort(vector <double> & list, long k)
{
	sort(list.begin(), list.end());
	return (list[k - 1]);
}

// Randomized selection
double random_selection(vector <double> & list, long k)
{
	double p;
	int i = get_uniform()*(list.size()-1);
	p = list[i];
	// Dividing into 3 parts
	vector <double> less_than_p;
	vector <double> equal_to_p;
	vector <double> greater_than_p;
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] < p)
				less_than_p.push_back(list[i]);
		if (list[i] == p)
				equal_to_p.push_back(list[i]);
		if (list[i] > p)
				greater_than_p.push_back(list[i]);
	}
	if (k <= less_than_p.size())
		return (random_selection(less_than_p, k));
	else if (k >(less_than_p.size() + equal_to_p.size()))
		return (random_selection(greater_than_p, (k - less_than_p.size() - equal_to_p.size())));
	else
		return (p);
}

int main(int argc, char* argv[])
{
	int m, n;
	clock_t time_before, time_after;
	float diff;
	sscanf_s(argv[1], "%d", &n);  // Size of the list
	sscanf_s(argv[2], "%d", &m);  // No. of trails
	ofstream outfile_1(argv[3]);  // csv file to record running times

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
	cout << "-------------" << endl;
	cout << "Randomized Selection Based Method" << endl;

	for (int j = 0; j < m; j++) 
	{
		time_before = clock(); // recording time before Randomized Selection algorithm starts
		random_selection(my_list, n/2);
		time_after = clock(); // recording time after Randomized Selection algorithm finishes
		diff = ((float)time_after - (float)time_before);
		outfile_1 << j << ", " << diff / CLOCKS_PER_SEC << endl;
	}

	cout << "The (" << n / 2 << ")-th smallest element in a " <<
		my_list.size() << "-long list is " << random_selection(my_list, n/2) << endl;

	cout << "-------------" << endl;
	cout << "Quick Sort Based Method" << endl;
	time_before = clock(); // recording time before quick sort starts
	cout << "The (" << copy_of_my_list.size()/2 << ")-th smallest element in a ";
	cout << copy_of_my_list.size() << "-long list is " << selection_from_quick_sort(copy_of_my_list, copy_of_my_list.size() / 2) << endl;
	time_after = clock(); // recording time after quick sort finishes
	diff = ((float)time_after - (float)time_before);
	cout << "It took " << diff / CLOCKS_PER_SEC << " seconds to complete" << endl;
	cout << "-------------" << endl;
	outfile_1 << m + 1 << ',' << diff / CLOCKS_PER_SEC << endl;
}