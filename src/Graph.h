/*
 * Graph.h
 *
 *  Created on: 14 de ago de 2017
 *      Author: andre
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <list>
#include <vector>
#include <algorithm>

#include "Node.h"

using namespace std;

class Graph {

public:
	Graph();

	virtual ~Graph();

	int getCapacidade() const {
		return capacidade;
	}

	void setCapacidade(int capacidade) {
		this->capacidade = capacidade;
	}

	int getNumRequisicoes() const {
		return numRequisicoes;
	}

	void setNumRequisicoes(int numRequisicoes) {
		this->numRequisicoes = numRequisicoes;
	}

	int getQtdCarros() const {
		return qtdCarros;
	}

	void setQtdCarros(int qtdCarros) {
		this->qtdCarros = qtdCarros;
	}

	double getMaxRideTime() const {
		return maxRideTime;
	}

	void setMaxRideTime(double maxRideTime) {
		this->maxRideTime = maxRideTime;
	}
	int getMaxDuration() const {
		return maxDuration;
	}

	void setMaxDuration(double maxDuration) {
		this->maxDuration = maxDuration;
	}
	vector<Node> const &getNodos() const {
		return nodos;
	}
	void setNodos(vector<Node> node) {
		this->nodos = move(node);
	}
	//    Node getNode(){
	//        return node;
	//    }
	void addNode(Node node);
	void addArcs(int node1, int node2);
	bool isNeighboor(int node1, int node2);

protected:

private:
	vector<Node> nodos;
	int numRequisicoes;
	int qtdCarros;
	int capacidade;
	double maxRideTime; //tempo máximo de permanencia do cliente no carro
	int maxDuration; //tempo máximo de rota

};

#endif /* GRAPH_H_ */
