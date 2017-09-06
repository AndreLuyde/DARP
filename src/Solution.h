/*
 * Solution.h
 *
 *  Created on: 5 de set de 2017
 *      Author: andre
 */

#ifndef SOLUTION_H_
#define SOLUTION_H_

#include "Node.h"
using namespace std;

class Solution{
public:
	Solution();

	const vector<Node>& getCar() const {
		return car;
	}

	void setCar(const vector<Node>& car) {
		this->car = car;
	}

	double getDistance() const {
		return distance;
	}

	void setDistance(double distance) {
		this->distance = distance;
	}

private:
	double distance;
	vector<Node> car;
};



#endif /* SOLUTION_H_ */
