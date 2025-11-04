//
// Created by Zmgoz on 10/20/2025.
//
#include "MusicLibary.h"
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
template<typename K, typename V>
MusicLibrary<K, V>::MusicLibrary()
    : artistTree(order),
      titleTree(order),
      emotionTree(order),
      genreTree(order),
      releaseTree(order),
      tempoTree(order),
      explicitTree(order),
      popularityTree(order),
      energyTree(order),
      danceabilityTree(order)
{}
template<typename K, typename V>
void MusicLibrary<K, V>::changeOrder(int ord) {
    this->order = ord;
}
template<typename K, typename V>
vector<string> MusicLibrary<K, V>::parseCSVLine(const string& line) {
    vector<string> result;
    string field;
    bool inQuotes = false;

    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            result.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    result.push_back(field);
    return result;
}
template<typename K, typename V>
void MusicLibrary<K, V>::loadData() {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Can't open file" << filename << "\n";
        return;
    }

    string header;
    getline(file, header); // skip header line

    string line;
    while (getline(file, line)) {
        vector<string> attr;
        stringstream ss(line);
        string attribute;

        attr = parseCSVLine(line);

        if (attr.size() < 10) continue;

        Song* songPtr = new Song(attr[0], attr[1], attr[2], attr[3],
                         stoi(attr[4]), stoi(attr[5]), attr[6],
                         stoi(attr[7]), stoi(attr[8]), stoi(attr[9]));

        // store pointer in a separate vector for lifetime management
        songsPtrs.push_back(songPtr);

        // insert into B+ trees
        artistTree.insert(attr[0], songPtr);
        titleTree.insert(attr[1], songPtr);
        emotionTree.insert(attr[2], songPtr);
        genreTree.insert(attr[3], songPtr);
        releaseTree.insert(stoi(attr[4]), songPtr);
        tempoTree.insert(stoi(attr[5]), songPtr);
        explicitTree.insert(attr[6], songPtr);
        popularityTree.insert(stoi(attr[7]), songPtr);
        energyTree.insert(stoi(attr[8]), songPtr);
        danceabilityTree.insert(stoi(attr[9]), songPtr);
    }
}
template<typename K, typename V>
void MusicLibrary<K, V>::buildDS() {
    for (Song& song : songs) {
        artistTable[song.artist].push_back(song);
        titleTable[song.songName].push_back(song);
        emotionTable[song.emotion].push_back(song);
        genreTable[song.genre].push_back(song);
        releaseTable[song.releaseDate].push_back(song);
        tempoTable[song.tempo].push_back(song);
        explicitTable[song.explicit_].push_back(song);
        popularityTable[song.popularity].push_back(song);
        energyTable[song.energy].push_back(song);
        danceabilityTable[song.danceability].push_back(song);

        artistTree.insert(song.artist, &song);
        titleTree.insert(song.songName, &song);
        emotionTree.insert(song.emotion, &song);
        genreTree.insert(song.genre, &song);
        releaseTree.insert(song.releaseDate, &song);
        tempoTree.insert(song.tempo, &song);
        explicitTree.insert(song.explicit_, &song);
        popularityTree.insert(song.popularity, &song);
        energyTree.insert(song.energy, &song);
        danceabilityTree.insert(song.danceability, &song);

    }
}
template<typename K, typename V>
vector<Song> MusicLibrary<K, V>::searchHashTable(vector<string> attr) {

}
template<typename K, typename V>
vector<Song> MusicLibrary<K, V>::searchBPlusTree(vector<string> attr) {
	vector<vector<Song*>> results;
    for (int i = 0; i < attr.size(); i++) {
        if (attr[i] != "-1") {
			results.push_back(trees[i].search(
        }
    }
}
template<typename K, typename V>
void MusicLibrary<K, V>::benchmarkTest(vector<string> attr) {
}
template<typename K, typename V>
MusicLibrary<K, V>::~MusicLibrary() {
    for (Song* songPtr : songsPtrs) {
        delete songPtr;
    }
}
