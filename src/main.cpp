#include <iostream>
#include "MusicLibary.h"
#include <string>
#include <iomanip>

using namespace std;

void printResults(vector<Song*> res) {
	cout << "\n========= SEARCH RESULTS =========\n\n";
	if (res.empty()) {
		cout << "No results match your search.\n\n";
	}

	const int artistWidth = 20;
	const int titleWidth = 20;
	const int emotionWidth = 10;
	const int genreWidth = 12;
	const int releaseWidth = 8;
	const int tempoWidth = 7;
	const int explicitWidth = 9;
	const int popWidth = 13;
	const int energyWidth = 8;
	const int danceWidth = 12;

	cout << string(artistWidth + titleWidth + emotionWidth + genreWidth +
				   releaseWidth + tempoWidth + explicitWidth + popWidth +
				   energyWidth + danceWidth, '-') << endl;

	cout << left
	<< setw(artistWidth) << "ARTIST"
	<< setw(titleWidth) << "TITLE"
	<< setw(emotionWidth) << "EMOTION"
	<< setw(genreWidth) << "GENRE"
	<< setw(releaseWidth) << "RELEASE"
	<< setw(tempoWidth) << "TEMPO"
	<< setw(explicitWidth) << "EXPLICIT"
	<< setw(popWidth) << "POPULARITY"
	<< setw(energyWidth) << "ENERGY"
	<< setw(danceWidth) << "DANCEABILITY"
	<< endl;

	cout << string(artistWidth + titleWidth + emotionWidth + genreWidth +
				   releaseWidth + tempoWidth + explicitWidth + popWidth +
				   energyWidth + danceWidth, '-') << endl;

	for (Song* song : res) {
		if (!song) {
			continue;
		}
		string artist = song->artist;
		if (artist.length() > artistWidth - 2) {
			artist = song->artist.substr(0, artistWidth - 3) + "..";
		}
		string title = song->songName;
		if (title.length() > titleWidth - 2) {
			title = song->songName.substr(0, titleWidth - 3) + "..";
		}
		string genre = song->genre;
		if (genre.length() > genreWidth - 2) {
			genre = song->genre.substr(0, genreWidth - 3) + "..";
		}

		cout << left
		<< setw(artistWidth) << artist
		<< setw(titleWidth) << title
		<< setw(emotionWidth) << song->emotion
		<< setw(genreWidth) << genre
		<< setw(releaseWidth) << song->releaseDate
		<< setw(tempoWidth) << song->tempo
		<< setw(explicitWidth) << song->explicit_
		<< setw(popWidth) << song->popularity
		<< setw(energyWidth) << song->energy
		<< setw(danceWidth) << song->danceability
		<< endl;
	}
	cout << string(artistWidth + titleWidth + emotionWidth + genreWidth +
				   releaseWidth + tempoWidth + explicitWidth + popWidth +
				   energyWidth + danceWidth, '-') << endl << endl;
	cout << res.size() << " results found\n\n";
}

int main() {
	cout << "==================================\n";
	cout << "           Music Library          \n";
	cout << "==================================\n\n";

	// 1. Create library
	cout << "1. Creating music library...\n";
	MusicLibrary<string, Song*> lib(128);

	// 2. Load CSV data
	cout << "2. Loading data from CSV...\n";
	lib.loadData();
	cout << "3. Data loaded successfully!\n";

	// 3. Build B+ tree structures
	cout << "4. Building B+ trees...\n";
	lib.buildDS();
	cout << "B+ trees built.\n\n";

	cout << "============== MENU ==============\n\n";
	cout << "1. Filtered Search" << endl;
	cout << "2. Range Search" << endl;
	cout << "3. Exit" << endl;
	string option;
	cin >> option;

	while (!isdigit(option[0])) {
		cout << "Error: please enter a valid number" << endl;
	}
	if (stoi(option) < 1 || stoi(option) > 3) {
		cout << "Error: please enter a valid option (1-3)" << endl;
	}
	while (stoi(option) >= 1 && stoi(option) <= 3) {
		int opt = stoi(option);
		if (opt == 1) {
			cin.ignore();

			vector<string> attributes;
			vector<string> prompts = {
				"Artist: ",
				"Title: ",
				"Emotion: ",
				"Genre: ",
				"Release Date: ",
				"Tempo: ",
				"Explicit (Yes/No): ",
				"Popularity: ",
				"Energy: ",
				"Danceability: "
			};

			cout << "\n========= FILTERED SEARCH =========\n";
			cout << "(if the value is none, click enter)\n\n";

			for (string& prompt : prompts) {
				string input;
				cout << prompt;
				getline(cin, input);
				attributes.push_back(input.empty() ? "-1" : input);
			}

			vector<Song*> results = lib.benchmarkTest(attributes);

			printResults(results);
		}
		else if (opt == 2) {
			cout << "\n========== RANGE SEARCH ==========\n";
			cout << "(if the value is none, click enter)\n\n";
			cout << "Pick an option:" << endl;
			cout << "[1]Artist, [2]Title, [3]Emotion, [4]Genre, [5]Release Date, [6]Tempo, [7]Explicit, [8]Popularity, [9]Energy, [10]Danceability" << endl;
			string choice;
			cin >> choice;
			int ch = stoi(choice);
			while (!isdigit(option[0])) {
				cout << "Error: please enter a valid number" << endl;
			}
			if (ch < 1 || ch > 10) {
				cout << "Error: please enter a valid option (1-10)" << endl;
			}

			string maxVal;
			string minVal;
			cout << "Enter maximum value: ";
			cin >> maxVal;
			cout << "Enter minimum value: ";
			cin >> minVal;
			vector<Song*> results = lib.rangeSearchBenchmark(ch-1, maxVal, minVal);
			printResults(results);
		}
		else if (opt == 3) {
			return 1;
		}
		cout << "============== MENU ==============\n\n";
		cout << "1. Filtered Search" << endl;
		cout << "2. Range Search" << endl;
		cout << "3. Exit" << endl;
		cin >> option;

		while (!isdigit(option[0])) {
			cout << "Error: please enter a valid number" << endl;
			cin >> option;
		}
		if (stoi(option) < 1 || stoi(option) > 3) {
			cout << "Error: please enter a valid option (1-3)" << endl;
			cin >> option;
		}
	}

	cout << "5. Starting benchmark test..." << endl;

	// // Optional: Print the Artist B+ tree structure
	// cout << "Artist B+ Tree Structure:\n";
	// lib.getArtistTree().print();
	// cout << endl;

	// // 4. Search for ABBA songs
	// cout << "Searching for ABBA songs:\n";
	// auto results = lib.getArtistTree().search("ABBA");

	// if (results.empty()) {
	// 	cout << "No songs found for ABBA.\n";
	// } else {
	// 	int idx = 1;
	// 	for (auto* song : results) {
	// 		cout << "Song #" << idx++ << ":\n";
	// 		cout << "-----------------------------------\n";
	// 		song->print();
	// 		cout << "-----------------------------------\n";
	// 	}
	// }

	// cout << "\n==================================\n";
	// cout << "Search complete!\n";
	// cout << "==================================\n";
	//
	// cout << "Songs with tempo between 100 and 140:\n";
	//
	// // Perform range search
	// auto tempoRange = lib.getTempoTree().rangeSearch(100, 101);
	//
	// if (tempoRange.empty()) {
	// 	cout << "No songs found in this tempo range.\n";
	// } else {
	// 	int idx = 1;
	// 	for (auto* song : tempoRange) {
	// 		cout << "Song #" << idx++ << ":\n";
	// 		cout << "-----------------------------------\n";
	// 		song->print();
	// 		cout << "-----------------------------------\n";
	// 	}
	// }

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