/*
 * Car.h
 *
 *  Created on: 5 de set de 2017
 *      Author: andre
 */

#ifndef CAR_H_
#define CAR_H_

#include "Node.h"
using namespace std;

class Car{
public:
	Car();

	double getDistance() const {
		return distance;
	}

	void setDistance(double distance) {
		this->distance = distance;
	}

	const vector<Node>& getRoute() const {
		return route;
	}

	void setRoute(const vector<Node>& route) {
		this->route = route;
	}

private:
	double distance;
	vector<Node> route;
};

#endif /* CAR_H_ */
