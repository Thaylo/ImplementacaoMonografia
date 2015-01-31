/*
 * Djasa.h
 *
 *  Created on: Jun 25, 2014
 *      Author: thaylo
 */

#ifndef DJASA_H_
#define DJASA_H_

#include "Solucao.h"
#include <math.h>

namespace std {

Solucao djasa(Instancia *inst, PoliticaDeAlocacao politica, double alfa, double beta);

} /* namespace std */

#endif /* DJASA_H_ */
