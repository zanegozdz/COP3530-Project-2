#include <iostream>
#include "MusicLibary.h"
#include <string>
#include <iomanip>

using namespace std;

void printResults(vector<Song*> res) {
    cout << "\n========= SEARCH RESULTS =========\n\n";
    if (res.empty()) {
        cout << "No results match your search.\n\n";
        return;
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
    cout << "            Tune Tree          \n";
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
    cout << "5. B+ trees built.\n\n";

    // Main menu loop
    while (true) {
        cout << "============== MENU ==============\n\n";
        cout << "1. Filtered Search" << endl;
        cout << "2. Range Search" << endl;
        cout << "3. Exit" << endl;
        cout << "Pick an option: ";

        string option;
        cin >> option;

        // Validate input - check if first character is a digit
        if (!isdigit(option[0])) {
            cout << "Error: please enter a valid number (1-3)\n\n";
            continue;
        }

        int opt = stoi(option);
        if (opt < 1 || opt > 3) {
            cout << "Error: please enter a valid option (1-3)\n\n";
            continue;
        }

        // Process valid option
        if (opt == 1) {
            cin.ignore();

            vector<string> attributes;
            vector<string> prompts = {
                "Artist (Ex. Jason Derulo): ",
                "Title (Ex. Take You Dancing): ",
                "Emotion (Ex. joy): ",
                "Genre (Ex. pop): ",
                "Release Date (Ex. 2020): ",
                "Tempo (Ex. 113): ",
                "Explicit (Ex. No): ",
                "Popularity (Ex. 72): ",
                "Energy (Ex. 71): ",
                "Danceability (Ex. 79): "
            };

            cout << "\n========= FILTERED SEARCH =========\n";
            cout << "(if the value is none, press enter)\n\n";

            for (const string& prompt : prompts) {
                string input;
                cout << prompt;
                getline(cin, input);
                attributes.push_back(input.empty() ? "-1" : input);
            }

            vector<Song*> results = lib.benchmarkTest(attributes);
            printResults(results);
        }
        else if (opt == 2) {
            cin.ignore(); // Clear buffer before range search

            cout << "\n========== RANGE SEARCH ==========\n";
            cout << "(if the value is none, press enter)\n\n";
            cout << "[1]Artist, [2]Title, [3]Emotion, [4]Genre, [5]Release Date, "
                 << "[6]Tempo, [7]Explicit, [8]Popularity, [9]Energy, [10]Danceability" << endl;
            cout << "Pick an option: ";

            string choice;
            getline(cin, choice);

            // Validate choice input
            if (choice.empty() || !isdigit(choice[0])) {
                cout << "Error: please enter a valid number (1-10)\n\n";
                continue;
            }

            int ch = stoi(choice);
            if (ch < 1 || ch > 10) {
                cout << "Error: please enter a valid option (1-10)\n\n";
                continue;
            }

            string minVal, maxVal;
            cout << "Enter minimum value: ";
            getline(cin, minVal);
            cout << "Enter maximum value: ";
            getline(cin, maxVal);

            vector<Song*> results = lib.rangeSearchBenchmark(ch - 1, minVal, maxVal);
            printResults(results);
        }
        else if (opt == 3) {
            cout << "\nExiting TuneTree. Goodbye!\n";
            return 0;
        }
    }

    return 0;
}