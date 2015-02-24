/*
 * SA.h
 *
 *  Created on: Jul 6, 2014
 *      Author: thaylo
 */

#ifndef SA_H_
#define SA_H_

using namespace std;

#include "Instancia.h"
#include "Djasa.h"
#include <math.h>

typedef enum
{
	tipo_R,
	tipo_T,
	mista_aleatoria
} vizinhanca_sa;

Solucao simulated_annealing(Instancia *inst, double T0, int max_per_per_iter, 
    int max_success_per_iter, double alfa_arref,double alfa, double beta, vizinhanca_sa vizinhanca);
    
#endif /* SA_H_ */
