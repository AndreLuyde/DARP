/*
 * Guloso.h
 *
 *  Created on: 12 de set de 2017
 *      Author: andre
 */

#ifndef SRC_GULOSO_H_
#define SRC_GULOSO_H_

#include "Graph.h"

#include <iostream>

class Guloso{
public:
	void run(Graph &g);
	int getIndexSmallTW(vector<Node> &nodes);
	bool verificaInsercao(vector<Node> &car, vector<Node> &nodos, int position, double timeMaxCar, double timeMaxDuration, double &timeTotal);
	bool verificaNumPersons(vector<Node> car, int maxNumPeople);
	bool verificaInsercaoVolta(vector<Node> &car, vector<Node> &nodos, int position, double timeMaxCar, double timeMaxDuration, double &timeTotal);
	void showResult(vector<Node> car);
	bool verificaViolacaoTW(vector<Node> car, double timeMaxDuration, double timeMaxCar, double &timeTotal);

};



#endif /* SRC_GULOSO_H_ */
