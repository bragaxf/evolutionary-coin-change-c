#include "algoritmo.h"


#define TAMANHO_POPULACAO 100
#define MAX_MOEDAS 8
#define TAXA_RECOMBINACAO 0.7
#define TAXA_MUTACAO 0.01

//                          util
int sol_aleatoria(float* moedas,int* solucao,int N,float V) {
    float soma =0.0;
    for (int i = 0; i < N; i++) {
            solucao[i] = 0;
    }

    while (fabs(soma - V) > 1e-6) { // Continua enquanto a soma não estiver dentro da tolerância de V
        int idx = rand() % N; // Escolhe um índice aleatório
        // Verifica se é possível incrementar sem ultrapassar V
        if (soma + moedas[idx] <= V) {
            solucao[idx]++; // Incrementa o número de moedas nesse índice
        }
        // Verifica se é possível decrementar sem ficar negativo
        else if (soma - moedas[idx] >= 0 && solucao[idx] > 0) {
            solucao[idx]--; // Decrementa o número de moedas nesse índice
        }
        // Recalcula a soma após a modificação
        soma = 0.0;
        for (int j = 0; j < N; j++) {
            soma += moedas[j] * solucao[j];
        }
    }
    return 0;

}

float avaliar_solucao(float *moedas, int *solucao, int N, float V, float penalidade) {
    float soma = 0.0;
    int total_moedas = 0;

    // Calcular soma total e número de moedas
    for (int i = 0; i < N; i++) {
        soma += solucao[i] * moedas[i];
        total_moedas += solucao[i];
    }

    if (fabs(soma - V) < 1e-3) {
       //printf("Solucao valida!!!\n");
        return total_moedas; // Solução válida: retorna número de moedas
    }
    else {
        //printf("Solucao invalida!!!\n");
        return total_moedas+ penalidade * fabs(soma - V); // Solução inválida: adiciona penalização
    }
}

void reparar_solucao(int *solucao, float *moedas    , int N, float V) {
    float soma = 0.0;

    // Calcula a soma inicial da solução
    for (int i = 0; i < N; i++) {
        soma += solucao[i] * moedas[i];
    }

    // Reduz o valor total se exceder V
    while (soma > V) {
        for (int i = N - 1; i >= 0; i--) {
            if (solucao[i] > 0) {
                solucao[i]--; // Remove uma moeda
                soma -= moedas[i];
                if (soma <= V)
                    break;
            }
        }
    }

    // Aumenta o valor total se estiver abaixo de V
    while (soma < V) {
        for (int i = 0; i < N; i++) {
            solucao[i]++;
            soma += moedas[i];
            if (soma >= V)
                break;
        }
    }
}

//                                  TREPA COLINAS

void gerar_vizinhos_incremento(int *solucao_atual, int *novo_vizinho, int N, float *moedas, float V) {
    int valido = 0; // Variável para verificar se a solução gerada é válida e diferente da atual

    while (!valido ) { // Continua enquanto o vizinho não for válido

        // Copia a solução atual para o novo vizinho
        for (int i = 0; i < N; i++) {
            novo_vizinho[i] = solucao_atual[i];
        }

        // Modificar múltiplas moedas
            int num_moedas_a_alterar = rand() % N + 1; // Escolhe aleatoriamente quantas moedas alterar (pelo menos 1)
            for (int k = 0; k < num_moedas_a_alterar; k++) {
                int idx = rand() % N; // Escolhe um índice aleatório
                int idx2 = rand() % N;
                while (idx == idx2) {
                    idx2 = rand() % N;

                }
                    int alterar = rand() % 5 - 2; // Incrementa ou decrementa entre -2 e +2
                    int alterar2 = rand() % 5 - 2; // Incrementa ou decrementa entre -2 e +2
                    novo_vizinho[idx] = (novo_vizinho[idx] + alterar >= 0) ? novo_vizinho[idx] + alterar : 0;
                    novo_vizinho[idx2] = (novo_vizinho[idx2] + alterar2 >= 0) ? novo_vizinho[idx2] + alterar2 : 0;

            }

            // Garante que nenhum elemento do novo vizinho seja negativo
            for (int i = 0; i < N; i++) {
                if (novo_vizinho[i] < 0) {
                    novo_vizinho[i] = 0; // Corrige valores negativos
                }
            }

            // Recalcula a soma do novo vizinho
            float soma = 0.0;
            for (int i = 0; i < N; i++) {
                soma += moedas[i] * novo_vizinho[i];
            }
            /// alterar aqui para custo igual do incremento
            // Verifica se a soma está dentro da tolerância
            if (fabs(soma - V) <= 1e-6) {
                    valido = 1; // Encerra o loop
            }
    }
}

void gerar_vizinhos_troca(int *solucao_atual, int *novo_vizinho, int N, float *moedas, float V) {
    // Copia a solução atual para o novo vizinho
    for (int i = 0; i < N; i++) {
        novo_vizinho[i] = solucao_atual[i];
    }

    // Determina quantas trocas fazer (mínimo 1, máximo N / 2)
    int num_trocas = (N > 2) ? rand() % (N / 2) + 1 : 1;

    // Realiza múltiplas trocas
    for (int t = 0; t < num_trocas; t++) {
        // Seleciona dois índices aleatórios
        int idx1 = rand() % N;
        int idx2 = rand() % N;

        // Certifica-se de que os índices são diferentes e válidos para a troca
        if (idx1 != idx2 && novo_vizinho[idx1] > 0 && moedas[idx2] > moedas[idx1]) {
            // Transfere moedas proporcionalmente
            int transferir = novo_vizinho[idx1] / 2;
            if (transferir > 0) {
                novo_vizinho[idx1] -= transferir;
                novo_vizinho[idx2] += transferir / (moedas[idx2] / moedas[idx1]);
            }
        }
    }

    reparar_solucao(novo_vizinho, moedas, N, V);
}

void gerar_vizinhos_substituicao(int *solucao_atual, int *novo_vizinho, int N, float *moedas, float V) {

    // Copia a solução atual para o novo vizinho
    for (int i = 0; i < N; i++) {
        novo_vizinho[i] = solucao_atual[i];
    }


        // Inicializa os índices como inválidos
        int idx1 = 0; // Índice com pelo menos 2 moedas
        int idx2 = 0; // Índice com moeda maior

        // Encontra um índice válido para `idx1`
        for (int i = 0; i < N; i++) {
            if (novo_vizinho[i] >= 2) { // Precisa de pelo menos 2 moedas
                idx1 = i;
                break;
            }
        }

        // Encontra um índice válido para `idx2` (moeda maior que `idx1`)
        for (int j = 0; j < N; j++) {
            if (moedas[j] > moedas[idx1]) {
                idx2 = j;
                break;
            }
        }

        // Realiza a substituição
        novo_vizinho[idx1] -= 2; // Remove 2 moedas do índice menor
        novo_vizinho[idx2]++;    // Adiciona 1 moeda ao índice maior
        reparar_solucao(novo_vizinho, moedas, N, V);
}

void trepa_colinas(float *moedas, int *solucao_inicial, int N, float V, float penalidade, int max_iter,int tipo_vizinhanca) {
    // Vetores para armazenar a solução atual e o melhor vizinho encontrado
    int solucao_atual[N], melhor_vizinho[N];
    float soma = 0.0;

    // Inicializa a solução atual como a solução inicial fornecida
    for (int i = 0; i < N; i++) {
        solucao_atual[i] = solucao_inicial[i];
    }

    // Avalia o fitness inicial da solução
    float fitness_atual = avaliar_solucao(moedas, solucao_atual, N, V, penalidade);
    int metodo_escolhido = 0; // Variável para rastrear o metodd
    // Loop principal: tenta melhorar a solução atual até atingir o máximo de iterações
    for (int iter = 0; iter < max_iter; iter++) {
        // Escolhe aleatoriamente um dos três métodos para gerar vizinhos
        //int tipo_vizinhanca = rand() % 3;

        if (tipo_vizinhanca == 0) {
            gerar_vizinhos_incremento(solucao_atual, melhor_vizinho, N, moedas, V);
        } else if (tipo_vizinhanca == 1) {
            gerar_vizinhos_troca(solucao_atual, melhor_vizinho, N, moedas, V);
        } else {
            gerar_vizinhos_substituicao(solucao_atual, melhor_vizinho, N, moedas, V);
        }
        // TROCA DE MOEDAS
        for (int i = 0; i < N - 1; i++) { // Percorre as moedas menores
            while (solucao_atual[i] >= (int)(moedas[i + 1] / moedas[i])) {
                int quantidade_troca = (int)(moedas[i + 1] / moedas[i]);
                solucao_atual[i] -= quantidade_troca;  // Remove moedas menores
                solucao_atual[i + 1]++;               // Adiciona moedas maiores
            }
        }
        reparar_solucao(solucao_atual, moedas, N, V);
        // Recalcula a soma do vizinho gerado
        soma = 0.0;
        for (int j = 0; j < N; j++) {
            soma += moedas[j] * melhor_vizinho[j];
        }

        // Avalia a qualidade do vizinho
        float fitness_vizinho = avaliar_solucao(moedas, melhor_vizinho, N, V, penalidade);

        // Atualiza a solução atual se o vizinho for melhor e válido
        if (fitness_vizinho <= fitness_atual && fabs(soma - V) <= 1e-3) {
            metodo_escolhido = tipo_vizinhanca + 1; // Salva o metodo
            for (int i = 0; i < N; i++) {
                solucao_atual[i] = melhor_vizinho[i];
            }
            fitness_atual = fitness_vizinho; // Atualiza o fitness atual
        }
    }

    // Exibe os resultados finais: a melhor solução encontrada e seu fitness
    /*if (metodo_escolhido == 1) {
        printf("Melhor solucao encontrada usando o metodo de incremento:\n");
    } else if (metodo_escolhido == 2) {
        printf("Melhor solucao encontrada usando o metodo de troca:\n");
    } else {
        printf("Melhor solucao encontrada usando o metodo de substituicao:\n");
    }

    for (int i = 0; i < N; i++) {
        printf("Moedas de %.2f: %d unidades\n", moedas[i], solucao_atual[i]);
    }*/
    fitness_atual=avaliar_solucao(moedas, solucao_atual, N, V, penalidade);
    //printf("Fitness Final: %.2f\n", fitness_atual);
    for (int i = 0; i < N; i++) {
        solucao_inicial[i] = solucao_atual[i];
    }
}

//                                  EVOLUTIVO

int selecao_torneio( float *fitness, int tamanho_populacao) {
    int idx1 = rand() % tamanho_populacao;
    int idx2 = rand() % tamanho_populacao;
    return fitness[idx1] < fitness[idx2] ? idx1 : idx2;
}

int selecao_torneio_com_elitismo(int N,int populacao[][N], float *fitness, int nova_populacao[][N], int taxa_elitismo) {
    // Determinar quantos indivíduos serão preservados pelo elitismo
    int num_elite = (TAMANHO_POPULACAO * taxa_elitismo);

    // Preservar os melhores indivíduos
    int indices[TAMANHO_POPULACAO];
    for (int i = 0; i < TAMANHO_POPULACAO; i++) indices[i] = i;

    // Ordenar índices pelo fitness (crescente)
    for (int i = 0; i < TAMANHO_POPULACAO - 1; i++) {
        for (int j = i + 1; j < TAMANHO_POPULACAO; j++) {
            if (fitness[indices[i]] > fitness[indices[j]]) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }

    // Copiar os indivíduos de elite para a nova população
    for (int i = 0; i < num_elite; i++) {
        for (int j = 0; j < N; j++) {
            nova_populacao[i][j] = populacao[indices[i]][j];
        }
    }
    for (int i = num_elite; i < TAMANHO_POPULACAO; i++) {
        int idx1 = rand() % TAMANHO_POPULACAO;
        int idx2 = rand() % TAMANHO_POPULACAO;

        int vencedor_idx = (fitness[idx1] < fitness[idx2]) ? idx1 : idx2;

        for (int j = 0; j < N; j++) {
            nova_populacao[i][j] = populacao[vencedor_idx][j];
        }
    }
    return num_elite;
}

int selecao_roleta(float *fitness, int tamanho_populacao) {
    float soma_fitness = 0.0;

    // Calcula o inverso do fitness para todos os indivíduos para peso maior
    for (int i = 0; i < tamanho_populacao; i++) {
        soma_fitness += 1 / fitness[i];
    }

    float escolha = ((float)rand() / RAND_MAX) * soma_fitness; //[0,1] * soma fitness
    float acumulado = 0.0;

    for (int i = 0; i < tamanho_populacao; i++) {
        acumulado += 1 / fitness[i];//Maior probabilidade de serem escolhidos
        if (acumulado >= escolha) {
            return i;
        }
    }
    return tamanho_populacao - 1;
}

void crossover_dois_pontos(int *pai1, int *pai2, int *filho, int N, float *moedas, float V) {
    int ponto_corte1 = rand() % N;
    int ponto_corte2 = rand() % N;

    // Garantir que ponto_corte1 < ponto_corte2
    if (ponto_corte1 > ponto_corte2) {
        int temp = ponto_corte1;
        ponto_corte1 = ponto_corte2;
        ponto_corte2 = temp;
    }

    // Realiza o crossover
    for (int i = 0; i < N; i++) {               // 0 ate p1 pai1,depois pais2 e depois p2 ate n pai1
        if (i < ponto_corte1 || i >= ponto_corte2) {
            filho[i] = pai1[i];
        } else {
            filho[i] = pai2[i];
        }
    }

    reparar_solucao(filho, moedas, N, V);
}

void crossover_uniforme_adaptativo(int *pai1, int *pai2, int *filho, int N, float fitness_pai1, float fitness_pai2, float *moedas, float V) {
    // Calcula a probabilidade adaptativa baseada no fitness
    float prob_pai1 = fitness_pai2 / (fitness_pai1 + fitness_pai2); // Pai mais apto tem menor probabilidade de perda

    for (int i = 0; i < N; i++) {
        // Decide de qual pai herdar o gene
        if ((rand() / (float)RAND_MAX) < prob_pai1) {
            filho[i] = pai1[i];
        } else {
            filho[i] = pai2[i];
        }
    }

    reparar_solucao(filho, moedas, N, V); // Garantir viabilidade da solução
}

void mutacao_simples(int *solucao, int N, float *moedas, float V) {

    int idx = rand() % N;
    solucao[idx] +=  (rand() % 3) - 1; // Apenas pequenas alterações[-1,0,1]

    if (solucao[idx] < 0)
        solucao[idx] = 0;

    reparar_solucao(solucao, moedas, N, V);
}

void mutacao_troca(int *solucao, int N, float *moedas, float V) {
    int idx1 = rand() % N;
    int idx2 = rand() % N;

    while (idx1 == idx2) {
        idx2 = rand() % N;
    }

    int temp = solucao[idx1];
    solucao[idx1] = solucao[idx2];
    solucao[idx2] = temp;

    // para mais diversidade 50% de ocorrer mais uma mutacao de incremento/decremento
    if (rand() % 2 == 0) {
        int idx3 = rand() % N;
        solucao[idx3] += (rand() % 3) - 1;
        if (solucao[idx3] < 0)
            solucao[idx3] = 0;
    }

    reparar_solucao(solucao, moedas, N, V);
}

void algoritmo_evolutivo(float *moedas, int N, float V, int max_geracoes, int *solucao_inicial, float penalidade) {
    int populacao[TAMANHO_POPULACAO][N];
    int nova_populacao[TAMANHO_POPULACAO][N];
    float fitness[TAMANHO_POPULACAO];
    float melhor_fitness = INFINITY;
    int melhor_solucao[N];

    // Inicializa a população
    for (int i = 0; i < TAMANHO_POPULACAO; i++) {
        if (i == 0) { // Primeira posição recebe a solução inicial
            for (int j = 0; j < N; j++) {
                populacao[i][j] = solucao_inicial[j];
            }
        } else { // Restante da população recebe soluções aleatórias
            sol_aleatoria(moedas, populacao[i], N, V);
        }
        fitness[i] = avaliar_solucao(moedas, populacao[i], N, V, penalidade);
    }

    // Atualiza o melhor fitness inicial
    for (int i = 0; i < TAMANHO_POPULACAO; i++) {
        if (fitness[i] < melhor_fitness) {
            melhor_fitness = fitness[i];
            for (int j = 0; j < N; j++) {
                melhor_solucao[j] = populacao[i][j];
            }
        }
    }

    int geracoes = 0;
    int tipo_mutacao;
    //printf("Qual tipo de mutacao deseja usar?\n Simples(0) Troca(1)\n");
    //scanf("%d", &tipo_mutacao);
    // Loop principal do algoritmo evolutivo
    while (geracoes < max_geracoes) {
        geracoes++;

        int num_elite =  selecao_torneio_com_elitismo(N, populacao, fitness, nova_populacao, 0.15); // 15% elitismo
        for (int i = num_elite; i < TAMANHO_POPULACAO; i++) {//para nova geracao ter o mesmo tamanho da original
            int pai1_idx = selecao_torneio(fitness, TAMANHO_POPULACAO);
            int pai2_idx = selecao_roleta(fitness, TAMANHO_POPULACAO);
            //
            // Recombinação
            if ((rand() / (float)RAND_MAX) < TAXA_RECOMBINACAO) {
                if (rand() % 2 == 0) {
                    crossover_dois_pontos(populacao[pai1_idx], populacao[pai2_idx], nova_populacao[i], N, moedas, V);
                }
                else {
                    crossover_uniforme_adaptativo(populacao[pai1_idx], populacao[pai2_idx], nova_populacao[i], N, fitness[pai1_idx], fitness[pai2_idx], moedas,V);
                }
            }


            // Mutação
            if ((rand() / (float)RAND_MAX) < TAXA_MUTACAO) {
                //
                if (rand() % 2 == 0) {
                    mutacao_simples(nova_populacao[i], N, moedas, V);
                } else {
                    mutacao_troca(nova_populacao[i], N, moedas, V);
                }
            }

            // Avaliação
            fitness[i] = avaliar_solucao(moedas, nova_populacao[i], N, V, penalidade);

            // Atualiza o melhor fitness
            if (fitness[i] < melhor_fitness) {
                melhor_fitness = fitness[i];
                for (int j = 0; j < N; j++) {
                    melhor_solucao[j] = nova_populacao[i][j];
                }
            }
        }

        // Substitui a população
        for (int i = 0; i < TAMANHO_POPULACAO; i++) {
            for (int j = 0; j < N; j++) {
                populacao[i][j] = nova_populacao[i][j];
            }
        }

        // Depuração
        // printf("Melhor fitness na geracao %d: %.2f\n", geracoes, melhor_fitness);
    }
    // Calcula o fitness final diretamente

    // Exibe a melhor solução encontrada
    printf("\nMelhor solucao encontrada:\n");
    for (int i = 0; i < N; i++) {
        printf("Moedas de %.2f: %d unidades\n", moedas[i], melhor_solucao[i]);
    }
    printf("Fitness final: %.2f\n", melhor_fitness);
    for (int i = 0; i < N; i++) {
        solucao_inicial[i] = melhor_solucao[i];
    }
}
//                              Hibrido
void algoritmo_hibrido(float *moedas, int N, float V, int max_geracoes, int max_iter, int *solucao_inicial, float penalidade,int tipo_vizinhanca) {
    int populacao[TAMANHO_POPULACAO][N];
    int nova_populacao[TAMANHO_POPULACAO][N];
    float fitness[TAMANHO_POPULACAO];
    int melhor_solucao[N];
    float melhor_fitness = INFINITY;

    // Inicializa a população
    for (int i = 0; i < TAMANHO_POPULACAO; i++) {
        if (i == 0) { // Primeira posição recebe a solução inicial
            for (int j = 0; j < N; j++) {
                populacao[i][j] = solucao_inicial[j];
            }
        } else { // Restante da população recebe soluções aleatórias
            sol_aleatoria(moedas, populacao[i], N, V);
        }
        fitness[i] = avaliar_solucao(moedas, populacao[i], N, V, penalidade);
    }

    int geracoes = 0;

    while (geracoes < max_geracoes) {
        geracoes++;

        // Seleção por torneio com elitismo
        int num_elite = selecao_torneio_com_elitismo(N, populacao, fitness, nova_populacao, 0.15);

        for (int i = num_elite; i < TAMANHO_POPULACAO; i++) {
            // Seleciona pais
            int pai1_idx = selecao_torneio(fitness, TAMANHO_POPULACAO);
            int pai2_idx = selecao_roleta(fitness, TAMANHO_POPULACAO);

            // Recombinação de dois pontos
            if ((rand() / (float)RAND_MAX) < TAXA_RECOMBINACAO) {
                crossover_dois_pontos(populacao[pai1_idx], populacao[pai2_idx], nova_populacao[i], N, moedas, V);
            }

            // Mutação por troca
            if ((rand() / (float)RAND_MAX) < TAXA_MUTACAO) {
                mutacao_troca(nova_populacao[i], N, moedas, V);
            }

            // Avalia a nova solução
            fitness[i] = avaliar_solucao(moedas, nova_populacao[i], N, V, penalidade);
        }

        // Substitui a população com a nova geração
        for (int i = 0; i < TAMANHO_POPULACAO; i++) {
            for (int j = 0; j < N; j++) {
                populacao[i][j] = nova_populacao[i][j];
            }
        }

        // Aplica Trepa Colinas com incremento ao melhor indivíduo
        int melhor_idx = 0;
        for (int i = 1; i < TAMANHO_POPULACAO; i++) {
            if (fitness[i] < fitness[melhor_idx]) {
                melhor_idx = i;
            }
        }

        // Copia o melhor indivíduo para uma solução temporária
        int melhor_individuo[N];
        for (int i = 0; i < N; i++) {
            melhor_individuo[i] = populacao[melhor_idx][i];
        }

        // Aplica Trepa Colinas com Incremento
        trepa_colinas(moedas, melhor_individuo, N, V, penalidade, max_iter,tipo_vizinhanca);

        // Avalia a solução pós-Trepa Colinas
        float fitness_incremento = avaliar_solucao(moedas, melhor_individuo, N, V, penalidade);

        // Substitui o melhor da população se a solução do Trepa Colinas for melhor
        if (fitness_incremento < fitness[melhor_idx]) {
            for (int i = 0; i < N; i++) {
                populacao[melhor_idx][i] = melhor_individuo[i];
            }
            fitness[melhor_idx] = fitness_incremento;
        }
    }

    // Seleciona a melhor solução da última geração
    int melhor_idx = 0;
    for (int i = 1; i < TAMANHO_POPULACAO; i++) {
        if (fitness[i] < fitness[melhor_idx]) {
            melhor_idx = i;
        }
    }

    for (int i = 0; i < N; i++) {
        solucao_inicial[i] = populacao[melhor_idx][i];
    }

    printf("\nMelhor solucao hibrida encontrada:\n");
    for (int i = 0; i < N; i++) {
        printf("Moedas de %.2f: %d unidades\n", moedas[i], solucao_inicial[i]);
    }
    printf("Fitness final: %.2f\n", fitness[melhor_idx]);
}

void algoritmo_hibrido2(float *moedas, int N, float V, int max_geracoes, int max_iter, int *solucao_inicial, float penalidade, int tipo_vizinhanca) {
    int populacao[TAMANHO_POPULACAO][N];
    int nova_populacao[TAMANHO_POPULACAO][N];
    float fitness[TAMANHO_POPULACAO];
    int melhor_solucao[N];
    float melhor_fitness = INFINITY;

    // Inicializa a população
    for (int i = 0; i < TAMANHO_POPULACAO; i++) {
        if (i == 0) { // Primeira posição recebe a solução inicial
            for (int j = 0; j < N; j++) {
                populacao[i][j] = solucao_inicial[j];
            }
        } else { // Restante da população recebe soluções aleatórias
            sol_aleatoria(moedas, populacao[i], N, V);
        }
        fitness[i] = avaliar_solucao(moedas, populacao[i], N, V, penalidade);
    }

    int geracoes = 0;

    while (geracoes < max_geracoes) {
        geracoes++;

        // Seleção por torneio com elitismo
        int num_elite = selecao_torneio_com_elitismo(N, populacao, fitness, nova_populacao, 0.15);

        for (int i = num_elite; i < TAMANHO_POPULACAO; i++) {
            // Seleciona pais
            int pai1_idx = selecao_torneio(fitness, TAMANHO_POPULACAO);
            int pai2_idx = selecao_roleta(fitness, TAMANHO_POPULACAO);

            // Recombinação Uniforme Adaptativa
            if ((rand() / (float)RAND_MAX) < TAXA_RECOMBINACAO) {
                crossover_uniforme_adaptativo(populacao[pai1_idx], populacao[pai2_idx], nova_populacao[i], N, fitness[pai1_idx], fitness[pai2_idx], moedas, V);
            }

            // Mutação Simples
            if ((rand() / (float)RAND_MAX) < TAXA_MUTACAO) {
                mutacao_simples(nova_populacao[i], N, moedas, V);
            }

            // Avalia a nova solução
            fitness[i] = avaliar_solucao(moedas, nova_populacao[i], N, V, penalidade);
        }

        // Substitui a população com a nova geração
        for (int i = 0; i < TAMANHO_POPULACAO; i++) {
            for (int j = 0; j < N; j++) {
                populacao[i][j] = nova_populacao[i][j];
            }
        }

        // Aplica Trepa Colinas ao melhor indivíduo
        int melhor_idx = 0;
        for (int i = 1; i < TAMANHO_POPULACAO; i++) {
            if (fitness[i] < fitness[melhor_idx]) {
                melhor_idx = i;
            }
        }

        // Copia o melhor indivíduo para uma solução temporária
        int melhor_individuo[N];
        for (int i = 0; i < N; i++) {
            melhor_individuo[i] = populacao[melhor_idx][i];
        }

        // Aplica Trepa Colinas com o tipo de vizinhança escolhido
        trepa_colinas(moedas, melhor_individuo, N, V, penalidade, max_iter, tipo_vizinhanca);

        // Avalia a solução pós-Trepa Colinas
        float fitness_vizinho = avaliar_solucao(moedas, melhor_individuo, N, V, penalidade);

        // Substitui o melhor da população se a solução do Trepa Colinas for melhor
        if (fitness_vizinho < fitness[melhor_idx]) {
            for (int i = 0; i < N; i++) {
                populacao[melhor_idx][i] = melhor_individuo[i];
            }
            fitness[melhor_idx] = fitness_vizinho;
        }
    }

    // Seleciona a melhor solução da última geração
    int melhor_idx = 0;
    for (int i = 1; i < TAMANHO_POPULACAO; i++) {
        if (fitness[i] < fitness[melhor_idx]) {
            melhor_idx = i;
        }
    }

    for (int i = 0; i < N; i++) {
        solucao_inicial[i] = populacao[melhor_idx][i];
    }

    printf("\nMelhor solução híbrida adaptativa encontrada:\n");
    for (int i = 0; i < N; i++) {
        printf("Moedas de %.2f: %d unidades\n", moedas[i], solucao_inicial[i]);
    }
    printf("Fitness final: %.2f\n", fitness[melhor_idx]);
}
