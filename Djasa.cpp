/*
 * Djasa.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: thaylo
 */

#include "Djasa.h"

namespace std {


Solucao djasa(Instancia *inst, PoliticaDeAlocacao politica, double alfa, double beta)
{
	Solucao construida;
	double avaliacao_corrente = 0;
	double menor_incremento = 999999999;
	int melhor_tarefa = -1;
	int melhor_maquina = -1;

	construida.bind(inst);
	list<int> unescheduled_jobs; // Começa vazia.

	for(int i = 0; i < inst->quantTarefas(); i++)
	{
		unescheduled_jobs.push_back(i);
	} // ENCHI

	while(unescheduled_jobs.size() > 0)
	{
		for(list<int>::iterator j = unescheduled_jobs.begin(); j != unescheduled_jobs.end(); j++)
		{
			for(int i = 0; i < construida.quantidadeMaquinas(); i++)
			{

				switch(politica)
				{
					case maxRec:
					{
						construida.atribuiTarefa(
								*j,
								construida.acessaInstancia()->RijkMax(i,
								                         construida.ultimaAtribuidaNaMaquina(i),*j),
								i
								);
						break;
					}
					case minRec:
					{
						construida.atribuiTarefa(
								*j,
								construida.acessaInstancia()->RijkMin(i,
								                         construida.ultimaAtribuidaNaMaquina(i),*j),
								i
								);
						break;
					}
					case averageRec:
					{

						construida.atribuiTarefa(
								*j,
								round(
										0.5*(construida.acessaInstancia()->RijkMin(i,
										                 construida.ultimaAtribuidaNaMaquina(i),*j)+
										construida.acessaInstancia()->RijkMax(i,
										                 construida.ultimaAtribuidaNaMaquina(i),*j))
										),

								i
								);
						break;
					}
				}


				double incremento = construida.avaliaSolucao(alfa,beta) - avaliacao_corrente;
				if(incremento < menor_incremento)
				{
					melhor_tarefa = *j;
					melhor_maquina = i;
					menor_incremento = incremento;
				}
				construida.retiraTarefa(*j,i);
			}
		}

		menor_incremento = 999999999;

		#if 0
		cout << "tarefa " << melhor_tarefa << ", maquina " << melhor_maquina <<" recurso: "  
		<< construida.acessaInstancia()->RijkMax(melhor_maquina,
		             construida.ultimaAtribuidaNaMaquina(melhor_maquina),melhor_tarefa) << "\n";
        #endif
        
		construida.atribuiTarefa(
					melhor_tarefa,
					construida.acessaInstancia()->RijkMax(melhor_maquina,
					             construida.ultimaAtribuidaNaMaquina(melhor_maquina),melhor_tarefa),
					melhor_maquina
					);

		for(list<int>::iterator j = unescheduled_jobs.begin(); j != unescheduled_jobs.end(); j++)
		{
			if (*j == melhor_tarefa)
			{
				unescheduled_jobs.erase(j);
				break;
			}
		}
	}
	otimiza_recursos(construida,alfa,beta);
	return construida;
}


} /* namespace std */
