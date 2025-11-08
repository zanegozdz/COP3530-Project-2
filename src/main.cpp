#include <iostream>
#include "MusicLibary.h"

using namespace std;

int main() {
	cout << "==================================\n";
	cout << "   Music Library - ABBA Search   \n";
	cout << "==================================\n\n";

	// 1. Create library
	cout << "1. Creating music library...\n";
	MusicLibrary lib;

	// 2. Load CSV data
	cout << "2. Loading data from CSV...\n";
	lib.loadData();
	cout << "3. Data loaded successfully!\n\n";

	// 3. Build B+ tree structures
	cout << "4. Building B+ trees...\n";
	lib.buildDS();
	cout << "B+ trees built.\n\n";

	// Optional: Print the Artist B+ tree structure
	cout << "Artist B+ Tree Structure:\n";
	lib.getArtistTree().print();
	cout << endl;

	// 4. Search for ABBA songs
	cout << "Searching for ABBA songs:\n";
	auto results = lib.getArtistTree().search("ABBA");

	if (results.empty()) {
		cout << "No songs found for ABBA.\n";
	} else {
		int idx = 1;
		for (auto* song : results) {
			cout << "Song #" << idx++ << ":\n";
			cout << "-----------------------------------\n";
			song->print();
			cout << "-----------------------------------\n";
		}
	}

	cout << "\n==================================\n";
	cout << "Search complete!\n";
	cout << "==================================\n";

	cout << "Songs with tempo between 100 and 140:\n";

	// Perform range search
	auto tempoRange = lib.getTempoTree().rangeSearch(100, 101);

	if (tempoRange.empty()) {
		cout << "No songs found in this tempo range.\n";
	} else {
		int idx = 1;
		for (auto* song : tempoRange) {
			cout << "Song #" << idx++ << ":\n";
			cout << "-----------------------------------\n";
			song->print();
			cout << "-----------------------------------\n";
		}
	}

	auto artistRange = lib.getArtistTree().rangeSearch("ABBA", "Queen");

	if (artistRange.empty()) {
		cout << "No songs found for artists in this range.\n";
	} else {
		int idx = 1;
		for (auto* song : artistRange) {
			cout << "Song #" << idx++ << ":\n";
			cout << "-----------------------------------\n";
			song->print();
			cout << "-----------------------------------\n";
		}
	}

	lib.getArtistTree().print();


	return 0;
}