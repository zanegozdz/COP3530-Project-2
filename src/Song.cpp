//
// Created by Raymond Adams on 11/2/25.
//

#include "Song.h"

Song::Song(string art, string sng, string emo, string gen, int rel, int tmp,
           string exp, int pop, int eng, int dance)
    : artist(art), songName(sng), emotion(emo), genre(gen), releaseDate(rel),
      tempo(tmp), explicit_(exp), popularity(pop), energy(eng), danceability(dance) {}

void Song::print() const {
    cout << "Artist: " << artist << endl;
    cout << "Song: " << songName << endl;
    cout << "Emotion: " << emotion << ", Genre: " << genre << endl;
    cout << "Release Date: " << releaseDate << ", Tempo: " << tempo << endl;
    cout << "Explicit: " << explicit_ << ", Popularity: " << popularity << endl;
    cout << "Energy: " << energy << ", Danceability: " << danceability << endl;
    cout << "-----------------------------------" << endl;
}