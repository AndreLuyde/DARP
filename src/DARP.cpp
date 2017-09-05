//============================================================================
// Name        : DARP.cpp
// Author      : André Luyde
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <string.h>
#include "Graph.h"

using namespace std;

void stringSplit(string str, string delim, vector<string>& results);
void calcuaDistancias(Graph &g);
double distanciaEntrePontos(Node node1, Node node2);
int getIndiceMenorDistancia(vector<double> distancias, vector<Node> &nodes);
int getIndexNextWindow(vector<Node> &nodes, double timeWindow);
void metodoGulosoMelhorado(Graph &g);
void showResult(vector<Node> car);
bool verificaInsercao(vector<Node> &car, vector<Node> &nodos, int position,
		double timeCar, double timeMaxDuration, double &timeTotal);
bool verificaViolacaoTW(vector<Node> car, double timeMaxDuration,
		double timeMaxCar, double &timeTotal);
void metodoGulosoSimples(Graph &g);
bool verificaNumPersons(vector<Node> car, int maxNumPeople);
bool verificaInsercaoVolta2(vector<Node> &car, vector<Node> &nodos,
		int position, double timeMaxCar, double timeInicialCar,
		double &timeTotal);
int getIndexSmallTW(vector<Node> &nodes);
double getSizeTWCar(vector<Node> car);
bool verificaInsercaoVoltaGulosa(vector<Node> &car, vector<Node> &nodos,
		int position, double timeMaxCar, double timeMaxDuration,
		double &timeTotal);
bool verificaInsercaoGulosa(vector<Node> &car, vector<Node> &nodos,
		int position, double timeMaxCar, double timeMaxDuration,
		double &timeTotal);
int main() {

	Graph G;

	//Leitura do arquivo e Armazenamento do Grafo
	ifstream readlist;
	ofstream resultList;
	vector<string> fileslist;
	string lineRead;

	readlist.open("/home/andre/Documentos/DARP/DARP/a2-16.txt");

	while (getline(readlist, lineRead)) {
		stringstream ss;
		ss << lineRead;
		fileslist.push_back(ss.str());
	}

	readlist.close();

	//dados do cabeçalho do arquivo
	vector<string> lineSplit1;
	stringSplit(fileslist[0], " ", lineSplit1);
	G.setQtdCarros(stoi(lineSplit1[0]));
	G.setNumRequisicoes(stoi(lineSplit1[1]));
	G.setMaxDuration(stoi(lineSplit1[2]));
	G.setCapacidade(stoi(lineSplit1[3]));
	G.setMaxRideTime(stoi(lineSplit1[4]));

	//armazenando no grafo os dados lidos
	for (int i = 1; i < fileslist.size(); i++) {
		vector<string> lineSplit;
		Node node;
		string line = fileslist[i];

		stringSplit(line, " ", lineSplit);

		node.node = stoi(lineSplit[0]);
		node.coordX = stod(lineSplit[1]);
		node.coordY = stod(lineSplit[2]);
		node.timeService = stoi(lineSplit[3]);
		node.numPersons = stoi(lineSplit[4]);
		node.twA = stod(lineSplit[5]);
		node.twB = stod(lineSplit[6]);

		G.addNode(node);
	}

	//calcular distâncias
	calcuaDistancias(G);

	//método guloso
	metodoGulosoSimples(G);

//	cout << "Hello world!" << endl;
	return 0;
}

void metodoGulosoMelhorado(Graph &g) {
	vector<vector<Node>> carros;
	double timeMaxRide = g.getMaxRideTime();
	double timeTotalRide = 0;
	double timeMaxDuration = g.getMaxDuration();
	int numPersons = 0;
	int numRequisicoes = g.getNumRequisicoes(); //adiciona numero de requisições sem considerar a volta
	int maxPeopleCar = g.getCapacidade();
	vector<Node> nodos = g.getNodos();
	vector<Node> carro;
	carro.push_back(nodos[0]);
	int k = 0;
	int countNodos = nodos.size() / 2;
	//retirar o ultimo no se ele for a garagem
	if (nodos.back().numPersons == 0) {
		nodos.pop_back();
		countNodos--;
	}
	while (countNodos > 0) {
		for (int i = 0; i < nodos.size(); i++) {
			if (!nodos[i].atendida) {
				nodos[i].verificado = false;
			}
		}
		for (int i = 0; i < nodos.size() / 2; i++) {
			showResult(carro);
			//			int dist = getIndiceMenorDistancia(nodos[k].listDist, nodos); //pega o mais próximo
			//			int dist = getIndexNextWindow(nodos, timePickUp+timeInicialRide);
			int dist = getIndexSmallTW(nodos);
			double oldTT = timeTotalRide; //salva o tempo atual de descida
			if (dist == -1) {
				break;
			} else {
				if (nodos[dist].numPersons > 0) {
					if (!nodos[dist].atendida) { //verifica se ainda não foi atendido
						//se o tempo total da viagem for menor que o inicio da janela, o cara espera
						//						if (timeTotalRide < nodos[dist].twA) {
						//							timeTotalRide += nodos[dist].twA - timeTotalRide;
						//						}
						vector<Node> car2 = carro;
						if (verificaInsercao(car2, nodos, dist, timeMaxRide,
								timeMaxDuration, timeTotalRide)) { //insere e verifica viabilidade	//Adicionar verificar tempo carro
							if (numPersons >= maxPeopleCar) {
								if (verificaNumPersons(car2, maxPeopleCar)) {
									numPersons += nodos[dist].numPersons;
									//colocar distancia negativa para não ser selecionada mais
									for (int j = 0; j < nodos.size(); j++) {
										nodos[j].listDist[dist] = -1;
									}
									countNodos--; //diminui a quantidade requisiçõess
									k++;
									carro = car2;
								} else {
									nodos[dist].atendida = false;
									nodos[dist + numRequisicoes].atendida =
											false;
								}
							} else {
								numPersons += nodos[dist].numPersons; //falta ajustar
								//colocar distancia negativa para não ser selecionada mais
								for (int j = 0; j < nodos.size(); j++) {
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
						if (verificaInsercaoVolta2(car2, nodos, dist,
								timeMaxRide, timeMaxDuration, timeTotalRide)) { //insere e verifica viabilidade	//Adicionar verificar tempo carro
							if (numPersons >= maxPeopleCar) {
								if (verificaNumPersons(car2, maxPeopleCar)) {
									numPersons +=
											nodos[dist - numRequisicoes].numPersons;
									//colocar distancia negativa para não ser selecionada mais
									for (int j = 0; j < nodos.size(); j++) {
										nodos[j].listDist[dist] = -1;
									}
									countNodos--; //diminui a quantidade requisiçõess
									k++;
									carro = car2;
								} else {
									nodos[dist].atendida = false;
									nodos[dist - numRequisicoes].atendida =
											false;
								}
							} else {
								numPersons +=
										nodos[dist - numRequisicoes].numPersons; //falta ajustar
								//colocar distancia negativa para não ser selecionada mais
								for (int j = 0; j < nodos.size(); j++) {
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
void metodoGulosoSimples(Graph &g) {
	vector<vector<Node>> carros;
	double timeMaxRide = g.getMaxRideTime();
	double timeTotalRide = 0;
	double timeMaxDuration = g.getMaxDuration();
	int numPersons = 0;
	int numRequisicoes = g.getNumRequisicoes(); //adiciona numero de requisições sem considerar a volta
	int maxPeopleCar = g.getCapacidade();
	vector<Node> nodos = g.getNodos();
	vector<Node> carro;
	carro.push_back(nodos[0]);
	int k = 0;
	int countNodos = nodos.size() / 2;
	//retirar o ultimo no se ele for a garagem
	if (nodos.back().numPersons == 0) {
		nodos.pop_back();
		countNodos--;
	}
	while (countNodos > 0) {
		for (int i = 0; i < nodos.size(); i++) {
			if (!nodos[i].atendida) {
				nodos[i].verificado = false;
			}
		}
		for (int i = 0; i < nodos.size() / 2; i++) {
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
						if (verificaInsercaoGulosa(car2, nodos, dist,
								timeMaxRide, timeMaxDuration, timeTotalRide)) { //insere e verifica viabilidade	//Adicionar verificar tempo carro
							if (numPersons >= maxPeopleCar) {
								if (verificaNumPersons(car2, maxPeopleCar)) {
									numPersons += nodos[dist].numPersons;
									//colocar distancia negativa para não ser selecionada mais
									for (int j = 0; j < nodos.size(); j++) {
										nodos[j].listDist[dist] = -1;
									}
									countNodos--; //diminui a quantidade requisiçõess
									k++;
									carro = car2;
								} else {
									nodos[dist].atendida = false;
									nodos[dist + numRequisicoes].atendida =
											false;
								}
							} else {
								numPersons += nodos[dist].numPersons; //falta ajustar
								//colocar distancia negativa para não ser selecionada mais
								for (int j = 0; j < nodos.size(); j++) {
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
						if (verificaInsercaoVoltaGulosa(car2, nodos, dist,
								timeMaxRide, timeMaxDuration, timeTotalRide)) { //insere e verifica viabilidade	//Adicionar verificar tempo carro
							if (numPersons >= maxPeopleCar) {
								if (verificaNumPersons(car2, maxPeopleCar)) {
									numPersons +=
											nodos[dist - numRequisicoes].numPersons;
									//colocar distancia negativa para não ser selecionada mais
									for (int j = 0; j < nodos.size(); j++) {
										nodos[j].listDist[dist] = -1;
									}
									countNodos--; //diminui a quantidade requisiçõess
									k++;
									carro = car2;
								} else {
									nodos[dist].atendida = false;
									nodos[dist - numRequisicoes].atendida =
											false;
								}
							} else {
								numPersons +=
										nodos[dist - numRequisicoes].numPersons; //falta ajustar
								//colocar distancia negativa para não ser selecionada mais
								for (int j = 0; j < nodos.size(); j++) {
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

double getSizeTWCar(vector<Node> car) {
	double timeCar = 0;
	vector<Node>::iterator it;
	int i = car.size() - 1;
	for (it = car.end() - 1; it >= car.begin(); it--) {
		if (it->twA > 0) {
			return timeCar + it->twA;
			break;
		}
		timeCar += it->listDist[car[i - 1].node];
		i++;
	}
	return timeCar;
}

bool verificaInsercaoGulosa(vector<Node> &car, vector<Node> &nodos,
		int position, double timeMaxCar, double timeMaxDuration,
		double &timeTotal) {
	double timeCar = 0;
	int positionInsert = nodos.size() / 2;
	bool insert = false;
	for (int i = 0; i < car.size() - 1; i++) {
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

bool verificaInsercaoVoltaGulosa(vector<Node> &car, vector<Node> &nodos,
		int position, double timeMaxCar, double timeMaxDuration,
		double &timeTotal) {
	double timeCar = 0;
	int positionInsert = nodos.size() / 2;
	bool insert = false;
	for (int i = 0; i < car.size() - 1; i++) {
		if (timeCar < car[i].twA) {
			timeCar = car[i].twA;
			timeCar += car[i].listDist[i + 1];
		} else {
			timeCar += car[i].listDist[i + 1];
		}
	}
	timeCar +=
			car[car.size() - 1].listDist[nodos[position - positionInsert].node];
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

//faz a inserção da requisição e verifica se ela não viola as restrições
bool verificaInsercao(vector<Node> &car, vector<Node> &nodos, int position,
		double timeMaxCar, double timeMaxDuration, double &timeTotal) {
	vector<Node>::iterator it;
	int contRequest = 0;
	//timeTotal é o tempo de percurso
	//insere a requisição no carro
	int positionInsert = nodos.size() / 2;
	for (it = car.begin(); it < car.end(); it++) {
		if (car.size() < 2) {
			car.insert(it + 1, nodos[position]);
			break;
		} else {
			if (contRequest < 3) {
				if (it->twA != 0) {
					if (nodos[position].twA >= it->twA) {
						//						&& (nodos[position].twA <= timeTotal
						//								&& nodos[position].twB >= timeTotal)) {
						car.insert(it + 1, nodos[position]);
						break;
					}
				}
			}
		}
		contRequest += it->numPersons;
	}
	//insere a requisição de volta da requisição no carro
//	for (it = car.end() - 1; it >= car.begin(); it--) {
//		if (nodos[positionInsert + position].twA > it->twA) {
//			car.insert(it + 1, nodos[positionInsert + position]);
//			break;
//		} else if (nodos[positionInsert + position].twA == it->twA) {
//			if (nodos[positionInsert + position].twB > it->twB) {
//				car.insert(it + 1, nodos[positionInsert + position]);
//				break;
//			} else {
//				car.insert(it, nodos[positionInsert + position]);
//				break;
//			}
//		}
//	}
	car.push_back(nodos[positionInsert + position]);

	//faz a verificação se está ok depois da inserção
	if (verificaViolacaoTW(car, timeMaxDuration, timeMaxCar, timeTotal)) {
		return false;
	} else {
		nodos[positionInsert + position].atendida = true;
		nodos[position].atendida = true;
		return true;
	}
}

bool verificaInsercaoVolta2(vector<Node> &car, vector<Node> &nodos,
		int position, double timeMaxCar, double timeMaxDuration,
		double &timeTotal) {
	vector<Node>::iterator it;
	int contRequest = 0;
	//timeTotal é o tempo de percurso
	//insere a requisição no carro
	int positionInsert = nodos.size() / 2;
	for (it = car.begin(); it < car.end(); it++) {
		if (car.size() < 2) {
			car.insert(it + 1, nodos[position - positionInsert]);
			break;
		} else {
			if (contRequest < 3) {
				if (nodos[position - positionInsert].twA >= it->twA) {
					//						&& (nodos[position].twA <= timeTotal
					//								&& nodos[position].twB >= timeTotal)) {
					car.insert(it + 1, nodos[position - positionInsert]);
					break;
				}
			}
		}
		contRequest += it->numPersons;
	}
	//insere a requisição de volta da requisição no carro
	//	for (it = car.end() - 1; it >= car.begin(); it--) {
	//		if (nodos[positionInsert + position].twA > it->twA) {
	//			car.insert(it + 1, nodos[positionInsert + position]);
	//			break;
	//		} else if (nodos[positionInsert + position].twA == it->twA) {
	//			if (nodos[positionInsert + position].twB > it->twB) {
	//				car.insert(it + 1, nodos[positionInsert + position]);
	//				break;
	//			} else {
	//				car.insert(it, nodos[positionInsert + position]);
	//				break;
	//			}
	//		}
	//	}
	car.push_back(nodos[position]);

	//faz a verificação se está ok depois da inserção
	if (verificaViolacaoTW(car, timeMaxDuration, timeMaxCar, timeTotal)) {
		return false;
	} else {
		nodos[positionInsert + position].atendida = true;
		nodos[position].atendida = true;
		return true;
	}
}

bool verificaInsercaoVolta(vector<Node> &car, vector<Node> &nodos, int position,
		double timeMaxCar, double timeInicialCar, double &timeTotal) {
	vector<Node>::iterator it;
	int contPeople = 0;
	bool insert = false;
	double timeCar = 0;
	int positionCar = 0;
	//insere a requisição de volta da requisição no carro
	for (it = car.begin(); it < car.end(); it++) {
		timeCar += it->listDist[(it + 1)->node];
		if (contPeople < 2) {
			if (nodos[position].twA <= it->twA
					|| (it->numPersons < 0 && it->twA == 0)) {
				car.insert(it + 1, nodos[position]);
				insert = true;
				positionCar++;
				break;
			} //else if (nodos[position].twA == it->twA) {
//				if (nodos[position].twB > it->twB) {
//					car.insert(it + 1, nodos[position]);
//					insert = true;
//					positionCar++;
//					break;
//				} else {
//					car.insert(it, nodos[position]);
//					insert = true;
//					positionCar++;
//					break;
//				}
//			}
		} else {
			if (nodos[position].twB <= timeCar
					|| (it->numPersons < 0 && it->twA == 0)
					|| nodos[position].twA <= it->twA) {
				car.insert(it, nodos[position]);
				insert = true;
				positionCar++;
				break;
			}
		}
		positionCar++;
		contPeople += it->numPersons;
	}
	//encontrar uma maneira de inserir a volta quebrar restrições
	//insere a requisição no carro
	contPeople = 0;
	int positionInsert = nodos.size() / 2;
	if (insert) {
		for (it = car.begin(); it < car.begin() + positionCar; it++) {
			if (contPeople < 3) {
				if (nodos[position - positionInsert].twA >= it->twA) {
					//				&& (nodos[position - positionInsert].twA <= timeTotal && nodos[position - positionInsert].twB >= timeTotal)) {
					car.insert(it + 1, nodos[position - positionInsert]);
					break;
				}
			}
			contPeople += it->numPersons;
		}
	} else {
		return false;
	}

	//faz a verificação se está ok depois da inserção
	if (verificaViolacaoTW(car, timeInicialCar, timeMaxCar, timeTotal)) {
		return false;
	} else {
		nodos[position].atendida = true;
		nodos[position - positionInsert].atendida = true;
		return true;
	}
}

//verifica se não há violação na janela de tempo e o tempo máximo do carro
bool verificaViolacaoTW(vector<Node> car, double timeMaxDuration,
		double timeMaxCar, double &timeTotal) {
	double timeCar = 0;
	bool violacao = false;
	//verifica se não viola nenhuma janela de tempo depois da inserção
	timeCar += car[0].listDist[car[1].node];
	if (timeCar < car[1].twA) {
		timeCar = car[1].twA;
	}
	for (int i = 1; i < car.size() - 1; i++) {
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
// verificação se não excede o numero máximo de pessoas por carro olhando a viagem completa
bool verificaNumPersons(vector<Node> car, int maxNumPeople) {
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

//Mostra o conteudo do carro	//precisa ser melhorada
void showResult(vector<Node> car) {
	cout << "================================================================"
			<< endl;
	cout << "Carro" << endl;
	double timeCar = 0;
	car.push_back(car[0]);
//	timeCar = car[0].listDist[car[1].node];
//	if (timeCar < car[1].twA) {
//		timeCar = car[1].twA;
//	}
	for (int i = 0; i < car.size() - 1; i++) {
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

//calcula todas as distancias do grafo
void calcuaDistancias(Graph &g) {
	vector<Node> nodos = g.getNodos();
	for (int i = 0; i < g.getNodos().size(); i++) {
		for (int j = 0; j < g.getNodos().size(); j++) {
			if (i != j) {
				nodos[i].listDist.push_back(
						distanciaEntrePontos(nodos[i], nodos[j]));
			} else {
				nodos[i].listDist.push_back(0);
			}
		}
	}
	g.setNodos(nodos);
}

//Calcula distancia entre dois pontos
double distanciaEntrePontos(Node node1, Node node2) {
	double distance = sqrt(
			pow((node2.coordX - node2.coordX), 2)
					+ pow((node2.coordY - node1.coordY), 2));
	return distance;
}

//retorna indice da menor distancia
int getIndiceMenorDistancia(vector<double> distancias, vector<Node> &nodes) {
	int menorDistancia = -1;
	int indiceMenor = -1;
	for (int i = 1; i < nodes.size(); i++) {
		if (menorDistancia < 0 && distancias[i] > 0 && !nodes[i].verificado) {
			menorDistancia = distancias[i];
			indiceMenor = i;
		} else if (distancias[i] < menorDistancia && distancias[i] > 0
				&& !nodes[i].verificado) {
			menorDistancia = distancias[i];
			indiceMenor = i;
		}
	}
	nodes[indiceMenor].verificado = true;
	return indiceMenor;
}

//retorna indice da menor distancia
int getIndexNextWindow(vector<Node> &nodes, double timeWindow) {
	double smallTw = -1.2233;
	double diference = -1;
	int smallIndex = -1;
	for (int i = 1; i < nodes.size() / 2; i++) {
		diference = timeWindow - nodes[i].twA;
		if (smallTw == -1.2233 && !nodes[i].verificado
				&& nodes[i].twA <= timeWindow) {
			smallTw = diference;
			smallIndex = i;
		} else if (diference < smallTw && diference > 0 && !nodes[i].verificado
				&& nodes[i].twA >= timeWindow) {
			smallTw = diference;
			smallIndex = i;
		}
	}
	nodes[smallIndex].verificado = true;
	return smallIndex;
}

//retorna indice da menor distancia considerando apenas as janelas restritas
int getIndexNextWindow2(vector<Node> &nodes, double timeWindow) {
	double smallTw = -1;
	double diference = -1;
	int smallIndex = -1;
	for (int i = 1; i < nodes.size() / 2; i++) {
		diference = timeWindow - nodes[i].twA;
		if (smallTw == -1 && !nodes[i].verificado
				&& nodes[i].twA <= timeWindow) {
			smallTw = diference;
			smallIndex = i;
		} else if (diference < smallTw && diference > 0 && !nodes[i].verificado
				&& nodes[i].twA >= timeWindow) {
			smallTw = diference;
			smallIndex = i;
		}
	}
	nodes[smallIndex].verificado = true;
	return smallIndex;
}

//retorna o indice do requisição com menor janela que não foi verificado
int getIndexSmallTW(vector<Node> &nodes) {
	double smallTw = -1;
	int smallIndex = -1;
	for (int i = 1; i < nodes.size(); i++) {
		if (smallTw == -1 && nodes[i].twA > 0 && !nodes[i].verificado) {
			smallTw = nodes[i].twA;
			smallIndex = i;
		} else if (nodes[i].twA < smallTw && nodes[i].twA > 0
				&& !nodes[i].verificado) {
			smallTw = nodes[i].twA;
			smallIndex = i;
		}
	}
	nodes[smallIndex].verificado = true;
	return smallIndex;
}

void stringSplit(string str, string delim, vector<string>& results) {
	int cutAt;
	while ((cutAt = str.find_first_of(delim)) != str.npos) {
		if (cutAt > 0) {
			results.push_back(str.substr(0, cutAt));
		}
		str = str.substr(cutAt + 1);
	}
	if (str.length() > 0) {
		results.push_back(str);
	}
}

//void checkSolution() {
//
//}
//
//void readSolution(string arquivo) {
//	//Leitura do arquivo e Armazenamento do Grafo
//	ifstream readlist;
//	ofstream resultList;
//	vector<string> fileslist;
//	string lineRead;
//
//	readlist.open("/home/andre/Documentos/DARP/DARP/a2-16.txt");
//
//	while (getline(readlist, lineRead)) {
//		stringstream ss;
//		ss << lineRead;
//		fileslist.push_back(ss.str());
//	}
//
//	readlist.close();
//
//	//dados do cabeçalho do arquivo
//	vector<string> lineSplit1;
//	stringSplit(fileslist[0], " ", lineSplit1);
//	G.setQtdCarros(stoi(lineSplit1[0]));
//	G.setNumRequisicoes(stoi(lineSplit1[1]));
//	G.setMaxDuration(stoi(lineSplit1[2]));
//	G.setCapacidade(stoi(lineSplit1[3]));
//	G.setMaxRideTime(stoi(lineSplit1[4]));
//
//	//armazenando no grafo os dados lidos
//	for (int i = 1; i < fileslist.size(); i++) {
//		vector<string> lineSplit;
//		Node node;
//		string line = fileslist[i];
//
//		stringSplit(line, " ", lineSplit);
//
//		node.node = stoi(lineSplit[0]);
//		node.coordX = stod(lineSplit[1]);
//		node.coordY = stod(lineSplit[2]);
//		node.timeService = stoi(lineSplit[3]);
//		node.numPersons = stoi(lineSplit[4]);
//		node.twA = stod(lineSplit[5]);
//		node.twB = stod(lineSplit[6]);
//
//		G.addNode(node);
//	}
//}
