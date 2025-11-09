# 🎵 TuneTree
A B+ Tree and Hash Table–Powered Music Library Search Engine

---

## Overview
TuneTree is a music library search engine.  
It uses multiple **B+ Trees** and **Hash Tables** to index song attributes — allowing fast, flexible searches across thousands of tracks.

Search by artist, genre, emotion, or perform range queries (e.g., find songs with popularity between 70–90).  
Perfect for exploring datasets and benchmarking data structures.

---

## How It Works

### Initialization
- A `MusicLibrary` object is created and configured with B+ Trees and Hash Tables for each attribute.

### Data Loading
- The program calls `loadData()` to import songs from a CSV file.
- The program initializes the data structures and **waits** for the menu to appear.
- It may take a moment for the data structures to finish loading.

---

## Menu System

### Options
- **Filtered Search**
- **Range Search**
- **Quit**

Select a menu option using the descriptions below:

---

### 1. Filtered Search
Match specific attributes (e.g., Artist = “Drake”, Genre = “Hip-Hop”).

- If no attributes match, the program will show **0 results**.
- The program also runs a **benchmark test**, comparing the performance of the Hash Table and B+ Tree and displaying statistics.

---

### 2. Range Search
Search between two values (e.g., Popularity 60–80).

---

### 3. Exit
Quit the program.

---

## Example Run

```text
==================================
             TuneTree
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
```
---
## Filtered Search Example
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
1 result found
```
---
## Range Search Example
```text
========== RANGE SEARCH ==========
(if the value is none, press enter)

[1]Artist, [2]Title, [3]Emotion, [4]Genre, [5]Release Date,
[6]Tempo, [7]Explicit, [8]Popularity, [9]Energy, [10]Danceability
Pick an option: 1

Enter minimum value: Adam Snow
Enter maximum value: Adara

========= BENCHMARK TEST =========

HASH TABLE:
Total time: 206508600 ns
Average time: 2065086 ns
Results found: 10

B+ TREE:
Total time: 228600 ns
Average time: 2286 ns
Results found: 10

B+ Tree is 903.00000x FASTER

========= SEARCH RESULTS =========

-----------------------------------------------------------------------------------------------------------------------
ARTIST              TITLE               EMOTION   GENRE       RELEASE TEMPO  EXPLICIT POPULARITY   ENERGY  DANCEABILITY
-----------------------------------------------------------------------------------------------------------------------
Adam Snow           9 to 5              sadness   Unknown,U.. 2015    93     No       27           65      79
Adam Snow           Pray For Me         sadness   Unknown,U.. 2024    175    No       55           89      46
Adam Torres         Green Mountain Ro.. love      Unknown,U.. 2017    107    No       38           24      41
Adam and the Ants   Prince Charming     joy       pop         2024    150    No       46           67      54
Adam and the Ants   Antmusic            sadness   pop         2016    81     No       47           81      70
Adamn Killa         Ten                 sadness   rap         2016    80     Yes      33           37      86
Adamski             Killer              joy       electronic  1990    120    No       57           55      78
Adamusic            DECADE OF POP | T.. joy       Unknown,U.. 2012    126    No       48           85      72
Adana Twins         Strange             fear      deep hous.. 2012    120    No       46           66      86
Adara               Dancing With a St.. sadness   Unknown,U.. 2020    103    No       79           52      74
-----------------------------------------------------------------------------------------------------------------------

10 results found
```