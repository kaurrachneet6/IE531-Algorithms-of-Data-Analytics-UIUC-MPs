// IE 531, HW 8
// Rachneet Kaur
// Flajolet-Martin-Szegedy Algorithm to estimate Second Moment
// AIM: To estimate best S value that achieves least error in estimation of second moment 
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <bitset>
#include <set>
using namespace std;

unsigned long long mix(unsigned long long a, unsigned long long b, unsigned long long c) {
	a = a - b;  a = a - c;  a = a ^ (c >> 13);
	b = b - c;  b = b - a;  b = b ^ (a << 8);
	c = c - a;  c = c - b;  c = c ^ (b >> 13);
	a = a - b;  a = a - c;  a = a ^ (c >> 12);
	b = b - c;  b = b - a;  b = b ^ (a << 16);
	c = c - a;  c = c - b;  c = c ^ (b >> 5);
	a = a - b;  a = a - c;  a = a ^ (c >> 3);
	b = b - c;  b = b - a;  b = b ^ (a << 10);
	c = c - a;  c = c - b;  c = c ^ (b >> 15);
	return c;
}

int main(int argc, char* argv[]) {
	ofstream outfile_1(argv[3]);       // File to store less varied estimates for second moment for each S = 1 to 100
	unsigned long long seed1[100];     // Different seeds
	unsigned long long mean[100];      // Different estimates for different seeds
	vector<long long> avg_no_elements; // To compute average of bins
	vector <pair <string, unsigned long>> X_xcount;
	for (int k = 0; k < 100; k++)       // Generating seeds
	{
		seed1[k] = mix(clock(), time(NULL), clock() * time(NULL));
	}

	for (int S = 1; S <= 100; S++)      // We are finding best S out of S from 1 to 100
	{
		for (int k = 0; k < 100; k++)   // For each S, using a distinct seed, finding an estimate and then binning to get less varied estimate 
		{
			unsigned long long no_of_words = atol(argv[2]);

			cout << "Current Seed: " << std::bitset<32>(seed1[k]) << endl;

			unsigned int idx_to_del;
			map <string, set <unsigned int>> X_listIdx;
			pair <string, unsigned long> p;
			unsigned long n = -1;
			mt19937 mt(seed1[k]);
			std::uniform_real_distribution<double> distribution(0.0, 1.0);

			// To get the random index to remove from X_xcount
			typedef uniform_int_distribution<unsigned long long> my_uniform_int;
			my_uniform_int UIID_INT(0, S - 1);

			unsigned long long i = 0;
			std::ifstream file;
			file.open(argv[1]);
			string a;
			while (getline(file, a) && i < no_of_words) {
				i++;
				if (a.empty()) {
					a = "\n";
				}
				n++;

				if (n < S) {
					// Get S-many estimators first
					unsigned int index = X_xcount.size();
					p = pair<string, unsigned long>(a, 0);
					X_xcount.push_back(p);
					X_listIdx[a].insert(index);
				}
				else {
					// check if the new stream-element should replace one of the S-many estimators
					if (distribution(mt) < ((double)S) / ((double)n)) {
						// figuring out which of the S-many estimators is to be deleted
						idx_to_del = (int)UIID_INT(mt);

						// Remove the index of element to be deleted from its vector
						X_listIdx[X_xcount[idx_to_del].first].erase(idx_to_del);
						if (X_listIdx[X_xcount[idx_to_del].first].size() == 0) {
							X_listIdx.erase(X_xcount[idx_to_del].first);
						}

						// Replace this estimator by the new one
						p = pair<string, unsigned long>(a, 0);
						X_xcount[idx_to_del] = p;
						X_listIdx[a].insert(idx_to_del);
					}
				}

				// Increase the wordcount for all occurrences of word in transformed list
				for (unsigned int j : X_listIdx[a]) {
					X_xcount[j].second++;
				}
			}
			file.close();
			//cout << "#Elements seen: " << n + 1 << "; " << "#Estimators: " << S << endl;

			mean[k] = 0;


			for (auto v : X_xcount) {
				mean[k] += (no_of_words) * ((2 * v.second) - 1);
			}

		}
		for (int i = 0; i < 100; i = i + 10)  // Binning the 100 estimates of second moment for fixed S
		{
			double avg = 0;
			for (int j = i; j < i + 10; j++)  // Bins with 10 estimates each
			{
				avg = avg + mean[j];         // Averaging the bin estimates
			}
			avg = avg / 10;
			avg_no_elements.push_back(avg);   // Average of all the bins
		}

		int size = avg_no_elements.size();

		// Calculating the median average of bins
		sort(avg_no_elements.begin(), avg_no_elements.end());
		double median;
		if (size % 2 == 0) {
			median = (avg_no_elements[size / 2] + avg_no_elements[size / 2 - 1]) / 2;
		}
		else
			median = (avg_no_elements[(size - 1) / 2]);
		outfile_1 << S << "," << median / X_xcount.size() << endl;  // The median value is a less varied estimate of second moment for fixed S 
		/* The less varied estimates for each of S values are stored in a file where we compute the error in the estimates
		(Absolute difference of FMS estimate and Actual value) and plot the Error versus S values, which attains a global minimum point
		The point where global minima is attained is the ideal S we need.
		*/

	}

	return 0;
}
