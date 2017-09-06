/*
 * Node.h
 *
 *  Created on: 5 de set de 2017
 *      Author: andre
 */

#ifndef SRC_NODE_H_
#define SRC_NODE_H_

#include <list>
#include <vector>
#include <algorithm>

using namespace std;

class Node {
public:
	Node();
	int node;
	double coordX;
	double coordY;
	double timeService;
	int numPersons;
	double twA;
	double twB;
	bool atendida = false;
	bool verificado = false;
	vector<double> listDist;
	vector<int> *listAdj;
};

#endif /* SRC_NODE_H_ */
