/*
 * Graph.cpp
 *
 *  Created on: 14 de ago de 2017
 *      Author: andre
 */

#include "Graph.h"

Graph::Graph() {
	// TODO Auto-generated constructor stub

}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
}

//adiciona aresta
void Graph::addArcs(int node1, int node2) {
	nodos[node1].listAdj[node1].push_back(node2);
}

//adiciona no
void Graph::addNode(Node node) {
	nodos.push_back(node);
}

//verifica Vizinho
bool Graph::isNeighboor(int node1, int node2) {
	if (find(nodos[node1].listAdj[node1].begin(),
			nodos[node1].listAdj[node1].end(), node2)
			!= nodos[node1].listAdj[node1].end())
		return true;
	return false;
}
