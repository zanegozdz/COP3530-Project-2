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

using namespace std;

class MusicLibrary {
    unordered_map<string, vector<Song>> artistTable;
    unordered_map<string, vector<Song>> titleTable;
    unordered_map<string, vector<Song>> emotionTable;
    unordered_map<string, vector<Song>> genreTable;
    unordered_map<int, vector<Song>> releaseTable;
    unordered_map<int, vector<Song>> tempoTable;
    unordered_map<string, vector<Song>> explicitTable;
    unordered_map<int, vector<Song>> popularityTable;
    unordered_map<int, vector<Song>> energyTable;
    unordered_map<int, vector<Song>> danceabilityTable;

    int order = 32;
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

public:
    string filename = "data/new_dataset.csv";
    void loadData();
    void buildDS();
    vector<Song> searchHashTable(vector<string> attr);
    vector<Song> searchBPlusTree(vector<string> attr);
    void benchmarkTest(vector<string> attr);
    MusicLibrary();
    ~MusicLibrary();
    void changeOrder(int ord);
};




#endif //PROJECT1_TUNETREE_H
