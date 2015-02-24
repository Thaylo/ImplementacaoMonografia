/*
 * Solucao.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: thaylo
 */

#include "Solucao.h"

namespace std {

Solucao::Solucao() {
	quantidade_maquinas = quantidade_tarefas = 0;
	inst = NULL;
}

Solucao::Solucao(Instancia *i) {
	if(!i)
	{
		cout << "Instancia inexistente.\n";
		return;
	}
	inst = i;
	quantidade_maquinas = i->quantMaquinas();
	quantidade_tarefas = 0;
	for(int i = 0; i < quantidade_maquinas; i++)
	{
		maquinas[i] = new list<Job>;
	}
}

Solucao::Solucao(const Solucao& a)
{
	if(this == &a)
	{
		return;
	}
	else
	{
		inst = a.inst;
		quantidade_maquinas = a.quantidade_maquinas;
		quantidade_tarefas = a.quantidade_tarefas;

		for(int i = 0; i < quantidade_maquinas; i++)
		{
			maquinas[i] = new list<Job>; // Isso faz toda a diferença!!!
			for(list<Job>::iterator it = a.maquinas[i]->begin(); it != a.maquinas[i]->end(); it++)
			{
				this->maquinas[i]->push_back(*it);
			}
		}
	}
}

void Solucao::bind(Instancia *i)
{
	if(!i)
	{
		cout << "Instancia inexistente.\n";
		return;
	}
	inst = i;
	for(int k = 0; k < quantidade_maquinas; k++)
	{
		delete maquinas[k];
	}

	quantidade_maquinas = inst->quantMaquinas();
	quantidade_tarefas = 0;



	for(int k = 0; k < inst->quantMaquinas(); k++)
	{
		maquinas[k] = new list<Job>;
		//cout << "Aloquei a lista\n";
	}

}

void Solucao::atribuiTarefa(int tarefa, int rec, int maquina)
{
	assert(maquina >= 0 && maquina < quantidade_maquinas);
	assert(maquinas[maquina] != NULL);
	
	Job x; x.id = tarefa; x.rec = rec;

	maquinas[maquina]->push_back(x);
	quantidade_tarefas++;
	
}

int Solucao::tarefasProgramadas()
{
	int cont = 0;
	for(int i = 0; i < quantidade_maquinas; i++)
	{
		cont += maquinas[i]->size();
	}
	return cont;
}

void Solucao::incrementaNumeroTarefas()
{
    quantidade_tarefas++;
}

void Solucao::retiraTarefa(int tarefa, int maquina)
{
	assert(maquina >= 0 && maquina < quantidade_maquinas);
	assert(maquinas[maquina] != NULL);

	for(list<Job>::iterator it = maquinas[maquina]->begin(); it != maquinas[maquina]->end(); it++)
	{
		if(it->id == tarefa)
		{
			if(maquinas[maquina]->size() > 0)
			{
				maquinas[maquina]->erase(it);
				quantidade_tarefas--;
			}
			return;
		}
	}
}



void ordena_LC(list<Job> &LC)
{
	LC.sort();
	LC.reverse();
}

void exibe_LC(list<Job> &LC)
{
	for(list<Job>::iterator it = LC.begin(); it != LC.end(); it++)
	{
		it->exibe();
	}
	//out << "\n";
}

void insere_job_solucao(Solucao &aux,Job &job_it, int maq, int pos,double alfa, double beta)
{
	int cnt = 0;
	
	assert(maq >= 0);
	assert(pos >= 0);
	
	for(list<Job>::iterator it = aux.maquinas[maq]->begin(); it != aux.maquinas[maq]->end(); it++, cnt++)
	{
		if(cnt == pos)
		{
			aux.maquinas[maq]->insert(it,job_it);
			aux.incrementaNumeroTarefas();
			otimiza_recursos(aux,alfa,beta);
			
			return;
		}
	}
	aux.maquinas[maq]->push_back(job_it);
	aux.incrementaNumeroTarefas();
	otimiza_recursos(aux,alfa,beta);
}

Solucao gera_vizinho_tipoT(Solucao &s, PoliticaDeAlocacao politica, double alfa, double beta)
{
	Solucao nova = s;
	int pos1 = 0;
	int maq1 = 0;
	int pos2 = 0;
	int maq2 = 0;

	assert(s.quantidadeMaquinas() != 0);
	
	maq1 = rand()%s.quantidadeMaquinas();
	maq2 = rand()%s.quantidadeMaquinas();
	if(maq1 == maq2 && s.maquinas[maq1]->size() == 1)
	{
		while(maq1 == maq2)
		{
			maq2 = rand()%s.quantidadeMaquinas();
		}
	}
	if(s.maquinas[maq1]->size() == 0)
	{
		pos1 = 0;
	}
	else
	{
		assert( 0 != s.maquinas[maq1]->size() );

		pos1 = rand()%s.maquinas[maq1]->size();
	}

	if( s.maquinas[maq2]->size() == 0)
	{
		pos2 = 0;
	}
	else
	{
		assert( 0 != s.maquinas[maq2]->size());
		pos2 = rand()%s.maquinas[maq2]->size();
	}

	swap_jobs(nova,maq1,pos1,maq2,pos2);
	otimiza_recursos(nova,alfa,beta);
	

	return nova;
}

Solucao gera_vizinho_tipoN(Solucao &s, PoliticaDeAlocacao politica, double alfa, double beta)
{
	Solucao temp;
	temp = s;

	list<Solucao> vizinhanca_retirada;
	list<Job> tarefas_retiradas;

	Solucao temp_aux;

	for(int i = 0; i < temp.quantidadeMaquinas(); i++)
	{
		for(list<Job>::iterator it = temp.maquinas[i]->begin(); it != temp.maquinas[i]->end(); it++)
		{
			temp_aux = temp;
			tarefas_retiradas.push_back(*it);
			temp_aux.retiraTarefa(it->id,i);
			vizinhanca_retirada.push_back(temp_aux);
		}
	}


	int tarefa_sorteada = 0;
	int qtar = tarefas_retiradas.size();
	if(qtar > 0) tarefa_sorteada = rand()%qtar;

	int maquina_sorteada = 0;
	int qmaq = temp.quantidadeMaquinas();
	if(qmaq > 0) maquina_sorteada = rand()%qmaq;

	int cnt = 0;
	Job job_sorteado;
	for(list<Job>::iterator it = tarefas_retiradas.begin(); it != tarefas_retiradas.end(); 
	                                                                                    it++, cnt++)
	{
		if(cnt == tarefa_sorteada)
		{
			job_sorteado = *it;
			//cout << *it;
			break;
		}
	}
	cnt = 0;
	Solucao solucao_sorteada;
	for(list<Solucao>::iterator it = vizinhanca_retirada.begin(); it != vizinhanca_retirada.end(); 
	                                                                                    it++, cnt++)
	{
		if(cnt == tarefa_sorteada)
		{
			solucao_sorteada = *it;
			//cout << *it;
			break;
		}
	}
	int posicao_sorteada = 0;
	int qpos = solucao_sorteada.maquinas[maquina_sorteada]->size();
	if(qpos > 0) posicao_sorteada = rand()%qpos;
	insere_job_solucao(solucao_sorteada,job_sorteado,maquina_sorteada,posicao_sorteada,alfa,beta);

	return solucao_sorteada;
}

Solucao busca_local_recursiva(Solucao &s, PoliticaDeAlocacao politica, double alfa, double beta)
{
	Solucao temp;
	temp = s;

	list<Solucao> vizinhanca_retirada;
	list<Job> tarefas_retiradas;
	list<Solucao> vizinhanca_completa;

	Solucao temp_aux;
	for(int i = 0; i < temp.quantidadeMaquinas(); i++)
	{
		for(list<Job>::iterator it = temp.maquinas[i]->begin(); it != temp.maquinas[i]->end(); it++)
		{
			temp_aux = temp;
			tarefas_retiradas.push_back(*it);
			temp_aux.retiraTarefa(it->id,i);
			vizinhanca_retirada.push_back(temp_aux);
		}
	}
	// Neste ponto, tenho várias soluções que carecem de reinserção.
	double Zmin = 9999999;
	Solucao melhor_solucao;
	for(list<Job>::iterator job_it = tarefas_retiradas.begin(); job_it != tarefas_retiradas.end(); 
	                                                                                       job_it++)
	{
		for(list<Solucao>::iterator sol_it = vizinhanca_retirada.begin(); 
		                                              sol_it != vizinhanca_retirada.end(); sol_it++)
		{
			Solucao aux;
			aux = *sol_it;
			for(int maq = 0; maq < aux.quantidadeMaquinas(); maq++)
			{
				for(int pos = 0; pos < (int) aux.maquinas[maq]->size()+1; pos++)
				{
					if(aux.jaEscalonada(job_it->id)) continue;
					insere_job_solucao(aux,*job_it, maq, pos, alfa, beta);
					otimiza_recursos(aux,alfa,beta);
					double avaliacao_corrente = aux.avaliaSolucao(alfa,beta);
					if(avaliacao_corrente < Zmin)
					{
						Zmin = avaliacao_corrente;
						melhor_solucao = aux;
					}
					aux = *sol_it;
				}
			}

		}
	}

	if(temp.avaliaSolucao(alfa,beta) > Zmin) // Reduz a quantidade de recursões!!!
	{
		// cout << "rec " << Zmin << "\n";
		return busca_local_recursiva(melhor_solucao,politica,alfa,beta);
	}
	else
	{
		return temp;
	}
}

Solucao swap_jobs(Solucao &s, int maq1, int job1, int maq2, int job2)
{
	Solucao nova = s;
	Job temp1,temp2,aux;
	int cnt1,cnt2;
	if(maq1 < 0 || maq1 >= s.quantidadeMaquinas())
	{
		cout << "indice de maquina 1 invalido\n";
		cout << maq1 << "\n";
		exit(0);
	}
	if(maq2 < 0 || maq2 >= s.quantidadeMaquinas())
	{
		cout << "indice de maquina 2 invalido\n";
		cout << maq2 << "\n";
		exit(0);
	}

	list<Job>::iterator it_temp1,it_temp2;
	cnt1 = cnt2 = 0;

	for(list<Job>::iterator it = s.maquinas[maq1]->begin(); it != s.maquinas[maq1]->end(); 
	                                                                                   it++, cnt1++)
	{
		if(cnt1 == job1)
		{
			temp1 = *it;
			it_temp1 = it;
			break;
		}
	}

	for(list<Job>::iterator it = s.maquinas[maq2]->begin(); it != s.maquinas[maq2]->end(); 
	                                                                                   it++, cnt2++)
	{
		if(cnt2 == job2)
		{
			temp2 = *it;
			it_temp2 = it;
			break;
		}
	}

	swap(*it_temp1, *it_temp2);

	return nova;
}

int distanciaSolucao(int *v1, int *v2, int size)
{
	int diff = 0;
	for(int i = 0; i < size; i++)
	{
		if(v1[i] != v2[i]) diff++;
	}
	return diff;
}

void constroi_vetores(Solucao &origem, Solucao &destino, int *ids_origem, int *ids_destino, 
                                                                                          int *size)
{

// Vetor que ganhará a forma [3 2 -1 5 1 4 -1 7 6]
	int qtd_ids_origem = 0;
	int qtd_ids_destino = 0;


	for(int i = 0; i < origem.quantidadeMaquinas(); i++)
	{
		for(list<Job>::iterator it = origem.maquinas[i]->begin(); it != origem.maquinas[i]->end(); 
		                                                                                       it++)
		{
			ids_origem[qtd_ids_origem++] = it->id;
		}
		ids_origem[qtd_ids_origem++] = -1;
	}

	for(int i = 0; i < destino.quantidadeMaquinas(); i++)
	{
		for(list<Job>::iterator it = destino.maquinas[i]->begin(); it != destino.maquinas[i]->end();
		                                                                                       it++)
		{
			ids_destino[qtd_ids_destino++] = it->id;
		}
		ids_destino[qtd_ids_destino++] = -1;
	}

	*size = qtd_ids_destino - 1;
}

Solucao constroi_solucao_de_vetor(Solucao &molde,int *v, int size,double alfa, double beta)
{
	Solucao nova = molde;
	for(int i = 0; i < nova.quantidadeMaquinas(); i++)
	{
		nova.maquinas[i]->clear();
	}

	int current_machine = 0;
	for(int i = 0; i < size; i++)
	{
		if(v[i] == -1)
		{
			current_machine++;
		}
		else
		{
			Job j;
			j.id = v[i];
			nova.maquinas[current_machine]->push_back(j);
		}
	}
	otimiza_recursos(nova,alfa,beta);
	//cout << "Uma solucao construida no PR: " << nova << "\n";
	return nova;
}

void cpy_vector(int *v1, int *v2, int size)
{
	for(int i = 0; i < size; i++) v1[i] = v2[i];
}

void display_vector(int *v, int size)
{
	for(int i = 0; i < size; i++)
	{
		cout << v[i] << " ";
	}
	cout << "\n";
}

int find_value(int value, int *v, int size)
{
	for(int i = 0; i < size; i++)
	{
		if(v[i] == value) return i;
	}
	return -1;
}

void swap_int(int *a, int *b)
{
    assert(a != NULL);
    assert(b != NULL);
    assert(a != b);
    
	int temp = *a;
	*a = *b;
	*b = temp;
}

void gera_caminho(Solucao &base, list<Solucao> &ls, int *v1, int *v2, int size, double alfa, 
                                                                                        double beta)
{
	//cout << "Entrei\n";
	Solucao si;
	int vaux[MAX_JOBS]; // So pro pedantic nao reclamar, esse valor esta
	// semanticamente errado mas é grande o suficiente
	cpy_vector(vaux,v1,size);
	si = constroi_solucao_de_vetor(base,vaux, size,alfa,beta);
	ls.push_back(si);
	for(int pos = 0; pos < size; pos ++)
	{
		if(vaux[pos] != v2[pos])
		{
			// Vejo onde está o elemento de v1 que deveria estar em v1[pos].
			int posicao_alvo = find_value(v2[pos],vaux,size);
			if(posicao_alvo == -1)
			{
				cout << "Indice nao encontrado\n";
				exit(0);
			}
			int temp = vaux[posicao_alvo];
			vaux[posicao_alvo] = vaux[pos];
			vaux[pos] = temp;

			gera_caminho(base,ls,vaux,v2,size,alfa,beta);
			//cout << "gerei caminho recursivamente\n";
			si = constroi_solucao_de_vetor(base,vaux, size,alfa,beta);
			//cout << "construi solucao\n";

			ls.push_back(si);
		}
	}
	si = constroi_solucao_de_vetor(base,v2, size,alfa,beta);
	ls.push_back(si);
	cout << "Sai\n";
}





// Essa função irá inserir novas soluções ao pool respeitando seu tamanho máximo e prioridade.
void alimenta_pool(list<Solucao> &pool,Solucao &s, double alfa, double beta)
{
	otimiza_recursos(s,alfa,beta);
	unsigned n = 5;
	if(pool.size() == 0)
	{
		pool.push_back(s);
		return;
	}

	if(pool.size() < n) // Número de caminhos é O(n²).
	{
		for(list<Solucao>::iterator it = pool.begin(); it != pool.end(); it++)
		{
			if(it->avaliaSolucao(alfa,beta) > s.avaliaSolucao(alfa,beta))
			{
				pool.insert(it,s);
				return;
			}
		}
		pool.push_back(s);
		return;
	}
	else
	{
		for(list<Solucao>::iterator it = pool.begin(); it != pool.end(); it++)
		{
			if(it->avaliaSolucao(alfa,beta) > s.avaliaSolucao(alfa,beta))
			{
				pool.insert(it,s);
				pool.erase(it);
				return;
			}
		}
		pool.pop_back();
		pool.push_back(s);
		return;
	}
}

void exibe_pool(list<Solucao> & pool)
{
	cout << "Exibindo pool:\n";
	for(list<Solucao>::iterator it = pool.begin(); it != pool.end(); it++)
	{
		cout << *it << "\n";
	}
}






Solucao busca_local(Solucao &s, PoliticaDeAlocacao politica, double alfa, double beta)
{
	Solucao temp;
	temp = s;

	list<Solucao> vizinhanca_retirada;
	list<Job> tarefas_retiradas;
	list<Solucao> vizinhanca_completa;

	Solucao temp_aux;
	for(int i = 0; i < temp.quantidadeMaquinas(); i++)
	{
		for(list<Job>::iterator it = temp.maquinas[i]->begin(); it != temp.maquinas[i]->end(); it++)
		{
			temp_aux = temp;
			tarefas_retiradas.push_back(*it);
			temp_aux.retiraTarefa(it->id,i);
			vizinhanca_retirada.push_back(temp_aux);
		}
	}
	// Neste ponto, tenho várias soluções que carecem de reinserção.
	double Zmin = 9999999;
	Solucao melhor_solucao;
	for(list<Job>::iterator job_it = tarefas_retiradas.begin(); job_it != tarefas_retiradas.end(); 
	                                                                                       job_it++)
	{
		for(list<Solucao>::iterator sol_it = vizinhanca_retirada.begin(); 
		                                              sol_it != vizinhanca_retirada.end(); sol_it++)
		{
			Solucao aux;
			aux = *sol_it;
			for(int maq = 0; maq < aux.quantidadeMaquinas(); maq++)
			{
				for(int pos = 0; pos < (int) aux.maquinas[maq]->size()+1; pos++)
				{
					if(aux.jaEscalonada(job_it->id)) continue;
					insere_job_solucao(aux,*job_it, maq, pos, alfa, beta);
					otimiza_recursos(aux,alfa,beta);
					double avaliacao_corrente = aux.avaliaSolucao(alfa,beta);
					if(avaliacao_corrente < Zmin)
					{
						Zmin = avaliacao_corrente;
						melhor_solucao = aux;
					}
					aux = *sol_it;
				}
			}

		}
	}
	
	if(temp.avaliaSolucao(alfa,beta) > Zmin) // Reduz a quantidade de recursões!!!
	{
		// cout << "rec " << Zmin << "\n";
		return melhor_solucao;
	}
	else
	{
		return temp;
	}
}

Solucao busca_local_iterada(Solucao &s, PoliticaDeAlocacao politica, double alfa, 
                                                                                        double beta)
{
	Solucao best,temp;
	best = busca_local(s,politica,alfa,beta);
	double melhor_avaliacao = best.avaliaSolucao(alfa,beta);
	double current;
	for(;;)
	{
		//cout << "i = " << i << "\n";
		temp = busca_local(best,politica,alfa,beta);
		current = temp.avaliaSolucao(alfa,beta);
		if(current < melhor_avaliacao)
		{
			best = temp;
			melhor_avaliacao = current;
		}
		else
		{
			break;
		}
	}
	return best;
}

ostream& operator<<(ostream &out, Job &s)
{
	out << s.id;
	out << " " << s.incremento << ", ";
    return out;
}

ostream& operator<<(ostream &out, Solucao &s)
{
	int contador_de_tarefas = s.quantidade_tarefas;
	for(int i = 0; i < s.quantidade_maquinas; i++)
	{
		if(s.maquinas[i] && contador_de_tarefas)
		{
			for(list<Job>::iterator it = s.maquinas[i]->begin(); it != s.maquinas[i]->end(); it++)
			{
				out << (*it).id << "/" << (*it).rec << " ";
				contador_de_tarefas--;
			}
			if(i != s.quantidade_maquinas - 1)
			{
				out <<"-1 ";
			}
		}
	}
	out << "\n";
    return out;
}

void Solucao::exibeSolucao()
{
	//cout << quantidade_tarefas << ", " << quantidade_maquinas << "\n";
	int contador_de_tarefas = quantidade_tarefas;
	for(int i = 0; i < quantidade_maquinas; i++)
	{
		assert(maquinas[i]);
		assert(contador_de_tarefas);
		
		for(list<Job>::iterator it = maquinas[i]->begin(); it != maquinas[i]->end(); it++)
		{
		
			cout << (*it).id << "/" << (*it).rec << " ";
			contador_de_tarefas--;
		}
		if(i != quantidade_maquinas - 1)
		{
			cout <<"-1 ";
		}
		
	}
	cout << "\n";
}


int Solucao::quantidadeMaquinas()
{
	return quantidade_maquinas;
}

int Solucao::tarefasAtribuidasNaMaquina(int j)
{
	return maquinas[j]->size();
}

int Solucao::posicaoNaMaquina(int tarefa, int maquina)
{
	int cnt = 0;
	for(list<Job>::iterator it = maquinas[maquina]->begin(); it != maquinas[maquina]->end(); it++)
	{
		if(it->id == tarefa) return cnt;
		cnt++;
	}
	return -1;
}

bool Solucao::jaEscalonada(int id)
{
	for(int i = 0; i < quantidade_maquinas; i++)
	if(posicaoNaMaquina(id, i) != -1) return true;
	return false;
}

int Solucao::antecessoraNaMaquina(int tarefa,int maquina)
{
	int i, i_ant = -1;
	for(list<Job>::iterator it = maquinas[maquina]->begin(); it != maquinas[maquina]->end(); it++)
	{
		i = it->id;
		if(i == tarefa) return i_ant;
		else i_ant = i;
	}
	return -1;
}

int Solucao::quantidadeDeSucessorasNaMaquina(int tarefa,int maquina)
{
	int cnt = 0;
	for(list<Job>::iterator it = maquinas[maquina]->begin(); it != maquinas[maquina]->end(); it++)
	{
		cnt++;
		if(it->id == tarefa) cnt = 0;
	}
	return cnt;
}

int Solucao::ultimaAtribuidaNaMaquina(int j)
{
	if(tarefasAtribuidasNaMaquina(j) > 0)
	{
		return maquinas[j]->back().id;
	}
	else
	{
		return -1;
	}
}

int Solucao::quantidadeTarefas()
{
	return quantidade_tarefas;
}

Instancia *Solucao::acessaInstancia()
{
	return inst;
}

int Solucao::insereNaMelhorPosicao(int tarefa, PoliticaDeAlocacao politica, double alfa, 
                                                                                        double beta)
{   
    assert(tarefa >= 0);
    
	double ava = -1;
	double melhor_avaliacao = 99999999;
	double melhor_maquina = -1;
	int rec = -1;
	int best_rec = -1;
	int tarefaAnterior = -1;
	for(int i = 0; i < quantidade_maquinas; i++)
	{
		atribuiTarefa(tarefa,rec,i);
		tarefaAnterior = antecessoraNaMaquina(tarefa,i);
		
		#if 0
		cout << "tarefa anterior a " << tarefa << " em maq[" << i << "] seria " 
		                                                                  << tarefaAnterior << "\n";
		#endif
		
		if(tarefaAnterior == -1)
		{
			rec = 0;
		}
		else
		{
			//cout << "Achei algo...\n";
			switch(politica)
			{
				case minRec:
				{
					rec = inst->RijkMin(i,tarefaAnterior,tarefa);
					break;
				}
				case maxRec:
				{
					rec = inst->RijkMax(i,tarefaAnterior,tarefa);
					break;

				}
				case averageRec:
				{
					rec = 0.5*(inst->RijkMax(i, tarefaAnterior, tarefa) + 
					                                      inst->RijkMin(i, tarefaAnterior, tarefa));
					break;

				}
			}
		}
		retiraTarefa(tarefa,i);
		atribuiTarefa(tarefa,rec,i);
		ava = avaliaSolucao(alfa,beta);
		if(ava < melhor_avaliacao)
		{
			best_rec = rec;
			melhor_maquina = i;
			melhor_avaliacao = ava;
		}
		retiraTarefa(tarefa,i);
	}
	atribuiTarefa(tarefa,best_rec,melhor_maquina);
	return melhor_maquina;
}

double Solucao::avaliaSolucao(double alfa, double beta)
{
	double Tres = 0.0;
	double C = 0.0;
	for(int i = 0; i < quantidade_maquinas; i++)
	{
		int id_ant;
		bool predec = false; // Flag para saber se existe predecessor;
		double tempo_transcorrido = 0.0;
		for(list<Job>::iterator it = maquinas[i]->begin(); it != maquinas[i]->end(); it++)
		{
			int id = it->id;
			double rec = it->rec;
			//cout << it->id << "\n";
			if(predec)
			{
				Tres += rec;
				tempo_transcorrido += inst->avaliaTempoDeSetupComRecurso(
						inst->SijkMax(i,id_ant,id),
						inst->SijkMin(i,id_ant,id),
						inst->RijkMax(i,id_ant,id),
						inst->RijkMin(i,id_ant,id),
						rec
				);
			}
			predec = true;
			id_ant = id;
			tempo_transcorrido += inst->Pij(id,i);
			C += tempo_transcorrido;
		}
	}
	return beta * Tres + alfa * C;
}



Solucao& Solucao::operator=(const Solucao &x) {
	if (this == &x)
	{
		return *this;
	}

	for(int i = 0; i < quantidade_maquinas; i++)
	{
		delete maquinas[i];
	}

	quantidade_maquinas = x.quantidade_maquinas;
	quantidade_tarefas = x.quantidade_tarefas;
	inst = x.inst;


	for(int i = 0; i < quantidade_maquinas; i++)
	{
		maquinas[i] = new list<Job>;
		for(list<Job>::iterator it = x.maquinas[i]->begin(); it != x.maquinas[i]->end(); it++)
		{
			maquinas[i]->push_back(*it);
		}
	}
    return *this;
}


Solucao::~Solucao() {
	for(int i = 0; i < quantidade_maquinas; i++)
	{
		maquinas[i]->clear();
		delete maquinas[i];
	}
}

bool operator==(Solucao& a, Solucao& b)
{
	if(&a == &b) return true;
	if(a.quantidadeMaquinas() != b.quantidadeMaquinas() ) return false;
	if(a.quantidadeTarefas() != b.quantidadeTarefas() ) return false;
	for(int i = 0; i < a.quantidadeMaquinas(); i++)
	{
		if(a.maquinas[i]->size() != b.maquinas[i]->size()) return false;
		list<Job>::iterator it2 = b.maquinas[i]->begin();
		for(list<Job>::iterator it = a.maquinas[i]->begin(); it != a.maquinas[i]->end(); it++,it2++)
		{
			if(it->id != it2->id) return false;
		}
	}
	return true;
}

void construcao_solucao(Instancia *inst, Solucao &sequencia, double alfa, double beta, 
                                             PoliticaDeAlocacao politica, double alfa_aleatoriedade)
{
	list<Job> LC;
	for(int i = 0; i < inst->quantTarefas(); i++)
	{
		LC.push_back(Job(i));
	} // Criei uma lista {0, 1, 2, ..., n-1}
	double avaliacao_corrente = 0;
	sequencia.bind(inst);

	while(sequencia.quantidadeTarefas() < inst->quantTarefas())
	{
		for(list<Job>::iterator it = LC.begin(); it != LC.end(); it++)
		{
			int maq = sequencia.insereNaMelhorPosicao(it->get_id(),politica,alfa,beta);
			it->set_inc(sequencia.avaliaSolucao(alfa,beta) - avaliacao_corrente);
			it->maq = maq;
			sequencia.retiraTarefa(it->get_id(),maq);
		}

		ordena_LC(LC);

		Job temp;
		//cout << LC.size() << "aki \n";
		int range = alfa_aleatoriedade * LC.size();
		int pos = 0;
		if(range != 0)
		{
			pos = rand()%range;
		}

		int i = 0;
		for(list<Job>::iterator it = LC.begin(); it != LC.end(); it++, i++)
		{
			if(i == pos)
			{
				temp.id = it->id;
				temp.incremento = it->incremento;
				temp.maq = it->maq;
				LC.erase(it);
				break;
			}
		}

		int tar = temp.id;
		int maq = temp.maq;

		int rec = 0;
		sequencia.atribuiTarefa(tar,rec,maq);

		switch(politica)
		{
			case maxRec:
			{
				rec = inst->RijkMax(maq,sequencia.antecessoraNaMaquina(tar,maq),tar);
				//cout << "Recurso alocado no maxRec: " << rec << "\n";
				sequencia.retiraTarefa(tar,maq);
				sequencia.atribuiTarefa(tar,rec,maq);
				break;
			}
			case minRec:
			{
				rec = inst->RijkMin(maq,sequencia.antecessoraNaMaquina(tar,maq),tar);
				//cout << "Recurso alocado no minRec: " << rec << "\n";
				sequencia.retiraTarefa(tar,maq);
				sequencia.atribuiTarefa(tar,rec,maq);
				break;
			}
			case averageRec:
			{
				rec = 0.5*(inst->RijkMax(maq,sequencia.antecessoraNaMaquina(tar,maq),tar) +
						inst->RijkMin(maq,sequencia.antecessoraNaMaquina(tar,maq),tar));
				//cout << "Recurso alocado no averageRec: " << rec << "\n";
				sequencia.retiraTarefa(tar,maq);
				sequencia.atribuiTarefa(tar,rec,maq);
				break;
			}
		}
		avaliacao_corrente = sequencia.avaliaSolucao(alfa,beta);
	}
}

void otimiza_recursos(Solucao &sequencia, double alfa, double beta)
{
	for(int i = 0; i < sequencia.quantidadeMaquinas(); i++)
	{
		for(list<Job>::iterator it = sequencia.maquinas[i]->begin();
		                                                   it != sequencia.maquinas[i]->end(); it++)
		{
			if(sequencia.posicaoNaMaquina(it->id,i) >= 1)
			{
				int RijkMax = sequencia.acessaInstancia()->RijkMax(i, 
				                                   sequencia.antecessoraNaMaquina(it->id,i),it->id);
				                                   
				int RijkMin = sequencia.acessaInstancia()->RijkMin(i, 
				                                   sequencia.antecessoraNaMaquina(it->id,i),it->id);
				if(RijkMax == RijkMin)
				{
					it->rec = RijkMin;
				}
				else
				{
					double SijkMax = sequencia.acessaInstancia()->SijkMax(i, 
					                               sequencia.antecessoraNaMaquina(it->id,i),it->id);
					                               
					double SijkMin = sequencia.acessaInstancia()->SijkMin(i,
					                               sequencia.antecessoraNaMaquina(it->id,i),it->id);
					                               
					if(SijkMax != SijkMin)
					{
					    double h = 1+sequencia.quantidadeDeSucessorasNaMaquina(it->id,i);
                        
                        #if 0					    
					    cout << h << " sucessoras do setup time da terefa " << it->id <<"\n";
                        #endif 
                        
					    double K2 = (SijkMax-SijkMin)/(RijkMax-RijkMin);
					    if(RijkMax != RijkMin)
					    {
					        #if 0	
					        cout << "K2, h, beta, alfa " << K2 << " " << h << " " << beta << " " << 
					                                                                   alfa << "\n";
					        #endif
					        
					        if( K2*h*beta < alfa )
					        {
						        it->rec = RijkMax;
					        }
					        else
					        {
						        it->rec = RijkMin;
					        }
					    }
					    else
					    {
					        it->rec = RijkMin;
					    }
					}
					else
					{
					    it->rec = RijkMin;
					}
				}
			}
			else
			{
			    it->rec = 0; 
			}
		}
	}
}

} /* namespace std */


