//
// Created by Raymond Adams on 11/2/25.
//

#ifndef PROJECT2_SONG_H
#define PROJECT2_SONG_H

#include <string>
#include <iostream>

using namespace std;

struct Song {
    string artist;
    string songName;
    string emotion;
    string genre;
    int releaseDate;
    int tempo;
    string explicit_;
    int popularity;
    int energy;
    int danceability;

    Song(string art, string sng, string emo, string gen, int rel, int tmp,
         string exp, int pop, int eng, int dance);

    void print() const;
};


#endif //PROJECT2_SONG_H