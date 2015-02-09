/*
 * Solucao.h
 *
 *  Created on: Jun 24, 2014
 *      Author: thaylo
 */

#ifndef SOLUCAO_H_
#define SOLUCAO_H_

#define ZINF INT_MAX
#include "Instancia.h"
#include "dummy_PR.h"
#include <list>
#include <limits.h>
#include <assert.h>

namespace std {

class Job
{
public:
	int id;
	int maq;
	int rec;
	double incremento;
	Job()
	{
		rec = 0;
		maq = -1;
		id = -1;
		incremento = 99999;
	}
	Job(int i)
	{
		rec = 0;
		maq = -1;
		id = i;
		incremento = 99999;
	}
	bool operator<(const Job& a) {
		return this->incremento < a.incremento;
	}
	bool operator>(const Job& a) {
		return this->incremento > a.incremento;
	}
	bool operator==(const Job& a) {
		return this->incremento == a.incremento;
	}
	Job& operator=(const Job& a) {
		this->incremento = a.incremento;
		this->maq = a.maq;
		this->id = a.id;
		this->rec = a.rec;
		return *this;

	}
	int get_id()
	{
		return id;
	}
	void set_inc(double value)
	{
		incremento = value;
	}
	void exibe()
	{
		std::cout << id;
		std::cout << " " << incremento << ", ";
	}
	~Job()
	{

	}
	friend ostream& operator<<(ostream &out, Job &s);
};



class Solucao {
public:
    // TODO tornar maquinas private.
	list<Job> *maquinas[max_maquinas]; 

	Solucao();
	Solucao(Instancia *i);
	Solucao(const Solucao &s);

	void bind(Instancia *i);
	void atribuiTarefa(int tarefa, int rec, int maquina);
	int tarefasProgramadas();
	void retiraTarefa(int tarefa, int maquina);
	void incrementaNumeroTarefas();
	void exibeSolucao();

	int quantidadeMaquinas();
	int quantidadeTarefas();
	int tarefasAtribuidasNaMaquina(int j);
	int posicaoNaMaquina(int tarefa, int maquina);
	bool jaEscalonada(int id);
	int antecessoraNaMaquina(int tarefa,int maquina);
	int quantidadeDeSucessorasNaMaquina(int tarefa, int maquina);
	int ultimaAtribuidaNaMaquina(int j);
	Instancia *acessaInstancia();
	int insereNaMelhorPosicao(int tarefa, PoliticaDeAlocacao politica, double alfa, double beta);



	double avaliaSolucao(double alfa, double beta);
	friend ostream& operator<< (ostream &out, Solucao &s);
	friend bool operator==(Solucao& a, Solucao& b);

	Solucao& operator=(const Solucao &x);
	virtual ~Solucao();
	Instancia *inst;
private:
	int quantidade_maquinas;
	int quantidade_tarefas;

};
void alimenta_pool(list<Solucao> &pool,Solucao &s, double alfa, double beta);

void exibe_pool(list<Solucao> & pool);

Solucao path_relinking(list<Solucao> & pool, double alfa, double beta);

Solucao swap_jobs(Solucao &s, int maq1, int job1, int maq2, int job2);

Solucao busca_local(Solucao &s, PoliticaDeAlocacao politica, double alfa, double beta);

Solucao gera_vizinho_tipoN(Solucao &s, PoliticaDeAlocacao politica, double alfa, double beta);

Solucao gera_vizinho_tipoT(Solucao &s, PoliticaDeAlocacao politica, double alfa, double beta);

Solucao busca_local_recursiva(Solucao &s, PoliticaDeAlocacao politica, double alfa, double beta);

Solucao busca_local_iterada(Solucao &s, PoliticaDeAlocacao politica, double alfa, double beta);

void construcao_solucao(Instancia *inst, Solucao &sequencia, double alfa, double beta, 
                                            PoliticaDeAlocacao politica, double alfa_aleatoriedade);
                                            
void insere_job_solucao(Solucao &aux,Job &job_it, int maq, int pos,double alfa, double beta);

void construcao_solucao(Instancia *inst, Solucao &sequencia, double alfa, double beta, 
                                            PoliticaDeAlocacao politica, double alfa_aleatoriedade);
                                            
void otimiza_recursos(Solucao &sequencia, double alfa, double beta);

} /* namespace std */

#endif /* SOLUCAO_H_ */
