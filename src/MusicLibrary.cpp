// //
// // Created by Zmgoz on 10/20/2025.
// //
// #include <chrono>
//
// #include "MusicLibary.h"
// #include <fstream>
// #include <iomanip>
// #include <string>
// #include <sstream>
// #include <unordered_set>
// using namespace std;
// template<typename K, typename V>
// MusicLibrary<K, V>::MusicLibrary(int ord)
//     : artistTree(ord),
//       titleTree(ord),
//       emotionTree(ord),
//       genreTree(ord),
//       releaseTree(ord),
//       tempoTree(ord),
//       explicitTree(ord),
//       popularityTree(ord),
//       energyTree(ord),
//       danceabilityTree(ord)
// {}
// template<typename K, typename V>
// void MusicLibrary<K, V>::changeOrder(int ord) {
//     this->order = ord;
// }
// template<typename K, typename V>
// vector<string> MusicLibrary<K, V>::parseCSVLine(const string& line) {
//     vector<string> result;
//     string field;
//     bool inQuotes = false;
//
//     for (char c : line) {
//         if (c == '"') {
//             inQuotes = !inQuotes;
//         } else if (c == ',' && !inQuotes) {
//             result.push_back(field);
//             field.clear();
//         } else {
//             field += c;
//         }
//     }
//     result.push_back(field);
//     return result;
// }
// template<typename K, typename V>
// void MusicLibrary<K, V>::loadData() {
//     ifstream file(filename);
//     if (!file.is_open()) {
//         cout << "Error reading file" << filename << "\n";
//         return;
//     }
//
//     string header;
//     getline(file, header); // skip header line
//
//     string line;
//     while (getline(file, line)) {
//         vector<string> attr;
//         stringstream ss(line);
//         string attribute;
//
//         attr = parseCSVLine(line);
//
//         if (attr.size() < 10) continue;
//
//         Song* songPtr = new Song(attr[0], attr[1], attr[2], attr[3],
//                          stoi(attr[4]), stoi(attr[5]), attr[6],
//                          stoi(attr[7]), stoi(attr[8]), stoi(attr[9]));
//
//         // store pointer in a separate vector for lifetime management
//         songsPtrs.push_back(songPtr);
//
//         // insert into B+ trees
//         // artistTree.insert(attr[0], songPtr);
//         // titleTree.insert(attr[1], songPtr);
//         // emotionTree.insert(attr[2], songPtr);
//         // genreTree.insert(attr[3], songPtr);
//         // releaseTree.insert(stoi(attr[4]), songPtr);
//         // tempoTree.insert(stoi(attr[5]), songPtr);
//         // explicitTree.insert(attr[6], songPtr);
//         // popularityTree.insert(stoi(attr[7]), songPtr);
//         // energyTree.insert(stoi(attr[8]), songPtr);
//         // danceabilityTree.insert(stoi(attr[9]), songPtr);
//     }
// }
// template<typename K, typename V>
// void MusicLibrary<K, V>::buildDS() {
//     for (Song* song : songsPtrs) {
//         artistTable[song->artist].push_back(song);
//         titleTable[song->songName].push_back(song);
//         emotionTable[song->emotion].push_back(song);
//         genreTable[song->genre].push_back(song);
//         releaseTable[song->releaseDate].push_back(song);
//         tempoTable[song->tempo].push_back(song);
//         explicitTable[song->explicit_].push_back(song);
//         popularityTable[song->popularity].push_back(song);
//         energyTable[song->energy].push_back(song);
//         danceabilityTable[song->danceability].push_back(song);
//
//         artistTree.insert(song->artist, song);
//         titleTree.insert(song->songName, song);
//         emotionTree.insert(song->emotion, song);
//         genreTree.insert(song->genre, song);
//         releaseTree.insert(song->releaseDate, song);
//         tempoTree.insert(song->tempo, song);
//         explicitTree.insert(song->explicit_, song);
//         popularityTree.insert(song->popularity, song);
//         energyTree.insert(song->energy, song);
//         danceabilityTree.insert(song->danceability,song);
//
//     }
// }
//
// template<typename K, typename V>
// vector<Song *> MusicLibrary<K, V>::intersectResults(vector<Song *> &a, vector<Song *> &b) {
// }
//
// vector<Song*> intersectResults(vector<Song*>& a, vector<Song*>& b) {
//     vector<Song*> result;
//     unordered_set<Song*> aSet(a.begin(), a.end());
//     unordered_set<Song*> bSet(b.begin(), b.end());
//     for (Song* song : aSet) {
//         if (bSet.count(song) > 0) {
//             result.push_back(song);
//         }
//     }
//     return result;
// }
//
// template<typename K, typename V>
// vector<Song*> MusicLibrary<K, V>::searchHashTable(vector<string> attr) {
//     vector<vector<Song*>> results;
//     for (int i = 0; i < attr.size(); i++) {
//         if (attr[i] == "-1") {
//             auto it = tables[i].find(attr[i]);
//             if (it != tables[i].end()) {
//                 results.push_back(it->second);
//             }
//         }
//     }
//
//     if (results.empty()) {
//         return {};
//     }
//     if (results.size() == 1) {
//         return results[0];
//     }
//
//     vector<Song*> intersection = results[0];
//
//     for (int i = 1; i < results.size(); i++) {
//         if (intersection.empty()) {
//             break;
//         }
//         intersection = intersectResults(intersection, results[i]);
//     }
//     return intersection;
// }
//
// template<typename K, typename V>
// vector<Song*> MusicLibrary<K, V>::searchBPlusTree(vector<string> attr) {
// 	vector<vector<Song*>> results;
//     for (int i = 0; i < attr.size(); i++) {
//         if (attr[i] != "-1") {
// 			results.push_back(trees[i].search(attr[i]));
//         }
//     }
//
//     if (results.empty()) {
//         return {};
//     }
//     if (results.size() == 1) {
//         return results[0];
//     }
//
//     vector<Song*> intersection = results[0];
//
//     for (int i = 1; i < results.size(); i++) {
//         if (intersection.empty()) {
//             break;
//         }
//         intersection = intersectResults(intersection, results[i]);
//     }
//     return intersection;
// }
//
// template<typename K, typename V>
// void MusicLibrary<K, V>::benchmarkTest(const vector<string>& attr) {
//     const int total_runs = 100;
//
//     cout << "\nBENCHMARK TEST" << endl;
//
//     auto startHash = chrono::high_resolution_clock::now();
//     vector<Song*> resultsHash;
//     for (int i = 0; i < total_runs; i++) {
//         resultsHash = searchHashTable(attr);
//     }
//     auto endHash = chrono::high_resolution_clock::now();
//     auto durationHash = chrono::duration_cast<chrono::milliseconds>(endHash - startHash);
//     long long avgHash = durationHash.count() / static_cast<long long>(total_runs);
//
//     auto startTree = chrono::high_resolution_clock::now();
//     vector<Song*> resultsTree;
//     for (int i = 0; i < total_runs; i++) {
//         resultsTree = searchBPlusTree(attr);
//     }
//     auto endTree = chrono::high_resolution_clock::now();
//     auto durationTree = chrono::duration_cast<chrono::milliseconds>(endTree - startTree);
//     long long avgTree = durationTree.count() / static_cast<long long>(total_runs);
//
//     cout << "HASH TABLE:" << endl;
//     cout << "\tTotal time: " << durationHash.count() << " ms" << endl;
//     cout << "\tAverage time: " << avgHash << " ms" << endl;
//     cout << "\tResults found: " << resultsHash.size() << endl;
//
//     cout << "\nB+ TREE:" << endl;
//     cout << "\tTotal time: " << durationTree.count() << " ms" << endl;
//     cout << "\tAverage time: " << avgHash << " ms" << endl;
//     cout << "\tResults found: " << resultsTree.size() << endl;
//
//     if (avgHash > avgTree) {
//         long long speedup = avgHash / avgTree;
//         cout << "B+ Tree is " << fixed << setprecision(2) << speedup << "x FASTER" << endl;
//     }
//     else {
//         long long speedup = avgTree / avgHash;
//         cout << "Hash Table is " << fixed << setprecision(2) << speedup << "x FASTER" << endl;
//     }
//
//     if (resultsHash.size() != resultsTree.size()) {
//         cout << "\nWARNING: Results do not match!" << endl;
//     }
// }
// template<typename K, typename V>
// MusicLibrary<K, V>::~MusicLibrary() {
//     for (Song* songPtr : songsPtrs) {
//         delete songPtr;
//     }
// }
//
