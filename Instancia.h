/*
 * Instancia.h
 *
 *  Created on: Jun 19, 2014
 *      Author: thaylo
 */

#ifndef INSTANCIA_H_
#define INSTANCIA_H_

#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <cmath>

#define max_tarefas 100
#define max_maquinas 20

typedef enum
{
	maxRec,
	minRec,
	averageRec
} PoliticaDeAlocacao;

namespace std {

class Instancia {
public:
	Instancia();
	Instancia(char *arquivo);
	void exibe();
	void lePij(FILE *fp);
	void leMatriz(FILE *fd,char *desc);
	double avaliaTempoDeSetupComRecurso(int SijkMax, int SijkMin, int RijkMax, int RijkMin, 
	                                                                             int RijkAtribuido);


	int quantTarefas();
	int quantMaquinas();
	int Pij(int i, int j);

	int SijkMax(int maq, int j,int k);
	int SijkMin(int maq, int j,int k);
	int RijkMax(int maq, int j,int k);
	int RijkMin(int maq, int j,int k);

	virtual ~Instancia();
private:
	int qtd_tarefas; // Numero de tarefas
	int qtd_maquinas; // Numero de maquinas
	int p[max_tarefas][max_maquinas];
	int RSSDMIN[max_maquinas][max_tarefas][max_tarefas];
	int RSSDMAX[max_maquinas][max_tarefas][max_tarefas];
	int SSDMIN[max_maquinas][max_tarefas][max_tarefas];
	int SSDMAX[max_maquinas][max_tarefas][max_tarefas];
};

} /* namespace std */


#endif /* INSTANCIA_H_ */
