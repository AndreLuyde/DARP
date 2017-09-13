/*
 * Guloso.cpp
 *
 *  Created on: 12 de set de 2017
 *      Author: andre
 */

#include "Guloso.h"

//resolução de modo guloso, inserindo requisição de embarque e desembarque seguidas
void Guloso::run(Graph &g) {
	vector<Node>::iterator it;
	vector<vector<Node>> carros;
	double timeMaxRide = g.getMaxRideTime(); //maior distancia percorrida por cada carro
	double timeTotalRide = 0;
	double timeMaxDuration = g.getMaxDuration(); //maxima duração de cada pessoa no carro
	int numPersons = 0;
	int numRequisicoes = g.getNumRequisicoes(); //adiciona numero de requisições sem considerar a volta
	int maxPeopleCar = g.getCapacidade(); //capacidade máxima do carro
	vector<Node> nodos = g.getNodos();
	vector<Node> carro;
	carro.push_back(nodos[0]); //adiciona a garagem(ponto de partida) no carro
	int k = 0;
	int countNodos = nodos.size() / 2;
	int sizeNodos = nodos.size();
	//retirar o ultimo no se ele for a garagem
	if (nodos.back().numPersons == 0) {
		nodos.pop_back();
		countNodos--;
	}
	while (countNodos > 0) {
		//coloca todos os nós que ainda não estão nos carros como não verificados
		for(it = nodos.begin(); it < nodos.end(); it++){
			if (!it->atendida) {
				it->verificado = false;
			}
		}
		for (int i = 0; i < sizeNodos / 2; i++) {
//			showResult(carro);
			int dist = getIndexSmallTW(nodos);
			double oldTT = timeTotalRide; //salva o tempo atual de descida
			if (dist == -1) {
				break;
			} else {
				if (nodos[dist].numPersons > 0) {
					if (!nodos[dist].atendida) { //verifica se ainda não foi atendido
						//se o tempo total da viagem for menor que o inicio da janela, o cara espera
						vector<Node> car2 = carro;
						if (verificaInsercao(car2, nodos, dist, timeMaxRide, timeMaxDuration, timeTotalRide)) { //insere e verifica viabilidade	//Adicionar verificar tempo carro
							if (numPersons >= maxPeopleCar) {
								if (verificaNumPersons(car2, maxPeopleCar)) {
									numPersons += nodos[dist].numPersons;
									//colocar distancia negativa para não ser selecionada mais
									for (int j = 0; j < sizeNodos; j++) {
										nodos[j].listDist[dist] = -1;
									}
									countNodos--; //diminui a quantidade requisiçõess
									k++;
									carro = car2;
								} else {
									nodos[dist].atendida = false;
									nodos[dist + numRequisicoes].atendida = false;
								}
							} else {
								numPersons += nodos[dist].numPersons; //falta ajustar
								//colocar distancia negativa para não ser selecionada mais
								for (int j = 0; j < sizeNodos; j++) {
									nodos[j].listDist[dist] = -1;
								}
								countNodos--; //diminui a quantidade requisiçõess
								k++;
								carro = car2;
							}
						} else {
							timeTotalRide = oldTT;
						}

					}
				} else {
					if (!nodos[dist].atendida) { //verifica se ainda não foi atendido
						vector<Node> car2 = carro;
						if (verificaInsercaoVolta(car2, nodos, dist,
								timeMaxRide, timeMaxDuration, timeTotalRide)) { //insere e verifica viabilidade	//Adicionar verificar tempo carro
							if (numPersons >= maxPeopleCar) {
								if (verificaNumPersons(car2, maxPeopleCar)) {
									numPersons += nodos[dist - numRequisicoes].numPersons;
									//colocar distancia negativa para não ser selecionada mais
									for (int j = 0; j < sizeNodos; j++) {
										nodos[j].listDist[dist] = -1;
									}
									countNodos--; //diminui a quantidade requisiçõess
									k++;
									carro = car2;
								} else {
									nodos[dist].atendida = false;
									nodos[dist - numRequisicoes].atendida = false;
								}
							} else {
								numPersons += nodos[dist - numRequisicoes].numPersons; //falta ajustar
								//colocar distancia negativa para não ser selecionada mais
								for (int j = 0; j < sizeNodos; j++) {
									nodos[j].listDist[dist] = -1;
								}
								countNodos--; //diminui a quantidade requisiçõess
								k++;
								carro = car2;
							}

						}
					}
				}
			}
		}
		//passa para o proximo carro
		carro.push_back(nodos[0]);
		carros.push_back(carro);
		showResult(carro);
		carro.clear();
		k = 0;
		numPersons = 0;
		carro.push_back(nodos[0]);
		timeTotalRide = timeMaxDuration;
	}
}

//retorna o indice do requisição com menor janela que não foi verificado
int Guloso::getIndexSmallTW(vector<Node> &nodes) {
	double smallTw = -1;
	int smallIndex = -1;
	int sizeNodes = nodes.size();
	for (int i = 1; i < sizeNodes; i++) {
		if (smallTw == -1 && nodes[i].twA > 0 && !nodes[i].verificado) {
			smallTw = nodes[i].twA;
			smallIndex = i;
		} else if (nodes[i].twA < smallTw && nodes[i].twA > 0 && !nodes[i].verificado) {
			smallTw = nodes[i].twA;
			smallIndex = i;
		}
	}
	nodes[smallIndex].verificado = true;
	return smallIndex;
}

bool Guloso::verificaInsercao(vector<Node> &car, vector<Node> &nodos, int position, double timeMaxCar, double timeMaxDuration, double &timeTotal) {
	double timeCar = 0;
	int positionInsert = nodos.size() / 2;
	bool insert = false;
	int sizeCar = car.size();
	for (int i = 0; i < sizeCar - 1; i++) {
		if (timeCar < car[i].twA) {
			timeCar = car[i].twA;
			timeCar += car[i].listDist[i + 1];
		} else {
			timeCar += car[i].listDist[i + 1];
		}
	}
	if (nodos[position].twB >= timeCar || nodos[position].twA >= timeCar) {
		timeCar += car[car.size() - 1].listDist[nodos[position].node];
		car.push_back(nodos[position]);
		timeCar +=
				car[car.size() - 1].listDist[nodos[position + positionInsert].node];
		car.push_back(nodos[position + positionInsert]);
		insert = true;
	}
	if (insert) {
		//faz a verificação se está ok depois da inserção
		if (verificaViolacaoTW(car, timeMaxDuration, timeMaxCar, timeTotal)) {
			return false;
		} else {
			nodos[positionInsert + position].atendida = true;
			nodos[position].atendida = true;
			return true;
		}
	} else {
		return false;
	}
}

// verificação se não excede o numero máximo de pessoas por carro olhando a viagem completa
bool Guloso::verificaNumPersons(vector<Node> car, int maxNumPeople) {
	int numPeople = 0;
	int size = car.size();
	bool avaliable = true;
	for (int i = 0; i < size; i++) {
		numPeople += car[i].numPersons;
		if (numPeople > maxNumPeople) {
			avaliable = false;
		}
	}
	return avaliable;
}

bool Guloso::verificaInsercaoVolta(vector<Node> &car, vector<Node> &nodos, int position, double timeMaxCar, double timeMaxDuration, double &timeTotal) {
	double timeCar = 0;
	int positionInsert = nodos.size() / 2;
	bool insert = false;
	int sizeCar = car.size();
	for (int i = 0; i < sizeCar - 1; i++) {
		if (timeCar < car[i].twA) {
			timeCar = car[i].twA;
			timeCar += car[i].listDist[i + 1];
		} else {
			timeCar += car[i].listDist[i + 1];
		}
	}
	timeCar += car[car.size() - 1].listDist[nodos[position - positionInsert].node];
	if (nodos[position].twB >= timeCar || nodos[position].twA >= timeCar) {
		car.push_back(nodos[position - positionInsert]);
		timeCar += car[car.size() - 1].listDist[nodos[position].node];
		car.push_back(nodos[position]);
		insert = true;
	}
	if (insert) {
		//faz a verificação se está ok depois da inserção
		if (verificaViolacaoTW(car, timeMaxDuration, timeMaxCar, timeTotal)) {
			return false;
		} else {
			nodos[positionInsert + position].atendida = true;
			nodos[position].atendida = true;
			return true;
		}
	} else {
		return false;
	}
}

//Mostra o conteudo do carro	//precisa ser melhorada
void Guloso::showResult(vector<Node> car) {
	cout << "================================================================"<< endl;
	cout << "Carro" << endl;
	double timeCar = 0;
	car.push_back(car[0]);
	int sizeCar = car.size();
	for (int i = 0; i < sizeCar - 1; i++) {
		timeCar += car[i].listDist[car[i + 1].node];
		if (timeCar < car[i].twA) {
			timeCar = car[i].twA;
		}
		if (car[i].numPersons > 0) {
			cout << "Requisição: " << car[i].node << endl;
			cout << "Janela A: " << car[i].twA;
			cout << " Janela B: " << car[i].twB << endl;
			cout << "Tempo do carro: " << timeCar << endl;
			cout << "---------------------------------------" << endl;
		} else {
			cout << "Requisição: " << car[i].node << endl;
			cout << "Janela A: " << car[i].twA;
			cout << " Janela B: " << car[i].twB << endl;
			cout << "Tempo do carro: " << timeCar << endl;
			cout << "---------------------------------------" << endl;
		}
	}
}

//verifica se não há violação na janela de tempo e o tempo máximo do carro
bool Guloso::verificaViolacaoTW(vector<Node> car, double timeMaxDuration, double timeMaxCar, double &timeTotal) {
	double timeCar = 0;
	bool violacao = false;
	int sizeCar = car.size();
	//verifica se não viola nenhuma janela de tempo depois da inserção
	timeCar += car[0].listDist[car[1].node];
	if (timeCar < car[1].twA) {
		timeCar = car[1].twA;
	}
	for (int i = 1; i < sizeCar - 1; i++) {
		timeCar += car[i].listDist[car[i + 1].node];
		if (timeCar > car[i].twB) {
			violacao = true;
		}
		if (timeCar < car[i].twA) {
			timeCar = car[i].twA;
		}
	}
	//verifica se não viola o tempo máximo que o carro pode percorrer e atualiza o tempo total de percurso
	if ((timeCar) > timeMaxDuration) {
		violacao = true;
	} else {
		timeTotal = timeCar;
	}
	return violacao;
}

