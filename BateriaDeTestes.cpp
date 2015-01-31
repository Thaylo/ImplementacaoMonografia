/*
 * BateriaDeTestes.cpp
 *
 *  Created on: Aug 11, 2014
 *      Author: thaylo
 */

#include "BateriaDeTestes.h"
#define GRASP_ITER 1
#define GRASP_PRSA 2
#define GRASP_SA 3

namespace std {

void grasp_switch(Solucao *s, Instancia *inst, PoliticaDeAlocacao politica, double alfa, 
              double beta, int max_iter, double alfa_aleatoriedade, double *z_values, double *times, 
                                                               int values_size, int *qtd, int gtype)
{
	switch(gtype)
	{
	    case GRASP_ITER:
	    {
	        *s = grasp_iterado(inst, politica, alfa, beta, max_iter, alfa_aleatoriedade, z_values, 
	                                                                       times, values_size, qtd);
	    }
	    case GRASP_PRSA:
	    {
	        *s = grasp_com_PRSA(inst, politica, alfa, beta, max_iter, alfa_aleatoriedade, z_values, 
	                                                                       times, values_size, qtd);
	    }
	    case GRASP_SA:
	    {
	        *s = grasp_com_SA(inst, politica, alfa, beta, max_iter, alfa_aleatoriedade, z_values, 
	                                                                       times, values_size, qtd);
	    }
	}   	
}

BateriaDeTestes::BateriaDeTestes() {}


void BateriaDeTestes::run(int instance_size)
{
	runRefactored(mq, instance_size);
}

void runOneInstance(Queue<Task> *mq, int instance_size)
{
    while(true)
    {
        Task t = mq->pop();

        char nome_da_instancia[300];
        int current_best;
        int job_index;

        strcpy(nome_da_instancia,t.target);
        current_best = t.current_best;
        job_index = t.id;

        if(job_index == -1) 
        {
            mq->push(t);
            printf("pilha vazia\n");
            break;
        }

        int max_iter = 2000;
        double alfa = 1;
        double beta = 50;
        char target[300] = "";
        int rc = 10;

        static int sizeDump = 1000;
        double valuesITER[1000],timesITER[1000], sizeITER = sizeDump;
        double valuesPRSA[1000],timesPRSA[1000], sizePRSA = sizeDump;
        double valuesSA[1000],timesSA[1000], sizeSA = sizeDump;

        int i = job_index;

        char outputImage[20] = "";
        sprintf(outputImage,"image%d",i);

        sprintf(target,"%s",nome_da_instancia);
        strcat(target,(char*)".txt");
        if(rc == 0)
        {
            printf("Erro de leitura\n");
            exit(0);
        }
        
        char diretorioDasInstancias[200] = "";
        getCurrentFolder(diretorioDasInstancias, sizeof(diretorioDasInstancias));
        
        if(instance_size == SMALL_SIZE)
        {
            strcat(diretorioDasInstancias,"instances/small/");
        }
        else
        {
            strcat(diretorioDasInstancias,"instances/large/");
        }
        strcat(diretorioDasInstancias,target);
        
        Instancia inst(diretorioDasInstancias);
        
        //Solucao sdjasa = djasa(&inst,averageRec,alfa,beta);
        //double ava_djasa = sdjasa.avaliaSolucao(alfa,beta);

        double alfa_aleatoriedade = 0.8;
        int qtd_prsa, qtd_sa, qtd_iter;

        qtd_prsa = 0;
        qtd_sa = 0;
        qtd_iter = 0;

        Solucao giter;
        Solucao gprsa;
        Solucao gsa;
	
        grasp_switch (&giter, &inst, averageRec, alfa, beta, max_iter, alfa_aleatoriedade,   
                                            valuesITER, timesITER, sizeITER, &qtd_iter, GRASP_ITER);
                                            
        grasp_switch (&gprsa, &inst ,averageRec, alfa, beta, max_iter, alfa_aleatoriedade,   
                                            valuesPRSA, timesPRSA, sizePRSA, &qtd_prsa, GRASP_PRSA);
                                            
        grasp_switch (&gsa,   &inst ,averageRec, alfa, beta, max_iter, alfa_aleatoriedade,   
                                                valuesSA,   timesSA,   sizeSA,   &qtd_sa, GRASP_SA);

        dump_resultsb(instance_size, target, outputImage, valuesITER, valuesPRSA, valuesSA, 
                                          timesITER, timesPRSA, timesSA, qtd_iter, qtd_prsa, qtd_sa, 
                (char*)"GRASP com Busca Vizinhanca Local", (char*)"GRASP PRSA", (char*) "GRASP SA");
    }

}



void runRefactored(Queue<Task> &mq, int instance_size)
{
    Task t;
    t.id = -1;
    mq.push(t);

    int max_iter = 2000;
    double alfa = 1;
    double beta = 50;
    char target[300] = "";
    double current_best;
    int rc = 10;
    
    char diretorioDasInstancias[200] = "";
    getCurrentFolder(diretorioDasInstancias, sizeof(diretorioDasInstancias));
    
    if(instance_size == SMALL_SIZE)
    {
        strcat(diretorioDasInstancias,"instances/small/");
    }
    else
    {
        strcat(diretorioDasInstancias,"instances/large/");
    }
    strcat(diretorioDasInstancias,target);
    
    char instances_index[300] = "";
    char optimal_values_index[300] = "";
    
    strcpy(instances_index, diretorioDasInstancias);
    strcat(instances_index, (char*)"indice_de_instancias");

    strcpy(optimal_values_index, diretorioDasInstancias);
    strcat(optimal_values_index, (char*)"indice_de_otimos");

    FILE *nomes_instancias = fopen(instances_index,"r");
    if(!nomes_instancias) 
    {
        printf("Fail to open %s\n",instances_index);
    }
    FILE *otimos_das_instancias = fopen(optimal_values_index,"r");
    if(!otimos_das_instancias) 
    {
        printf("Fail to open %s\n",optimal_values_index);
    }

    int quantidade_nomes, quantidade_otimos;
    rc = fscanf(nomes_instancias,"%d",&quantidade_nomes);
    rc = fscanf(otimos_das_instancias,"%d",&quantidade_otimos);

    if(quantidade_nomes != quantidade_otimos)
    {
        printf("Erro, arquivos com tamanhos diferentes!!!\n");
        return;
    }
	

	//for(int i = 0; i < quantidade_nomes; i++)
	for(int i = 0; i < 4; i++)
	{
        char outputImage[20] = "";
        sprintf(outputImage,"image%d",i);

        rc = fscanf(nomes_instancias,"%s",target);
        if(rc == 0)
        {
            printf("Erro de leitura\n");
            exit(0);
        }
        rc = fscanf(otimos_das_instancias,"%lf",&current_best);
        if(rc == 0)
        {
            printf("Erro de leitura\n");
            exit(0);
        }

        Task t;
        t.id = i;
        strcpy(t.target,target);
        t.current_best = current_best;
        mq.push(t); 
	}
	
    std::thread myThreads[5];
    for(int i = 0; i < 5; i++)
    {
        myThreads[i] = std::thread(runOneInstance,&mq, instance_size);
    }

    for (int i = 0; i < 5; i++)
    {
        myThreads[i].join();
    }

    fclose(nomes_instancias);
    fclose(otimos_das_instancias);
}

BateriaDeTestes::~BateriaDeTestes() {
	// TODO Auto-generated destructor stub
}

} /* namespace std */
