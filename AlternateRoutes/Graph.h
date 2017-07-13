#pragma 
#include "json.hpp"
#include <fstream>
#include <string>
#include <unordered_map>
using namespace std;

using json = nlohmann::json;

class Graph{

private:
	float** adjacencyMatrix;
	int vertexCount;
	int edgesCount;
	map<string, int> verticesHash;

public:
	json data;
	Graph(json data) : data(data) {
		vertexCount = data["nodes"].size();
		edgesCount = data["edges"].size();
		adjacencyMatrix = new float*[vertexCount];

		for (int i = 0; i < vertexCount; i++) {
			if (!verticesHash[data["nodes"][i]]) {
				verticesHash[data["nodes"][i]] = i;
			} else {

			}
		}

		for (int i = 0; i < vertexCount; i++) {
			adjacencyMatrix[i] = new float[vertexCount];
			for (int j = 0; j < vertexCount; j++)
				adjacencyMatrix[i][j] = -1.0;
		}

		for (int i = 0; i < edgesCount; i++) {
			int edge = verticesHash[data["edges"][i]["association"][0]];
			int edge1 = verticesHash[data["edges"][i]["association"][1]];
			this->addEdge(edge, edge1, data["edges"][i]["weight"]);
		}
	};

	Graph();
	~Graph();
	int getSize();

	void addEdge(int a, int b, float weight);
	float getDistance(string a, string b);
	float getDistance(int a, int b);
	int minDistance(float dist[], bool sptSet[]);
	void shortestPath(string src);
	void printSolution(float dist[], int n);
};

