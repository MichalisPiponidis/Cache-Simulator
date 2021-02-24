#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

int blocks, type, ways, cache_lines, i, index, misses = 0, hits = 0, j,found,policy,min,found_index,min_index,bits,rrpv;
float miss_rate, hit_rate;
ifstream fp;
ofstream output;
unsigned long long int address;
unsigned long long int** cache;
unsigned long long int** replace;
vector<unsigned long long int>input;
vector<unsigned long long int>unique_addresses;

void check_unique_address(unsigned long long int addr) {
	if (std::find(unique_addresses.begin(), unique_addresses.end(), addr) != unique_addresses.end()) { // If the address exists in the unique addresses
				// unique_addresses contains address
	}
	else { //if it doesn't exist, add it
		unique_addresses.push_back(addr);
	}
}

void export_output() {
	miss_rate = ((float)((float)misses / (float)input.size()) * 100);
	hit_rate = ((float)((float)hits / (float)input.size()) * 100);
	output.open("output_file.txt", ios::out);
	output << "Cache_accesses = " << input.size() << "\nTotal_number_of_unique_addresses = " << unique_addresses.size() << "\nTotal_number_of_misses = " << misses << "\nTotal_number_of_hits = " << hits << "\nMiss_rate = " << miss_rate << "\nHit_rate = " << hit_rate << "\n\n--------Cache Contents---------\n";
	for (i = 0; i < cache_lines; i++) {
		output << "\n# " << i;
		for (j = 0; j < ways; j++)
			output << " " << cache[i][j];
		output << " ";
	}
	output << "\n";
	output.close();
}

int main() {
	fp.open("D_trace.txt", ios::in);
	if (!(fp.is_open())) {
		cout << "File open failed." << endl;
		return 0;
	}
	//Read addresses from file
	while (!fp.eof()) {
		fp >> address;
		input.push_back(address);
	}
	fp.close();
	//Read Parameters
	cout << "Insert total cache blocks: ";
	cin >> blocks;
	cout << "\nInsert cache type:\n\n1. Direct-Mapped\n2. Set-Associative\n3. Fully Associative\n\n";
	cin >> type;
	if (type == 1) { //Direct Mapped
		ways = 1;
		cache_lines = blocks;
		cache = new unsigned long long int* [cache_lines];
		for (i = 0; i < cache_lines; i++) { // cache initialization
			cache[i] = new unsigned long long int[1];
			cache[i][0] = 0;
		}
		for (i = 0; i < input.size(); i++) { //add addresses to cache
			index = input[i] % cache_lines;
			check_unique_address(input[i]);
			if (cache[index][0] != input[i]) { //Miss
				misses++;
				cache[index][0] = input[i];
			}
			else { //Hit
				hits++;
			}
		}
		export_output();
	}
	else if (type == 2) { //Set Associative
		cout << "\nInsert number of ways: ";
		cin >> ways;
		cout << "\n\nInsert replace policy:\n1. Least-Recently Used (LRU)\n2. Static Re-Reference Interval Prediction (SRRIP)\n\n";
		cin >> policy;
		if (policy == 2) {
			cout << "\n\nInsert bits for SRRIP: ";
			cin >> bits;
			rrpv = pow(2, bits);
		}
		cache_lines = blocks / ways;
		cache = new unsigned long long int* [cache_lines];
		replace = new unsigned long long int* [cache_lines];
		for (i = 0; i < cache_lines; i++) { // cache initialization
			cache[i] = new unsigned long long int[ways];
			replace[i] = new unsigned long long int[ways];
			for (j = 0; j < ways; j++) {
				cache[i][j] = 0;
				if (policy == 1) //for LRU
					replace[i][j] = 0;
				else //for SRRIP
					replace[i][j] = rrpv - 1;
			}
		}
		for (i = 0; i < input.size(); i++) { //add addresses to cache
			index = input[i] % cache_lines;
			found = 0;
			check_unique_address(input[i]);
			for (j = 0; j < ways; j++)
				if (cache[index][j] == input[i]) { //check if address is in cache
					found = 1;
					found_index = j;
					break;
				}
			if (found == 0) { //Miss
				misses++;
				min = INT_MAX;
				if (policy == 1) { //LRU
					for (j = 0; j < ways; j++)
						if (replace[index][j] < min) {
							min = replace[index][j];
							min_index = j;
						}
					replace[index][min_index] = i + 1;
				}
				else { //SRRIP
					while (found == 0) {
						for (j = 0; j < ways; j++) {
							if (replace[index][j] == rrpv - 1) {//If found
								found = 1;
								min_index = j;
								break;
							}
						}
						if (found == 0)
							for (j = 0; j < ways; j++) //If not found, increment all RRPVs by 1
								replace[index][j]++;
					}
					replace[index][min_index] = rrpv - 2; //Set RRPV of block to 2^n - 2
				}
				cache[index][min_index] = input[i];
			}
			else { //Hit
				hits++;
				if (policy == 1) //LRU
					replace[index][found_index] = i + 1;
				else { //SRRIP
					replace[index][found_index] = 0;
				}
			}
		}
		export_output();
	}
	else if (type == 3) { //Fully Associative
		ways = blocks;
		cout << "\n\nInsert replace policy:\n1. Least-Recently Used (LRU)\n2. Static Re-Reference Interval Prediction (SRRIP)\n\n";
		cin >> policy;
		if (policy == 2) {
			cout << "\n\nInsert bits for SRRIP: ";
			cin >> bits;
			rrpv = pow(2, bits);
		}
		cache_lines = 1;
		cache = new unsigned long long int* [1];
		cache[0] = new unsigned long long int[ways];
		replace = new unsigned long long int* [1];
		replace[0] = new unsigned long long int[ways];
		for (i = 0; i < ways; i++) { // cache initialization
			cache[0][i] = 0;
			if (policy==1) //For LRU
				replace[0][i] = 0;
			else //For SRRIP
				replace[0][i] = rrpv - 1;
		}
		for (i = 0; i < input.size(); i++) { //add addresses to cache
			found = 0;
			check_unique_address(input[i]);
			for (j=0;j<ways;j++)
				if (cache[0][j] == input[i]) { //check if address is in cache
					found = 1;
					found_index = j;
					break;
				}
			if (found==0) { //Miss
				misses++;
				min = INT_MAX;
				if (policy == 1) { //LRU
					for (j = 0; j < ways; j++)
						if (replace[0][j] < min) {
							min = replace[0][j];
							index = j;
						}
					replace[0][index] = i+1;
				}
				else { //SRRIP
					while (found == 0) {
						for (j = 0; j < ways; j++) {
							if (replace[0][j] == rrpv - 1) {//If found
								found = 1;
								index = j;
								break;
							}
						}
						if (found == 0)
							for (j = 0; j < ways; j++) //If not found, increment all RRPVs by 1
								replace[0][j]++;
					}
					replace[0][index] = rrpv - 2; //Set RRPV of block to 2^n - 2
				}
				cache[0][index] = input[i];
			}
			else { //Hit
				hits++;
				if (policy == 1) //LRU
					replace[0][found_index] = i + 1;
				else { //SRRIP
					replace[0][found_index] = 0;
				}
			}
		}
		export_output();
	}
}