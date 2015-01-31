/*
 * dummy_PR.cpp
 *
 *  Created on: Aug 11, 2014
 *      Author: thaylo
 */
 
#include "dummy_PR.h"

Solucao_dummy *alocaSolucao_dummy(int n)
{
	Solucao_dummy *s = (Solucao_dummy*) malloc (sizeof(Solucao_dummy));
	s->size = n;
	return s;
}

Solucao_dummy *desalocaSolucao_dummy(Solucao_dummy *s)
{
	if(s == NULL) return NULL;
	free(s);
	return NULL;
}

void exibeSolucao_dummy(Solucao_dummy *s)
{
	printf("Size = %d: [",s->size);
	for(int i = 0; i < s->size; i++)
	{
		printf("%d",s->data[i]);
		if(i != s->size - 1) printf(" ");
	}
	printf("]\n");
}

void swap(int *a, int *b)
{
	if(a == b || a == NULL || b == NULL) return;
	int temp = *a;
	*a = *b;
	*b = temp;
}

void swapSolucao_dummy(Solucao_dummy *s, int i, int j)
{
	int *a = (s->data)+i;
	int *b = (s->data)+j;
	swap(a,b);
}


Solucao_dummy *geraVizinha(Solucao_dummy *original, int i, int j)
{
	Solucao_dummy *modificada = alocaSolucao_dummy(original->size);
	*modificada = *original;
	swapSolucao_dummy(modificada, i, j);
	return modificada;
}

int find(Solucao_dummy *dest, Solucao_dummy *src, int value)
{
	for(int k = 0; k < dest->size; k++)
	if(dest->data[k] == value)
	{
		if(src->data[k] == value) continue;
		return k;
	}
	return -1;
}

// Retorna 1 para iguais, 0 para diferentes.
int comparaSolucoes(Solucao_dummy *a, Solucao_dummy *b)
{
	if(a->size != b->size) return 0;
	for(int i = 0; i < a->size; i++)
	{
		if(a->data[i] != b->data[i])
		return 0;
	}
	return 1;
}

void checkSizes(Solucao_dummy *a, Solucao_dummy *b)
{
	if(a->size != b->size)
	{
		printf("Tamanhos dest %d e src %d diferentes\n",a->size,b->size);
		exit(0);
	}
}



