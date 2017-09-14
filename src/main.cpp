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
#include "Guloso.h"
#include "Car.h"

using namespace std;

double distanciaEntrePontos(Node node1, Node node2);
void stringSplit(string str, string delim, vector<string>& results);
void calculaDistancias(Graph &g);
vector<Car> readSolution(string arquivo, Graph problem);
void readSolution2(string arquivo);
bool checkSolution(vector<Car> solution, Graph problem);
bool checkTimeRideClient(int position, vector<Node> car, int numRequest,
		double maxRideTimeClient);

int main() {
	Graph G;

	Guloso guloso;

	//Leitura do arquivo e Armazenamento do Grafo
	ifstream readlist;
	ofstream resultList;
	vector<string> fileslist;
	string lineRead;

	readlist.open("/home/andre/Documentos/DARP/Tabu/Instancias/pr05.txt");

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
	calculaDistancias(G);
	//faz leitura da solução
	vector<Car> solution = readSolution("teste", G);
	//verifica se a solução é valida
	if (checkSolution(solution, G)) {
		cout << "Solução Válida!" << endl;
	} else {
		cout << "Solução Inválida!" << endl;
	}
	//método guloso
	guloso.run(G);
//	metodoGulosoSimples(G);

//	cout << "Hello world!" << endl;
	return 0;
}
//calcula todas as distancias do grafo
void calculaDistancias(Graph &g) {
	vector<Node> nodos = g.getNodos();
	int sizeNodos = g.getNodos().size();
	for (int i = 0; i < sizeNodos; i++) {
		for (int j = 0; j < sizeNodos; j++) {
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
			pow((node2.coordX - node1.coordX), 2)
					+ pow((node2.coordY - node1.coordY), 2));
	return distance;
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

bool checkSolution(vector<Car> solution, Graph problem) {
	double rideTime = 0;
	vector<Node> car;
	bool avaliable = true;
	int numPeople = 0;

	//checkar janelas de tempo
	for (int i = 0; i < solution.size(); i++) {
		car = solution[i].getRoute();
		rideTime = car[1].twA;
		for (int j = 1; j < car.size()-1; j++) {
			rideTime += car[j].listDist[car[j + 1].node]; //atualiza o tempo percorrido
			if (rideTime < car[j].twA) { //se for menor que a janela de tempo, o motorista espera e o tempo passa a ser a janela
				rideTime = car[j].twA;
			}
			if (rideTime > car[j].twB) { //verifica se está dentro da janela de tempo
				cout << "Viola Restrição de janela de tempo!" << endl;
				cout << "Tempo de descida: " << rideTime << ". Janela do requisitante: " << car[j].twA << " e " << car[j].twB << endl;
				cout << "=====================================================================" << endl;
				avaliable = false;
			}
		}
		car.clear();
	}

	//checkar quantidade de pessoas no carro
	for (int i = 0; i < solution.size(); i++) {
		car = solution[i].getRoute();
		for (int j = 1; j < car.size(); j++) {
			numPeople += car[j].numPersons;
			if (numPeople > problem.getCapacidade()) { //carro nunca deve exceder a capacidade
				cout << "Viola Restrição de Capacidade do Carro!" << endl;
				cout << "Numero de pessoas: " << numPeople << ". Capacidade: " << problem.getCapacidade() << endl;
				cout << "=====================================================================" << endl;
				avaliable = false;
			}
		}
		if (numPeople != 0) {	//no fim de cada rota o carro deve estar vazio
			cout << "Carro: " << i+1 << endl;
			cout << "Viola Restrição de Embarque e Desembarque!" << endl;
			cout << "Numero de pessoas sem desembarcar: " << numPeople << endl;
			cout << "=====================================================================" << endl;
			avaliable = false;
		}
		car.clear();
	}

	//verificar tempo de cada passageiro
	for (int i = 0; i < solution.size(); i++) {
		car = solution[i].getRoute();
		int size = (car.size() - 2) / 2;
		int count = 1;
		while (count < size) {
			for (int k = 0; k < car.size(); k++) {
				if (car[k].numPersons > 0) {
					if (!checkTimeRideClient(k, car, problem.getNumRequisicoes()/2, problem.getMaxRideTime())) {
						avaliable = false;
					}
					count++;
				}
			}
		}
		car.clear();
	}

	//verificar a duração máxima da rota
	for (int i = 0; i < solution.size(); i++) {
		car = solution[i].getRoute();
		rideTime = car[0].twA;
		for (int j = 0; j < car.size()-1; j++) {
			rideTime += car[j].listDist[car[j + 1].node]; //atualiza o tempo percorrido
			if (rideTime < car[j].twA) { //se for menor que a janela de tempo, o motorista espera e o tempo passa a ser a janela
				rideTime = car[j].twA;
			}
		}
		if (rideTime > problem.getMaxDuration()) {
			cout << "Viola Restrição de tempo de duração da rota!" << endl;
			cout << "Tempo da rota: " << rideTime << ". Tempo Máximo: " << problem.getMaxDuration() << endl;
			cout << "=====================================================================" << endl;
			avaliable = false;
		}
		car.clear();
	}
	return avaliable;
}
//verifica se não excede o tempo máximo de permanencia do cliente no veículo
bool checkTimeRideClient(int position, vector<Node> car, int numRequest,
		double maxRideTimeClient) {
	bool avaliable = true;
	double timeRideClient = 0;
	for (int i = position; i < car.size()-1; i++) {
		if (car[i].node == car[numRequest].node) {
			break;
		}
		timeRideClient += car[i].listDist[car[i + 1].node];
	}
	if (timeRideClient <= maxRideTimeClient) {
		return true;	//se não viola o tempo
	} else {
		cout << "Viola Restrição de tempo de permanencia de usuário no veículo!" << endl;
		cout << "Cliente: " << car[position].node << endl;
		cout << "Tempo de permanência: " << timeRideClient << ". Tempo Máximo: " << maxRideTimeClient << endl;
		cout << "=====================================================================" << endl;
		return false;	//se viola o tempo
	}
}
//padrão de leitura das instâncias Cordeau resolvidos com busca tabu
vector<Car> readSolution(string arquivo, Graph problem) {
	Car car;
	vector<Car> solution;
	vector<Node> route;

	//Leitura do arquivo de resposta
	ifstream readlist;
	ofstream resultList;
	vector<string> fileslist;
	string lineRead;
	arquivo = "/home/andre/Documentos/DARP/Solution/pr05.res.txt";
	readlist.open(arquivo);

	while (getline(readlist, lineRead)) {
		stringstream ss;
		ss << lineRead;
		fileslist.push_back(ss.str());
	}

	readlist.close();

	//descobrir o que é o número da primeira linha
	vector<string> lineSplit1;
	stringSplit(fileslist[0], " ", lineSplit1);

	//armazenando no grafo os dados lidos
	for (int i = 1; i < fileslist.size(); i++) {
		if (fileslist[i] != "" && fileslist[i].front() != 'T') {
			vector<string> lineSplit;
			string line = fileslist[i];
			stringSplit(line, " ", lineSplit);
			car.setDistance(stod(lineSplit[2]));

			//monta o carro
			for (int j = 1; j < lineSplit.size() - 1; j++) {
				if (lineSplit[j + 1].front() == '(') {
					route.push_back(problem.getNodos()[stoi(lineSplit[j])]);
				}
			}
			car.setRoute(route);
			solution.push_back(car); //coloca o carro na solução
			route.clear();
		}
	}
	return solution;
}

void readSolution2(string arquivo) {
	Car car;
	double totalCost = 0.0;

	//Leitura do arquivo de resposta
	ifstream readlist;
	ofstream resultList;
	vector<string> fileslist;
	string lineRead;
	arquivo =
			"/home/andre/Documentos/DARP/BCroutingSolutionsI/a2-16hetIUY-0.res.txt";
	readlist.open(arquivo);

	while (getline(readlist, lineRead)) {
		stringstream ss;
		ss << lineRead;
		fileslist.push_back(ss.str());
	}

	readlist.close();

	//descobrir o que é o número da primeira linha
	vector<string> lineSplit1;
	stringSplit(fileslist[0], " ", lineSplit1);
	totalCost = stod(lineSplit1[2]);
	//armazenando no grafo os dados lidos
	for (int i = 1; i < fileslist.size(); i++) {
		if (fileslist[i] != "") {
			vector<string> lineSplit;
			Node node;
			string line = fileslist[i];

			stringSplit(line, " ", lineSplit);

			car.setDistance(stod(lineSplit[2]));

			node.node = stoi(lineSplit[0]);
			node.coordX = stod(lineSplit[1]);
			node.coordY = stod(lineSplit[2]);
			node.timeService = stoi(lineSplit[3]);
			node.numPersons = stoi(lineSplit[4]);
			node.twA = stod(lineSplit[5]);
			node.twB = stod(lineSplit[6]);
		}

	}
}
