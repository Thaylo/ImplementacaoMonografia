/*
 * SA.cpp
 *
 *  Created on: Jul 6, 2014
 *      Author: thaylo
 */

using namespace std;

#include "SA.h"

int rand_linear(int min, int max)
{
	int w = max-min;
	return rand()%(w+1)+min;
}

// Gera um número aleatório entre 0 e 1.
double random_t()
{
	int r = rand_linear(0,1000000);
	return r / 1000000.0;
}



Solucao simulated_annealing(Instancia *inst, double T0, int max_per_per_iter, 
     int max_success_per_iter, double alfa_arref,double alfa, double beta, vizinhanca_sa vizinhanca)
{
	Solucao s;
	PoliticaDeAlocacao politica = averageRec;
	construcao_solucao(inst, s, alfa, beta, politica, 1);
	otimiza_recursos(s,alfa,beta);
	Solucao si;
	si = s;

	double T = T0;
	double Tf = 1e-10;
    // Sabendo a "temperatura" final desejada, o número maximo de iterações é conhecido.    
	int max_iter = log(Tf/T0)/log(alfa_arref);

	int nSuccess = 1;
	int j = 0;
	int i = 0;
	while(1)
	{
		i = 0;
		nSuccess = 0;
		while(1)
		{

			switch(vizinhanca)
			{
				case tipo_R:
				{
					si = gera_vizinho_tipoR(s,politica,alfa,beta);
					break;
				}
				case tipo_T:
				{
					si = gera_vizinho_tipoT(s,politica,alfa,beta);
					break;
				}
				case mista_aleatoria:
				{
					int rand_value = rand();
					
					if(rand_value == 0 || rand_value % 2 == 0)
						si = gera_vizinho_tipoR(s,politica,alfa,beta);
					else
						si = gera_vizinho_tipoT(s,politica,alfa,beta);
					break;
				}
			}
			otimiza_recursos(si,alfa,beta);

			//cout << "A partir de \n"<< s<<"foi gerado\n" << si << "\n";
			double current_eva = si.avaliaSolucao(alfa,beta);

			double DeltaFi = current_eva - s.avaliaSolucao(alfa,beta);
			double aux = exp(-DeltaFi/T);

			if(DeltaFi < 0.0 || aux >= random_t())
			{
				s = si;
				nSuccess++;
			}
			i++;
			if(nSuccess >= max_success_per_iter || i > max_per_per_iter)
			{
				break;
			}
		}
		T *= alfa_arref;
		j++;
		if(nSuccess == 0 || j >= max_iter)
		{
			break;
		}
	}
	return s;
}
