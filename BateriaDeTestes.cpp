/*
 * BateriaDeTestes.cpp
 *
 *  Created on: Aug 11, 2014
 *      Author: thaylo
 */

#include "BateriaDeTestes.h"

namespace std {

BateriaDeTestes::BateriaDeTestes() {}

static double evaluateAtTime(list<Sample> &ls, double time)
{
    double ret = 0;

    if ( time <= ls.front().time )
    {
        return ls.front().evaluation;
    }
    else if ( time >= ls.back().time )
    {
        return ls.back().evaluation;
    }
    else
    {
        for (Sample s : ls)
        {
            if ( time >= s.time )
            {
                ret = s.evaluation;
            }
            else
            {
                break;
            }
        }
    }
    return ret;
}

static list<Sample> sumSamples(list<Sample> &A, list<Sample> &B)
{
    list<Sample> auxiliar;
    list<Sample> final;

    double minTime = min(A.front().time, B.front().time);
    double maxTime = max(A.back().time, B.back().time);

    double currentTime = -1.0;
    double oldTime = -1.0;
    
    list<Sample>::iterator ita = A.begin();
    list<Sample>::iterator itb = B.begin();

    while(ita != A.end() || itb != B.end() )
    {
        if (ita == A.end() )
        {
            auxiliar.push_back(*itb);
            ++itb;
        }
        else if (itb == B.end())
        {
            auxiliar.push_back(*ita);
            ++ita;
        }
        else
        {
            if( ita->time < itb->time )    
            {
                auxiliar.push_back(*ita);
                ++ita;
            }
            else
            {
                auxiliar.push_back(*itb);
                ++itb;
            }
        }
    }

    list<Sample>::iterator itaux = auxiliar.begin();
    while( itaux->time < minTime )
    {
        ++itaux;
    }

    while(itaux != auxiliar.end() )
    {
        currentTime = itaux->time;
        if(itaux->time > maxTime)
        {
            break;
        }
        if ( currentTime != oldTime )
        {
            Sample s;
            s.time = itaux->time;
            s.evaluation = evaluateAtTime(A, currentTime) + evaluateAtTime(B, currentTime);
            final.push_back(s);
            oldTime = currentTime;
        }
        ++itaux;
    }
    return final;
}

static void multiplyByConstant(list<Sample> &ls, double constant)
{
    for(list<Sample>::iterator it = ls.begin(); it != ls.end(); ++it)
    {
        it->evaluation *= constant;
    }
}

void computeMeans(Summary *generalSummary);

static void runOneInstance(Queue<Task> *mq, Summary *generalSummary, int instance_size)
{
    Task t;
    const int repetitionToComputeMeans = 20;
    Summary localSummary;
    while(mq->ifhaspop(t))
    {
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

        for( int count = 0; count < repetitionToComputeMeans; ++count)
        {
            list<Sample> samplesBvt;
            list<Sample> samplesPrsa;
            list<Sample> samplesSa;
            
            if (instance_size != LARGE_SIZE)
            {
                (void) grasp_with_setings(&inst, averageRec, alfa, beta, max_iter, 
                                                              alfa_aleatoriedade, &samplesBvt, bvt);
            }
            (void) grasp_with_setings(&inst, averageRec, alfa, beta, max_iter, alfa_aleatoriedade, 
                                                                                &samplesPrsa, prsa);

            (void) grasp_with_setings(&inst, averageRec, alfa, beta, max_iter, alfa_aleatoriedade, 
                                                                                    &samplesSa, sa);

            if (instance_size != LARGE_SIZE)
            {
                localSummary.bvt.push(samplesBvt);
            }
            localSummary.prsa.push(samplesPrsa);
            localSummary.sa.push(samplesSa);

        }
        computeMeans(&localSummary);
        dump_results_structured(instance_size, target, outputImage, 
                          localSummary.meanBvt, localSummary.meanPrsa, localSummary.meanSa, 
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

    fclose(nomes_instancias);
    fclose(otimos_das_instancias);
}

void BateriaDeTestes::run(int instance_size)
{
	runRefactored(mq, generalSummary, instance_size);
}

static int evaluateSequence(list<Sample> &seq, double time)
{
    if(time < seq.begin()->time) 
    {
        return seq.begin()->evaluation;
    }
    else
    {
        for(Sample s : seq)
        {
            if(s.time >= time)
            {
                return s.evaluation;
            }
        }
        return seq.end()->evaluation;
    }
}   

static void computeMaxMin(list< list <Sample> > &set, double *start, double *end)
{
    double tini = -1;
    double tfim = -1;
        
    for(list<Sample> lss: set)
    {
        if ( -1 == tini )
        {
            tini = lss.begin()->time;
        }
        else if ( lss.begin()->time > tini )
        {
            tini = lss.begin()->time;
        }

        if ( -1 == tfim )
        {
            tfim = lss.back().time;
        }
        else if ( lss.back().time < tfim )
        {
            tfim = lss.back().time;
        }
    }
    *end = tfim;
    *start = tini;
}

static void fillVectorWithEvaluations(list<Sample> &s, double *v, int szv,
                                                                         double tstart, double tend)
{
    int i = 0;
    for( double t = tstart; t <= tend; ) // TODO FIXME. t pode passar de tend antes da hora.
    {
        v[i] = evaluateSequence(s, t);
        ++i;
        t += (tend - tstart)/(szv-1);
    }
}

static void computeMean(Queue<list<Sample>> &set, list<Sample> &sampleMean)
{
    const int numberOfSamples = 1000;
    double values[numberOfSamples] = {0};
    double means[numberOfSamples] = {0};

    double tstart, tend;

    list<list<Sample>> set_cpy;
    list<Sample> aux;
    list<Sample> processedMean;

    while(set.ifhaspop(aux))
    {
        set_cpy.push_back(aux);
    }
    
    int cnt = 0;

    for( list<Sample> s : set_cpy)
    {
        ++cnt;
        if ( processedMean.size() == 0 )
        {
            processedMean = s;
        }
        else
        {
            processedMean = sumSamples(processedMean, s);
        }
    }
    multiplyByConstant(processedMean, 1/(double)cnt);
    sampleMean = processedMean;
}

void computeMeans(Summary *generalSummary)
{
    computeMean(generalSummary->bvt, generalSummary->meanBvt);
    computeMean(generalSummary->prsa, generalSummary->meanPrsa);
    computeMean(generalSummary->sa, generalSummary->meanSa);
}





BateriaDeTestes::~BateriaDeTestes() {
}

} /* namespace std */
