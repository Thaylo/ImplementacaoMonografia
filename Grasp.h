/*
 * Grasp.h
 *
 *  Created on: Jun 27, 2014
 *      Author: thaylo
 */

#ifndef GRASP_H_
#define GRASP_H_

#include "Instancia.h"
#include <iostream>
#include "Solucao.h"
#include <cmath>
#include <algorithm>
#include <stdlib.h>

#include <vector>
#include <list>
#include <cctype>
#include "SA.h"

#include <unistd.h>


using namespace std;


namespace std {


#define SMALL_SIZE 1
#define LARGE_SIZE 2


void getCurrentFolder(char *bufferOut, int sizeBufferOut);
void dump_resultsb(int instance_size, char *instance_name, char *output, double values1[], 
              double values2[], double values3[], double times1[], double times2[], double times3[], 
    unsigned size1, unsigned size2, unsigned size3, char* legenda1, char* legenda2, char *legenda3);

#if 0
Solucao grasp_recursivo(Instancia *inst, PoliticaDeAlocacao politica, double alfa, double beta, 
          int max_iter, double alfa_aleatoriedade, double *z_values, double *times, int values_size, 
                                                                                          int &qtd);
#endif

Solucao grasp_iterado(Instancia *inst, PoliticaDeAlocacao politica, double alfa, double beta, 
          int max_iter, double alfa_aleatoriedade, double *z_values, double *times, int values_size, 
                                                                                          int *qtd);
                                                                                          
Solucao grasp_com_SA(Instancia *inst, PoliticaDeAlocacao politica, double alfa, double beta, 
          int max_iter, double alfa_aleatoriedade, double *z_values, double *times, int values_size, 
                                                                                          int *qtd);
                                                                                          
Solucao grasp_com_PRSA(Instancia *inst, PoliticaDeAlocacao politica, double alfa, double beta, 
          int max_iter, double alfa_aleatoriedade, double *z_values, double *times, int values_size, 
                                                                                          int *qtd);

} // End namespace
#endif /* GRASP_H_ */
