/*
 * Instancia.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: thaylo
 */

#include "Instancia.h"

namespace std {

Instancia::Instancia() {
	qtd_maquinas = qtd_tarefas = 0;
}

void Instancia::lePij(FILE *fd)
{
	int lixo = 0;
	int rc = 0;
	for(int i = 0; i < qtd_tarefas; i++)
	{
		for(int j = 0; j < qtd_maquinas; j++)
		{
			rc = fscanf(fd,"%d%d",&lixo,&(p[i][j]));
			if(rc == 0)
			{
			    printf("Erro de leitura");
			    exit(0);
			}
		}
	}
}

// Esta função lê as matrizes de Sijk+, Sijk-, Rijk+ e Rijk- para cada máquina.
void Instancia::leMatriz(FILE *fd,char *desc)
{
	char buffer[30];
	int rc = 0;
	
	rc = fscanf(fd,"%s",buffer);
    if(rc == 0)
    {
        printf("Erro de leitura1");
        exit(0);
    }
	if(!strcmp(buffer,desc))
	{
		for(int k = 0; k < qtd_maquinas; k++)
		{
			rc = fscanf(fd,"%s",buffer); // M0, M1, M2, ... Mm
			if(rc == 0)
			{
			    printf("Erro de leitura2");
			    exit(0);
			}
			for(int i = 0; i < qtd_tarefas; i++)
			{
				for(int j = 0; j < qtd_tarefas; j++)
				{
					if(!strcmp(desc,(char*)"RSSDMIN"))
					{
						rc = fscanf(fd,"%d",&(RSSDMIN[k][i][j]));
						if(rc == 0)
                        {
                            printf("Erro de leitura3");
                            exit(0);
                        }
					}

					else if(!strcmp(desc,(char*)"RSSDMAX"))
					{
						rc = fscanf(fd,"%d",&(RSSDMAX[k][i][j]));
						if(rc == 0)
			            {
			                printf("Erro de leitura4");
			                exit(0);
			            }
					}

					else if(!strcmp(desc,(char*)"SSDMIN"))
					{
						rc = fscanf(fd,"%d",&(SSDMIN[k][i][j]));
						if(rc == 0)
			            {
			                printf("Erro de leitura5");
			                exit(0);
			            }
					}

					else if(!strcmp(desc,(char*)"SSDMAX"))
					{
						rc = fscanf(fd,"%d",&(SSDMAX[k][i][j]));
                        if(rc == 0)
                        {
                            printf("Erro de leitura6");
                            exit(0);
                        }
					}

					else
					{
						printf("Erro de leitura7\n");
						exit(0);
					}
				}
			}
		}
	}
	else
	{
		printf("Leitura incorreta.\n");
	}
}

Instancia::Instancia(char *arquivo) {

	// Preparação
	FILE *fd = fopen(arquivo,"r");
	if(!fd)
	{
		printf("File %s not found.\n",arquivo);
		qtd_maquinas = qtd_tarefas = 0;
	}

	int lixo;
	int rc = 0;
	
	rc = fscanf(fd,"%d%d%d",&qtd_tarefas,&qtd_maquinas,&lixo);
	if(rc == 0)
	{
	    printf("Erro de leitura");
	    exit(0);
	}
	if(lixo != 1)
	{
		printf("A instancia %s tem o terceiro parametro diferente de 1.\n",arquivo);
		exit(0);
	}
	rc = fscanf(fd,"%d",&lixo);
	if(rc == 0)
	{
	    printf("Erro de leitura");
	    exit(0);
	}

	// Leitura propriamente dita.
	lePij(fd);
	leMatriz(fd,(char*)"RSSDMIN");
	leMatriz(fd,(char*)"RSSDMAX");
	leMatriz(fd,(char*)"SSDMIN");
	leMatriz(fd,(char*)"SSDMAX");

	fclose(fd);
}

void Instancia::exibe()
{
	printf("n = %d, m = %d\n",qtd_tarefas,qtd_maquinas);
	for(int i = 0; i < qtd_tarefas; i++)
	{
		for(int j = 0; j < qtd_maquinas; j++)
		{
			printf("%d ",p[i][j]);
		}
		printf("\n");
	}

	printf("RSSDMIN\n");
	for(int k = 0; k < qtd_maquinas; k++)
	{
		printf("M%d\n",k);
		for(int i = 0; i < qtd_tarefas; i++)
		{
			for(int j = 0; j < qtd_tarefas; j++)
			{
				printf("%d ",RSSDMIN[k][i][j]);
			}
			printf("\n");
		}
	}


	printf("RSSDMAX\n");
	for(int k = 0; k < qtd_maquinas; k++)
	{
		printf("M%d\n",k);
		for(int i = 0; i < qtd_tarefas; i++)
		{
			for(int j = 0; j < qtd_tarefas; j++)
			{
				printf("%d ",RSSDMAX[k][i][j]);
			}
			printf("\n");
		}
	}


	printf("SSDMIN\n");
	for(int k = 0; k < qtd_maquinas; k++)
	{
		printf("M%d\n",k);
		for(int i = 0; i < qtd_tarefas; i++)
		{
			for(int j = 0; j < qtd_tarefas; j++)
			{
				printf("%d ",SSDMIN[k][i][j]);
			}
			printf("\n");
		}
	}

	printf("SSDMAX\n");
	for(int k = 0; k < qtd_maquinas; k++)
	{
		printf("M%d\n",k);
		for(int i = 0; i < qtd_tarefas; i++)
		{
			for(int j = 0; j < qtd_tarefas; j++)
			{
				printf("%d ",SSDMAX[k][i][j]);
			}
			printf("\n");
		}
	}
}

double Instancia::avaliaTempoDeSetupComRecurso(int SijkMax, int SijkMin, int RijkMax, int RijkMin, 
                                                                                  int RijkAtribuido)
{
    if(RijkAtribuido == RijkMax) return SijkMin;
    if(RijkAtribuido == RijkMin) return SijkMax;
    
	double deltaX = RijkMin - RijkMax;
	if(deltaX == 0) // Evitando divisão por zero.
	{
		return SijkMax;
	}
	
	double deltaY = SijkMax - SijkMin;
	double coef_angular = deltaY/deltaX;

	return (RijkAtribuido - RijkMin)*coef_angular + SijkMax;
}

#if 0
/* Algoritmo testado que seleciona a parte desejada da solucao */
  	int start = 0;
	for(int i = 0; i < maquina; i++)
	{
		while(s->indices[start] != -1 )
		{
			start++;
		}
		start++;
	}
	for(int i = start; i < s->quantidade_tarefas; i++)
	{
		if(s->indices[i] == -1) break;
		//printf("%d ",s->indices[i]);
	}
	//printf("\n");
#endif

int Instancia::quantTarefas()
{
	return qtd_tarefas;
}

int Instancia::quantMaquinas()
{
	return qtd_maquinas;
}

int Instancia::Pij(int i, int j)
{
	return p[i][j];
}

int Instancia::SijkMax(int maq, int j, int k)
{
	if(j == -1)
	{
	//	cout << "Nao ha antecessora\n";
		return 0;
	}
	else
	{
		//	cout << "Existe antecessora\n";
	}
	return SSDMAX[maq][j][k];
}

int Instancia::SijkMin(int maq, int j, int k)
{
	if(j == -1)
	{
		//	cout << "Nao ha antecessora\n";
		return 0;
	}
	else
	{
		//	cout << "Existe antecessora\n";
	}
	return SSDMIN[maq][j][k];
}

int Instancia::RijkMax(int maq, int j,int k)
{
	if(j == -1)
	{
		//	cout << "Nao ha antecessora\n";
		return 0;
	}
	else
	{
		//	cout << "Existe antecessora\n";
	}
	return RSSDMAX[maq][j][k];
}

int Instancia::RijkMin(int maq, int j,int k)
{
	if(j == -1)
	{
		//	cout << "Nao ha antecessora\n";
		return 0;
	}
	else
	{
		//	cout << "Existe antecessora\n";
	}
	return RSSDMIN[maq][j][k];
}

Instancia::~Instancia() {}

} /* namespace std */
