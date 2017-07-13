#include "stdafx.h"
#include "Graph.h"
#include "json.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <climits>

using json = nlohmann::json;
using namespace std;

Graph::Graph() {
}

Graph::~Graph()
{
}

int Graph::getSize()
{
	return this->vertexCount;
}

void Graph::addEdge(int a, int b, float weight)
{
	if (a >= 0 && a < vertexCount && b >= 0 && b < vertexCount) {
		adjacencyMatrix[a][b] = weight;
		adjacencyMatrix[b][a] = weight;
	}
}

// Get distance between two neighbors
float Graph::getDistance(string a, string b)
{
	if (adjacencyMatrix[verticesHash[a]][verticesHash[b]] != -1.0) {
		return adjacencyMatrix[verticesHash[a]][verticesHash[b]];
	}
	return NULL;
}

// Overloaded function in case we want the index of the string
float Graph::getDistance(int a, int b)
{
	if (adjacencyMatrix[a][b] != -1.0) {
		return adjacencyMatrix[a][b];
	}
	return NULL;
}

// See http://www.geeksforgeeks.org/greedy-algorithms-set-6-dijkstras-shortest-path-algorithm/
int Graph::minDistance(float dist[], bool sptSet[])
{
	// Initialize min value
	float min = FLT_MAX, min_index;
	for (int v = 0; v < this->getSize(); v++){
		if (sptSet[v] == false && dist[v] <= min) {
			dist[v];
			v;
			min = dist[v], min_index = v;
		}
	}
	return min_index;
}

// See http://www.geeksforgeeks.org/greedy-algorithms-set-6-dijkstras-shortest-path-algorithm/
void Graph::shortestPath(string src) {
	// Dynamically sized array of floats or booleans
	float* dist = NULL;   
	int n = this->getSize();           
	dist = new float[n]; 

	bool* sptSet = NULL;   
	int p = this->getSize();         
	sptSet = new bool[p];

	int V = this->getSize();

	// Initialize all distances as INFINITE and stpSet[] as false
	for (int i = 0; i < V; i++)
		dist[i] = FLT_MAX, sptSet[i] = false;

	// Distance of source vertex from itself is always 0
	int index = this->verticesHash[src];
	dist[index] = 0.0;

	// Find shortest path for all vertices
	for (int count = 0; count < V; count++)
	{
		// Pick the minimum distance vertex from the set of vertices not
		// yet processed. u is always equal to src in first iteration.
		int u = minDistance(dist, sptSet);

		// Mark the picked vertex as processed

		sptSet[u] = true;
		// Update dist value of the adjacent vertices of the picked vertex.
		for (int v = 0; v < V; v++) {

			// Update dist[v] only if is not in sptSet, there is an edge from 
			// u to v, and total weight of path from src to  v through u is 
			// smaller than current value of dist[v]
			float aye = this->getDistance(u, v);
			if (!sptSet[v] && this->getDistance(u, v) && dist[u] != FLT_MAX
				&& dist[u] + this->getDistance(u, v) < dist[v]){ 
				
				dist[v] = dist[u] + this->getDistance(u, v);
			}
		}
	}
	
	// print the constructed distance array
	printSolution(dist, V);
}


void Graph::printSolution(float dist[], int n)
{
	ofstream log;
	log.open("results.txt");
	
	// Find the key based on the value
	for (int i = 0; i < this->getSize(); i++) {
		string key;
		for (auto &j : this->verticesHash) {
			if (j.second == i) {
				key = j.first;
				break;
			}
		}
		log << key << " takes " << dist[i] << "meters\n";
	}

	log << "End Session \n\n";
	log.close();
}