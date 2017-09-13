/*
 * ILS.h
 *
 *  Created on: 12 de set de 2017
 *      Author: andre
 */

#ifndef SRC_ILS_H_
#define SRC_ILS_H_

#include "Car.h"

#include <vector>

using namespace std;

class ILS {
public:
	ILS(vector<Car> solution);
	void localSearch();
	void disturbance();
	void run();

	const vector<Car>& getInicialSolution() const {
		return inicialSolution;
	}

	void setInicialSolution(const vector<Car>& inicialSolution) {
		this->inicialSolution = inicialSolution;
	}

private:
	vector<Car> inicialSolution;
};

#endif /* SRC_ILS_H_ */
