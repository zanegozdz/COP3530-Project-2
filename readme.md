# 🎵 TuneTree

A B+ Tree and Hash Table–Powered Music Library Search Engine


# Overview

TuneTree is a high-performance music library search engine built in C++.
It uses multiple B+ trees to index song attributes — allowing fast, flexible searches across thousands of tracks.

Search by artist, genre, emotion, or perform range queries (e.g., find songs with popularity between 70–90).
Perfect for exploring datasets and benchmarking indexing structures.

# How It Works

### Initialization: 
- A MusicLibrary object is created and configured with B+ trees for each attribute.

### Data Loading
- The program calls loadData() to import songs from a CSV file. 
- Allow for the program to initialize the data structures and __WAIT__ for the menu to appear
- It may take a moment for the data structures to load up


# Menu System
- **Options:** Filtered Search, Range Search, and Quit

    - Select a menu option using the description below

1. #### Filtered Search: Match specific attributes (e.g., Artist = “Drake”, Genre = “Hip-Hop”).

  - If there are no attributes matching the search the program will show 0 results
  - The program also runs a benchmark testing which data structure runs quicker displaying various stats

2. #### Range Search: Search between two values (e.g., Popularity 60–80).

3. #### Exit: Quit the program gracefully.

Example Run
==================================
==================================

1. Creating music library...
2. Loading data from CSV...
3. Data loaded successfully!
4. Building B+ trees...
5. B+ trees built.

============== MENU ==============

1. Filtered Search
2. Range Search
3. Exit
   Pick an option: 1


Filtered Search Example
==================================

```text
========= FILTERED SEARCH =========
(if the value is none, press enter)

Artist (Ex. Jason Derulo): Jason Derulo

Title (Ex. Take You Dancing):

Emotion (Ex. joy):

Genre (Ex. pop): pop

Release Date (Ex. 2020): 2020

Tempo (Ex. 113):

Explicit (Ex. No):

Popularity (Ex. 72):

Energy (Ex. 71):

Danceability (Ex. 79):


========= BENCHMARK TEST =========

HASH TABLE:
    Total time: 6824400 ns
    Average time: 6824400 ns
    Results found: 2

B+ TREE:
    Total time: 4635700 ns
    Average time: 4635700 ns
    Results found: 2

B+ Tree is 1.00000x FASTER

========= SEARCH RESULTS =========

ARTIST               TITLE               EMOTION   GENRE       RELEASE TEMPO  EXPLICIT  POPULARITY   ENERGY  DANCEABILITY
---------------------------------------------------------------------------------------------------------------
Jason Derulo         Take You Dancing    joy       pop         2020    113    No       72           71      79
---------------------------------------------------------------------------------------------------------------
1 results found


