/*
 * dummy_PR.h
 *
 *  Created on: Aug 11, 2014
 *      Author: thaylo
 */

#ifndef DUMMY_PR_H_
#define DUMMY_PR_H_


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_JOBS 10000
typedef struct
{
	int data[MAX_JOBS];
	int size;
} Solucao_dummy;


Solucao_dummy *alocaSolucao_dummy(int n);

Solucao_dummy *desalocaSolucao_dummy(Solucao_dummy *s);

void exibeSolucao_dummy(Solucao_dummy *s);

void swap(int *a, int *b);

void swapSolucao_dummy(Solucao_dummy *s, int i, int j);


Solucao_dummy *geraVizinha(Solucao_dummy *original, int i, int j);

int find(Solucao_dummy *dest, Solucao_dummy *src, int value);

// Retorna 1 para iguais, 0 para diferentes.
int comparaSolucoes(Solucao_dummy *a, Solucao_dummy *b);

void checkSizes(Solucao_dummy *a, Solucao_dummy *b);







#endif /* DUMMY_PR_H_ */
