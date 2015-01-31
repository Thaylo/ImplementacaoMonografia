/*
 * BateriaDeTestes.h
 *
 *  Created on: Aug 11, 2014
 *      Author: thaylo
 */

#ifndef BATERIADETESTES_H_
#define BATERIADETESTES_H_

#include <iostream>
#include <string>

#ifdef _threading_now
    #include <thread>
#endif /* thread */

#include "Instancia.h"
#include "Solucao.h"
#include "Djasa.h"
#include "Grasp.h"
#include "SA.h"
#include <string>
#include <stdio.h>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include "Queue.h"
namespace std {

typedef struct
{

    char target[200];
    int current_best;
    int id;
} Task;

class BateriaDeTestes {
private:

public:
    Queue<Task> mq;
	BateriaDeTestes();
	
	void run(int instance_size);
	virtual ~BateriaDeTestes();
};

void runRefactored(Queue<Task> &mq, int instance_size);
void grasp_switch(Solucao *s, Instancia *inst, PoliticaDeAlocacao politica, double alfa, 
              double beta, int max_iter, double alfa_aleatoriedade, double *z_values, double *times, 
                                                              int values_size, int *qtd, int gtype);


} /* namespace std */

#endif /* BATERIADETESTES_H_ */
