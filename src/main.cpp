#include <iostream>
#include <B+Tree.h>
#include <MusicLibary.h>

using namespace std;

int main(){
	cout << "==================================" << endl;
	cout << "   Music Library - ABBA Search   " << endl;
	cout << "==================================" << endl;

	// Create library and load data
	cout << "\n1. Creating music library..." << endl;
	MusicLibrary lib;

	cout << "2. Loading data from CSV..." << endl;
	lib.loadData();

	cout << "3. Data loaded successfully!\n" << endl;

	// Search for all ABBA songs
	cout << "==================================" << endl;
	cout << "Searching for ABBA songs..." << endl;
	cout << "==================================" << endl;

	auto abbaSongs = lib.getArtistTree().search("ABBA");

	cout << "\nFound " << abbaSongs.size() << " ABBA songs:\n" << endl;

	// Print each song
	for (size_t i = 0; i < abbaSongs.size(); i++) {
		cout << "Song #" << (i + 1) << ":" << endl;
		cout << "-----------------------------------" << endl;
		if (abbaSongs[i] != nullptr) {
			abbaSongs[i]->print();
		} else {
			cout << "ERROR: Null pointer!" << endl;
		}
		cout << endl;
	}

	cout << "==================================" << endl;
	cout << "Search complete!" << endl;
	cout << "==================================" << endl;

	return 0;
}