//
// Created by Zmgoz on 10/20/2025.
//

#ifndef PROJECT1_TUNETREE_H
#define PROJECT1_TUNETREE_H

#pragma once
#include <iostream>
#include "Song.h"
#include <unordered_map>
#include "B+Tree.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <unordered_set>

using namespace std;

template<typename K, typename V>
class MusicLibrary {
    unordered_map<string, vector<Song*>> artistTable;
    unordered_map<string, vector<Song*>> titleTable;
    unordered_map<string, vector<Song*>> emotionTable;
    unordered_map<string, vector<Song*>> genreTable;
    unordered_map<int, vector<Song*>> releaseTable;
    unordered_map<int, vector<Song*>> tempoTable;
    unordered_map<string, vector<Song*>> explicitTable;
    unordered_map<int, vector<Song*>> popularityTable;
    unordered_map<int, vector<Song*>> energyTable;
    unordered_map<int, vector<Song*>> danceabilityTable;

    int order = 128;
    BPlusTree<string, Song> artistTree;
    BPlusTree<string, Song> titleTree;
    BPlusTree<string, Song> emotionTree;
    BPlusTree<string, Song> genreTree;
    BPlusTree<int, Song> releaseTree;
    BPlusTree<int, Song> tempoTree;
    BPlusTree<string, Song> explicitTree;
    BPlusTree<int, Song> popularityTree;
    BPlusTree<int, Song> energyTree;
    BPlusTree<int, Song> danceabilityTree;


    vector<Song> songs;
    vector<string> parseCSVLine(const string& line);
    vector<Song*> songsPtrs;
public:
    string filename = "../data/new_dataset.csv";
    void loadData();
    void buildDS();
    // vector<Song*> searchTable(K key);
    vector<Song*> intersectResults(vector<Song*>& a, vector<Song*>& b);
    vector<Song*> searchHashTable(vector<string> attr);
    vector<Song*> searchBPlusTree(vector<string> attr);
    vector<Song*> rangeSearchHashTable(int val, string low, string high);
    vector<Song*> rangeSearchBPlusTree(int val, string low, string high);

    vector<Song*> benchmarkTest(const vector<string>& attr);
    vector<Song*> rangeSearchBenchmark(int attrIndex, string low, string high);
    MusicLibrary(int ord);
    ~MusicLibrary();
    void changeOrder(int ord);

    BPlusTree<string, Song>& getArtistTree() { return artistTree; }
    BPlusTree<int, Song>& getTempoTree() { return tempoTree; }
    BPlusTree<string, Song>& getGenreTree() { return genreTree; }
    BPlusTree<int, Song>& getPopularityTree() { return popularityTree; }
};

template<typename K, typename V>
MusicLibrary<K, V>::MusicLibrary(int ord)
    : artistTree(ord),
      titleTree(ord),
      emotionTree(ord),
      genreTree(ord),
      releaseTree(ord),
      tempoTree(ord),
      explicitTree(ord),
      popularityTree(ord),
      energyTree(ord),
      danceabilityTree(ord) {}
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
        cout << "Error reading file" << filename << "\n";
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
        // artistTree.insert(attr[0], songPtr);
        // titleTree.insert(attr[1], songPtr);
        // emotionTree.insert(attr[2], songPtr);
        // genreTree.insert(attr[3], songPtr);
        // releaseTree.insert(stoi(attr[4]), songPtr);
        // tempoTree.insert(stoi(attr[5]), songPtr);
        // explicitTree.insert(attr[6], songPtr);
        // popularityTree.insert(stoi(attr[7]), songPtr);
        // energyTree.insert(stoi(attr[8]), songPtr);
        // danceabilityTree.insert(stoi(attr[9]), songPtr);
    }
}
template<typename K, typename V>
void MusicLibrary<K, V>::buildDS() {
    for (Song* song : songsPtrs) {
        artistTable[song->artist].push_back(song);
        titleTable[song->songName].push_back(song);
        emotionTable[song->emotion].push_back(song);
        genreTable[song->genre].push_back(song);
        releaseTable[song->releaseDate].push_back(song);
        tempoTable[song->tempo].push_back(song);
        explicitTable[song->explicit_].push_back(song);
        popularityTable[song->popularity].push_back(song);
        energyTable[song->energy].push_back(song);
        danceabilityTable[song->danceability].push_back(song);

        artistTree.insert(song->artist, song);
        titleTree.insert(song->songName, song);
        emotionTree.insert(song->emotion, song);
        genreTree.insert(song->genre, song);
        releaseTree.insert(song->releaseDate, song);
        tempoTree.insert(song->tempo, song);
        explicitTree.insert(song->explicit_, song);
        popularityTree.insert(song->popularity, song);
        energyTree.insert(song->energy, song);
        danceabilityTree.insert(song->danceability,song);

    }
}

template<typename K, typename V>
vector<Song*> MusicLibrary<K, V>::intersectResults(vector<Song*>& a, vector<Song*>& b) {
    vector<Song*> result;
    unordered_set<Song*> aSet(a.begin(), a.end());
    unordered_set<Song*> bSet(b.begin(), b.end());
    for (Song* song : aSet) {
        if (bSet.count(song) > 0) {
            result.push_back(song);
        }
    }
    return result;
}

template<typename K, typename V>
vector<Song*> MusicLibrary<K, V>::searchHashTable(vector<string> attr) {
    vector<vector<Song*>> results;
    for (size_t i = 0; i < attr.size(); i++) {
        if (attr[i] != "-1") {
            if (i == 0) {
                auto it = artistTable.find(attr[i]);
                if (it != artistTable.end()) {
                    results.push_back(it->second);
                }
            }
            else if (i == 1) {
                auto it = titleTable.find(attr[i]);
                if (it != titleTable.end()) {
                    results.push_back(it->second);
                }
            }
            else if (i == 2) {
                auto it = emotionTable.find(attr[i]);
                if (it != emotionTable.end()) {
                    results.push_back(it->second);
                }
            }
            else if (i == 3) {
                auto it = genreTable.find(attr[i]);
                if (it != genreTable.end()) {
                    results.push_back(it->second);
                }
            }
            else if (i == 4) {
                auto it = releaseTable.find(stoi(attr[i]));
                if (it != releaseTable.end()) {
                    results.push_back(it->second);
                }
            }
            else if (i == 5) {
                auto it = tempoTable.find(stoi(attr[i]));
                if (it != tempoTable.end()) {
                    results.push_back(it->second);
                }
            }
            else if (i == 6) {
                auto it = explicitTable.find(attr[i]);
                if (it != explicitTable.end()) {
                    results.push_back(it->second);
                }
            }
            else if (i == 7) {
                auto it = popularityTable.find(stoi(attr[i]));
                if (it != popularityTable.end()) {
                    results.push_back(it->second);
                }
            }
            else if (i == 8) {
                auto it = energyTable.find(stoi(attr[i]));
                if (it != energyTable.end()) {
                    results.push_back(it->second);
                }
            }
            else if (i == 9) {
                auto it = danceabilityTable.find(stoi(attr[i]));
                if (it != danceabilityTable.end()) {
                    results.push_back(it->second);
                }
            }
        }
    }

    if (results.empty()) {
        return {};
    }
    if (results.size() == 1) {
        return results[0];
    }

    vector<Song*> intersection(results[0].begin(), results[0].end());

    for (size_t i = 1; i < results.size(); i++) {
        if (intersection.empty()) {
            break;
        }
        intersection = intersectResults(intersection, results[i]);
    }
    return intersection;
}

template<typename K, typename V>
vector<Song*> MusicLibrary<K, V>::searchBPlusTree(vector<string> attr) {
	vector<vector<Song*>> results;
    for (size_t i = 0; i < attr.size(); i++) {
        if (attr[i] != "-1") {
            if (i == 0) {
                results.push_back(artistTree.search(attr[i]));
            }
            else if (i == 1) {
                results.push_back(titleTree.search(attr[i]));
            }
            else if (i == 2) {
                results.push_back(emotionTree.search(attr[i]));
            }
            else if (i == 3) {
                results.push_back(genreTree.search(attr[i]));
            }
            else if (i == 4) {
                results.push_back(releaseTree.search(stoi(attr[i])));
            }
            else if (i == 5) {
                results.push_back(tempoTree.search(stoi(attr[i])));
            }
            else if (i == 6) {
                results.push_back(explicitTree.search(attr[i]));
            }
            else if (i == 7) {
                results.push_back(popularityTree.search(stoi(attr[i])));
            }
            else if (i == 8) {
                results.push_back(energyTree.search(stoi(attr[i])));
            }
            else if (i == 9) {
                results.push_back(danceabilityTree.search(stoi(attr[i])));
            }
        }
    }
    if (results.empty()) {
        return {};
    }
    if (results.size() == 1) {
        return results[0];
    }

    vector<Song*> intersection(results[0].begin(), results[0].end());

    for (size_t i = 1; i < results.size(); i++) {
        if (intersection.empty()) {
            break;
        }
        intersection = intersectResults(intersection, results[i]);
    }
    return intersection;
}

template<typename K, typename V>
vector<Song*> MusicLibrary<K, V>::rangeSearchHashTable(int val, string low, string high) {
    vector<Song*> result;

    if (val == 0) {
        for (auto& pair : artistTable) {
            if (pair.first >= low && pair.first <= high) {
                for (Song* song : pair.second) {
                    result.push_back(song);
                }
            }
        }
    }
    else if (val == 1) {
        for (auto& pair : titleTable) {
            if (pair.first >= low && pair.first <= high) {
                for (Song* song : pair.second) {
                    result.push_back(song);
                }
            }
        }
    }
    else if (val == 2) {
        for (auto& pair : emotionTable) {
            if (pair.first >= low && pair.first <= high) {
                for (Song* song : pair.second) {
                    result.push_back(song);
                }
            }
        }
    }
    else if (val == 3) {
        for (auto& pair : genreTable) {
            if (pair.first >= low && pair.first <= high) {
                for (Song* song : pair.second) {
                    result.push_back(song);
                }
            }
        }
    }
    else if (val == 4) {
        for (auto& pair : releaseTable) {
            if (pair.first >= stoi(low) && pair.first <= stoi(high)) {
                for (Song* song : pair.second) {
                    result.push_back(song);
                }
            }
        }
    }
    else if (val == 5) {
        for (auto& pair : tempoTable) {
            if (pair.first >= stoi(low) && pair.first <= stoi(high)) {
                for (Song* song : pair.second) {
                    result.push_back(song);
                }
            }
        }
    }
    else if (val == 6) {
        for (auto& pair : explicitTable) {
            if (pair.first >= low && pair.first <= high) {
                for (Song* song : pair.second) {
                    result.push_back(song);
                }
            }
        }
    }
    else if (val == 7) {
        for (auto& pair : popularityTable) {
            if (pair.first >= stoi(low) && pair.first <= stoi(high)) {
                for (Song* song : pair.second) {
                    result.push_back(song);
                }
            }
        }
    }
    else if (val == 8) {
        for (auto& pair : energyTable) {
            if (pair.first >= stoi(low) && pair.first <= stoi(high)) {
                for (Song* song : pair.second) {
                    result.push_back(song);
                }
            }
        }
    }
    else if (val == 9) {
        for (auto& pair : danceabilityTable) {
            if (pair.first >= stoi(low) && pair.first <= stoi(high)) {
                for (Song* song : pair.second) {
                    result.push_back(song);
                }
            }
        }
    }
    return result;
}

template<typename K, typename V>
vector<Song*> MusicLibrary<K, V>::rangeSearchBPlusTree(int val, string low, string high) {
    if (val == 0) {
         return artistTree.rangeSearch(low, high);
    }
    else if (val == 1) {
        return titleTree.rangeSearch(low, high);
    }
    else if (val == 2) {
        return emotionTree.rangeSearch(low, high);
    }
    else if (val == 3) {
        return genreTree.rangeSearch(low, high);
    }
    else if (val == 4) {
        return releaseTree.rangeSearch(stoi(low), stoi(high));
    }
    else if (val == 5) {
        return tempoTree.rangeSearch(stoi(low), stoi(high));
    }
    else if (val == 6) {
        return explicitTree.rangeSearch(low, high);
    }
    else if (val == 7) {
        return popularityTree.rangeSearch(stoi(low), stoi(high));
    }
    else if (val == 8) {
        return energyTree.rangeSearch(stoi(low), stoi(high));
    }
    else if (val == 9) {
        return danceabilityTree.rangeSearch(stoi(low), stoi(high));
    }
    return {};
}

template<typename K, typename V>
vector<Song*> MusicLibrary<K, V>::benchmarkTest(const vector<string>& attr) {
    const int total_runs = 100;

    cout << "\n========= BENCHMARK TEST =========\n" << endl;

    auto startHash = chrono::high_resolution_clock::now();
    vector<Song*> resultsHash;
    for (int i = 0; i < total_runs; i++) {
        resultsHash = searchHashTable(attr);
    }
    auto endHash = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::nanoseconds>(endHash - startHash);
    long long avgHash = durationHash.count() / static_cast<long long>(total_runs);

    auto startTree = chrono::high_resolution_clock::now();
    vector<Song*> resultsTree;
    for (int i = 0; i < total_runs; i++) {
        resultsTree = searchBPlusTree(attr);
    }
    auto endTree = chrono::high_resolution_clock::now();
    auto durationTree = chrono::duration_cast<chrono::nanoseconds>(endTree - startTree);
    long long avgTree = durationTree.count() / static_cast<long long>(total_runs);

    cout << "HASH TABLE:" << endl;
    cout << "\tTotal time: " << durationHash.count() << " ns" << endl;
    cout << "\tAverage time: " << avgHash << " ns" << endl;
    cout << "\tResults found: " << resultsHash.size() << endl;

    cout << "\nB+ TREE:" << endl;
    cout << "\tTotal time: " << durationTree.count() << " ns" << endl;
    cout << "\tAverage time: " << avgHash << " ns" << endl;
    cout << "\tResults found: " << resultsTree.size() << endl;

    if (resultsHash.size() != resultsTree.size()) {
        cout << "\nWARNING: Results do not match!" << endl;
    }

    if (avgHash > avgTree) {
        double speedup = durationHash / durationTree;
        cout << "B+ Tree is " << fixed << setprecision(5) << speedup << "x FASTER" << endl;
        return resultsTree;
    }
    else {
        double speedup = durationTree / durationHash;
        cout << "Hash Table is " << fixed << setprecision(5) << speedup << "x FASTER" << endl;
        return resultsHash;
    }


}

template<typename K, typename V>
vector<Song*> MusicLibrary<K, V>::rangeSearchBenchmark(int attrIndex, string low, string high) {
    const int total_runs = 100;

    cout << "\n========= BENCHMARK TEST =========\n" << endl;

    auto startHash = chrono::high_resolution_clock::now();
    vector<Song*> resultsHash;
    for (int i = 0; i < total_runs; i++) {
        resultsHash = rangeSearchHashTable(attrIndex, low, high);
    }
    auto endHash = chrono::high_resolution_clock::now();
    auto durationHash = chrono::duration_cast<chrono::nanoseconds>(endHash - startHash);
    long long avgHash = durationHash.count() / static_cast<long long>(total_runs);

    auto startTree = chrono::high_resolution_clock::now();
    vector<Song*> resultsTree;
    for (int i = 0; i < total_runs; i++) {
        resultsTree = rangeSearchBPlusTree(attrIndex, low, high);
    }
    auto endTree = chrono::high_resolution_clock::now();
    auto durationTree = chrono::duration_cast<chrono::nanoseconds>(endTree - startTree);
    long long avgTree = durationTree.count() / static_cast<long long>(total_runs);

    cout << "HASH TABLE:" << endl;
    cout << "\tTotal time: " << durationHash.count() << " ns" << endl;
    cout << "\tAverage time: " << avgHash << " ns" << endl;
    cout << "\tResults found: " << resultsHash.size() << endl;

    cout << "\nB+ TREE:" << endl;
    cout << "\tTotal time: " << durationTree.count() << " ns" << endl;
    cout << "\tAverage time: " << avgHash << " ns" << endl;
    cout << "\tResults found: " << resultsTree.size() << endl;

    if (resultsHash.size() != resultsTree.size()) {
        cout << "\nWARNING: Results do not match!" << endl;
    }

    if (avgHash > avgTree) {
        double speedup = durationHash / durationTree;
        cout << "B+ Tree is " << fixed << setprecision(5) << speedup << "x FASTER" << endl;
        return resultsTree;
    }
    else {
        double speedup = durationTree / durationHash;
        cout << "Hash Table is " << fixed << setprecision(5) << speedup << "x FASTER" << endl;
        return resultsHash;
    }
}

template<typename K, typename V>
MusicLibrary<K, V>::~MusicLibrary() {
    for (Song* songPtr : songsPtrs) {
        delete songPtr;
    }
}


#endif //PROJECT1_TUNETREE_H
