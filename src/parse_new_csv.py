import csv

cols_to_remove = {3, 6, 8, 13, 14, 15, 16, 17}

with open("data/light_spotify_dataset.csv", newline='', encoding='utf-8') as infile:
    reader = list(csv.reader(infile))

with open("data/new_dataset.csv", 'w', newline='', encoding='utf-8') as outfile:
    writer = csv.writer(outfile)
    for row in reader:
        filtered = []
        for i, val in enumerate(row):
            if i not in cols_to_remove:
                filtered.append(val)
        writer.writerow(filtered)