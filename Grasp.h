/*
 * Grasp.h
 *
 *  Created on: Jun 27, 2014
 *      Author: thaylo
 */

#ifndef GRASP_H_
#define GRASP_H_

#include "Instancia.h"
#include "TimeSeries.h"
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

typedef enum{
    bvt,
    sa,
    prsa
} grasp_config;

#define SMALL_SIZE 1
#define LARGE_SIZE 2


void getCurrentFolder(char *bufferOut, int sizeBufferOut);

void dump_resultsb(int instance_size, char *instance_name, char *output, 
                             list<Sample> *samples1, list<Sample> *samples2, list<Sample> *samples3,
                                                    char* legenda1, char* legenda2, char *legenda3);

Solucao grasp_with_setings(Instancia *inst, PoliticaDeAlocacao politica, double alfa, double beta, 
                  int max_iter, double alfa_aleatoriedade, list<Sample> *samples, grasp_config set);

void dump_results_structured(int instance_size, char *instance_name, char *output, 
                             list<Sample> &samples1, list<Sample> &samples2, list<Sample> &samples3,
                                                    char* legenda1, char* legenda2, char *legenda3,
                                                                              int bestOfLiterature);

} // End namespace
#endif /* GRASP_H_ */
