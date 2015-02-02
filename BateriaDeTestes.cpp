/*
 * BateriaDeTestes.cpp
 *
 *  Created on: Aug 11, 2014
 *      Author: thaylo
 */

#include "BateriaDeTestes.h"

namespace std {

BateriaDeTestes::BateriaDeTestes() {}

static void runOneInstance(Queue<Task> *mq, Summary *generalSummary, int instance_size)
{
    while(!mq->empty())
    {
        Task t;
        
        bool result = mq->ifhaspop(t);
        
        if(false == result)
        {
            break;
        }
        
        char nome_da_instancia[300];
        
        int job_index;

        strcpy(nome_da_instancia,t.target);
        job_index = t.id;

        int max_iter = 50;
        double alfa = 1;
        double beta = 50;
        char target[300] = "";
        int rc = 10;

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

        Solucao giter;
        Solucao gprsa;
        Solucao gsa;

        list<Sample> samplesBvt;
        list<Sample> samplesPrsa;
        list<Sample> samplesSa;
    
        giter = grasp_with_setings(&inst, averageRec, alfa, beta, max_iter, alfa_aleatoriedade, 
                                                                                  &samplesBvt, bvt);

        gprsa = grasp_with_setings(&inst, averageRec, alfa, beta, max_iter, alfa_aleatoriedade, 
                                                                                &samplesPrsa, prsa);

        gsa = grasp_with_setings(&inst, averageRec, alfa, beta, max_iter, alfa_aleatoriedade, 
                                                                                    &samplesSa, sa);

        generalSummary->bvt.push(samplesBvt);
        generalSummary->prsa.push(samplesPrsa);
        generalSummary->sa.push(samplesSa);

        dump_results_structured(instance_size, target, outputImage,
                                                                 samplesBvt, samplesPrsa, samplesSa, 
                         (char*)"Vizinhanca simples por trocas 2 a 2", (char*)"PRSA", (char*) "SA");
    }

    printf("Job done\n");
}

static void runRefactored(Queue<Task> &mq, Summary &generalSummary, int instance_size)
{

    static unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
    
    char target[300] = "";
    double current_best;
    int rc = 0;
    
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
    

    for(int i = 0; i < quantidade_nomes; i++)
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
    
    list<thread> myThreads;
    
    for(int i = 0; i < (int) concurentThreadsSupported; i++)
    {
        myThreads.push_back(thread(runOneInstance,&mq, &generalSummary, instance_size));
    }

    for (list<thread>::iterator it = myThreads.begin(); it != myThreads.end(); ++it)
    {
        it->join();
    }

    /* Here we must compute the means and display it*/
    #if 0
    void dump_results_structured(instance_size, (char *)"", (char *)"MEANS.m", 
                             list<Sample> &samples1, list<Sample> &samples2, list<Sample> &samples3,
                                                     char* legenda1, char* legenda2, char *legenda3)
    #endif
    fclose(nomes_instancias);
    fclose(otimos_das_instancias);
}

void BateriaDeTestes::run(int instance_size)
{
	runRefactored(mq, generalSummary, instance_size);
}







BateriaDeTestes::~BateriaDeTestes() {
}

} /* namespace std */
