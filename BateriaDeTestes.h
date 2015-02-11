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

#include "TimeSeries.h"
#include "Instancia.h"
#include "Solucao.h"
#include "Djasa.h"
#include "Grasp.h"
#include "SA.h"
#include <string>
#include <stdio.h>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include "Queue.h"
#include "AsyncIO.h"

namespace std {

typedef struct
{
    char target[200];
    int current_best;
    int id;
} Task;

typedef struct
{
    Queue<list<Sample>> bvt;
    Queue<list<Sample>> prsa;
    Queue<list<Sample>> sa;
    
    list<Sample> meanBvt;
    list<Sample> meanPrsa;
    list<Sample> meanSa;
} Summary;

class BateriaDeTestes {
private:

public:
    Queue<Task> mq;

    Summary generalSummary;

	BateriaDeTestes();
	
	void run(int instance_size, int repetitions, int iterations);
    void computeMeans();
	virtual ~BateriaDeTestes();
};

} /* namespace std */

#endif /* BATERIADETESTES_H_ */
