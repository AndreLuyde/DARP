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
void checkSolution();
void readSolution(string arquivo);
void readSolution2(string arquivo);

int main() {

	Graph G;
	Guloso guloso;

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
	calculaDistancias(G);

	readSolution2("teste");
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
			pow((node2.coordX - node2.coordX), 2)
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

void checkSolution(vector<Car> solution, Graph problem) {
	double rideTime = 0;
	vector<Node> car;
	bool avaliable = true;
	int numPeople = 0;

	//checkar janelas de tempo
	for (int i = 0; i < solution.size(); i++) {
		car = solution[i].getRoute();
		rideTime = car[1].twA;
		for (int j = 1; j < car.size(); j++) {
			rideTime += car[j].listDist[car[j + 1].node];
			if (rideTime < car[j].twA && rideTime > car[j].twB) {
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
			if(numPeople > problem.getCapacidade()){
				avaliable = false;
			}
		}

	}
}

void readSolution(string arquivo) {
	Car car;

	//Leitura do arquivo de resposta
	ifstream readlist;
	ofstream resultList;
	vector<string> fileslist;
	string lineRead;
	arquivo = "/home/andre/Documentos/DARP/Solution/pr02.res.txt";
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
