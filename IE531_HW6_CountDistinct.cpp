// Rachneet Kaur
// IE 531 Assignment 6
// Count Distinct for Twitter Data set
#include<iostream>
#include<math.h>
#include<string>
#include<unordered_map>
using namespace std;
int main(int argc, const char * argv[])
{
	unordered_map <string, long long> word_count;
	unordered_map <string, long long> ::iterator i;
	string w;                   // To hold the word
	long long surprise_no =0;  // Second moment of frequency
	while (getline(cin,w))    
	{
		//cout << w;
		i = word_count.find(w);                        // Checking if element is already in the map
		if (i == word_count.end())
			word_count.insert(make_pair(w, 1));       // Insert a new key
		else
			i->second++;                             // Increment counter for already inserted key
	}
	cout << "No. of distinct words are: " << word_count.size();
	for (i = word_count.begin(); i != word_count.end(); i++)
		surprise_no += pow(i->second, 2);             // Taking square sum for counters
	cout << "\nSurprise No. (Second moment of Frequency) is: " << surprise_no << "\n";
	return 0;
}