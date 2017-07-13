"""
SYNOPSIS
    This will take the file named Distances1.csv, add create a json for the
	nodes and edges that it finds.
		
AUTHOR
    Programmer: Ivelin Tchangalov <i.tchangalov@gmail.com>
"""

import csv
import json
filename = 'Distances1.csv'

# Open csv file, add raw data to array
with open(filename, 'r') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',')
	data = []
	for row in spamreader:
		data.append(row)
		
	
# The two main components. Second one is a set to keep elements unique
Nodes = set([])
Edges = []

# for each row, add in nodes and weight associations
for row in data:
	Nodes.add(row[0])
	Nodes.add(row[1])
	try:
		curr = {"association": [row[0],row[1]], "weight":float(row[2])}
		Edges.append(curr)
	except:
		print("mistake")
		pass

# Construct overall dict object and write to the file
with open('SampleGraph2.json', 'w') as outfile:
    json.dump({"nodes":sorted(Nodes), "edges":Edges}, outfile)