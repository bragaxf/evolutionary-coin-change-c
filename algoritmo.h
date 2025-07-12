#ifndef ALGORITMO_H
#define ALGORITMO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Constantes
#define TAMANHO_POPULACAO 100
#define MAX_MOEDAS 8
#define TAXA_RECOMBINACAO 0.7
#define TAXA_MUTACAO 0.1

// Funções
int sol_aleatoria(float* moedas, int* solucao, int N, float V);
float avaliar_solucao(float* moedas, int* solucao, int N, float V, float penalidade);
void reparar_solucao(int* solucao, float* moedas, int N, float V);

void gerar_vizinhos_incremento(int* solucao_atual, int* novo_vizinho, int N, float* moedas, float V);
void gerar_vizinhos_troca(int* solucao_atual, int* novo_vizinho, int N, float* moedas, float V);
void gerar_vizinhos_substituicao(int* solucao_atual, int* novo_vizinho, int N, float* moedas, float V);

void trepa_colinas(float* moedas, int* solucao_inicial, int N, float V, float penalidade, int max_iter, int tipo_vizinhanca);

int selecao_torneio(float* fitness, int tamanho_populacao);
int selecao_torneio_com_elitismo(int N, int populacao[][N], float* fitness, int nova_populacao[][N], int taxa_elitismo);
int selecao_roleta(float* fitness, int tamanho_populacao);

void crossover_dois_pontos(int* pai1, int* pai2, int* filho, int N, float* moedas, float V);
void crossover_uniforme_adaptativo(int* pai1, int* pai2, int* filho, int N, float fitness_pai1, float fitness_pai2, float* moedas, float V);

void mutacao_simples(int* solucao, int N, float* moedas, float V);
void mutacao_troca(int* solucao, int N, float* moedas, float V);

void algoritmo_evolutivo(float* moedas, int N, float V, int max_geracoes, int* solucao_inicial, float penalidade);

void algoritmo_hibrido(float *moedas, int N, float V, int max_geracoes, int max_iter, int *solucao_inicial, float penalidade,int tipo_vizinhanca);
void algoritmo_hibrido2(float *moedas, int N, float V, int max_geracoes, int max_iter, int *solucao_inicial, float penalidade, int tipo_vizinhanca);
#endif
