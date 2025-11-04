//
// Created by Zmgoz on 10/20/2025.
//
#include "MusicLibary.h"
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

MusicLibrary::MusicLibrary()
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

void MusicLibrary::changeOrder(int ord) {
    this->order = ord;
}

void MusicLibrary::loadData() {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Can't open file" << filename << "\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> attr;
        stringstream ss(line);
        string attribute;

        while (getline(ss, attribute, ',')) {
            attr.push_back(attribute);
        }

        if (attr.size() < 10) continue;

        Song song(attr[0], attr[1], attr[2], attr[3],
            stoi(attr[4]), stoi(attr[5]), attr[6], stoi(attr[7]),
            stoi(attr[8]), stoi(attr[9]));

        songs.push_back(song);
    }
}

void MusicLibrary::buildDS() {
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
