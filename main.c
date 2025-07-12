#include "algoritmo.h"
//3 alterar para qual selecao usar
//4 alterar para qual crossover usar
//5 alterar para qual mutacao usar
//6 penalizacao vs reparacao(nao faco a minima ideia) talvez desligar a reparacao??!?!?

void main() {
    while (1) {
        int instances;
        printf("Qual Instancia deseja utilizar?\n   (1) (2) (3) (4) (5)\n");
        scanf("%d", &instances);
        FILE *file;
        if (instances == 1) {
            file = fopen("./instances/file1.txt", "r");
        } else if (instances == 2) {
            file = fopen("./instances/file2.txt", "r");
        } else if (instances == 3) {
            file = fopen("./instances/file3.txt", "r");
        } else if (instances == 4) {
            file = fopen("./instances/file4.txt", "r");
        } else if (instances == 5) {
            file = fopen("./instances/file5.txt", "r");
        } else {
            printf("Arquivo de instancias nao encontrado!!!\n");
            exit(EXIT_FAILURE);
        }
        if (file == NULL) {
            printf("Erro na abertura do arquivo\n");
            exit(EXIT_FAILURE);
        }

        //srand(time(NULL));
        float V = 0; // Valor a dar troco
        float moeda1 = 0.0, moeda2 = 0.0, moeda3 = 0.0, moeda4 = 0.0, moeda5 = 0.0, moeda6 = 0.0, moeda7 = 0.0, moeda8 = 0.0;
        int nmoedas = 0;
        float penalidade = 10.0;
        char linha[200];
        int max_iter = 100;
        int max_geracoes = 2500;

        // Ler ficheiro
        int contador = 0;
        while (fgets(linha, sizeof(linha), file)) {
            contador++;
            if (contador == 1) {
                if (sscanf(linha, "%d %f", &nmoedas, &V) == 2) {
                    printf("NMoeadas: %d, Valor: %f\n", nmoedas, V);
                }
            } else if (contador == 2) {
                if (sscanf(linha, "%f %f %f %f %f %f %f %f", &moeda1, &moeda2, &moeda3, &moeda4, &moeda5, &moeda6, &moeda7, &moeda8) > 2) {
                    printf("Moeda1: %.2f, Moeda2: %.2f, Moeda3: %.2f , Moeda4: %.2f\nMoeda5: %.2f, Moeda6: %.2f, Moeda7: %.2f, Moeda8: %.2f\n",
                           moeda1, moeda2, moeda3, moeda4, moeda5, moeda6, moeda7, moeda8);
                }
            } else if (contador > 2) {
                break;
            }
        }
        fclose(file);

        // Adicionar moedas no array se for diferente de 0.0
        float moedas_criacao_array[8] = {moeda1, moeda2, moeda3, moeda4, moeda5, moeda6, moeda7, moeda8};
        int validas_idx = 0;
        float moedas_validas[8] = {0.0};
        for (int i = 0; i < nmoedas; i++) {
            if (moedas_criacao_array[i] != 0.0) {
                moedas_validas[validas_idx++] = moedas_criacao_array[i];
            }
        }
        float moedas[nmoedas];
        for (int i = 0; i < nmoedas; i++) {
            moedas[i] = 0.0;
        }
        for (int i = 0; i < validas_idx; i++) {
            moedas[i] += moedas_validas[i];
        }

        int metodo;
        printf("Qual metodo deseja usar?\n(1) para Trepa Colinas (2) Para Algoritmo Evolutivo (3) Para Algoritmo Hibrido (4) Para Algoritmo Hibrido2\n");
        scanf("%d", &metodo);
        int tipo_vizinhanca;
        if(metodo==1 || metodo==3 || metodo==4) {
            printf("Qual metodo deseja usar?\n Incremento(0) Troca(1) Substituicao(2)\n");
            scanf("%d", &tipo_vizinhanca);
        }

        float melhor_fitness_global = INFINITY;
        float soma_fitness = 0.0;
        for (int exec = 0; exec < 10; exec++) {
            int solucao[nmoedas];
            sol_aleatoria(moedas, solucao, nmoedas, V);
            printf("Execucao %d:\n", exec + 1);
            printf("Solucao inicial gerada:\n");
            for (int i = 0; i < nmoedas; i++) {
                printf("Moeda de %.2f: %d unidades\n", moedas[i], solucao[i]);
            }
            float fitness_atual = avaliar_solucao(moedas, solucao, nmoedas, V, penalidade);
            printf("Fitness atual: %.2f\n", fitness_atual);
            if (metodo == 1) {

                printf("Iniciando Trepa Colinas....\n");
                trepa_colinas(moedas, solucao, nmoedas, V, penalidade, max_iter,tipo_vizinhanca);
            } else if (metodo == 2) {
                printf("Iniciando Algoritmo Evolutivo....\n");
                algoritmo_evolutivo(moedas, nmoedas, V, max_geracoes, solucao, penalidade);
            } else if (metodo == 3) {
                printf("Iniciando Algoritmo Hibrido 1....\n");
                algoritmo_hibrido(moedas, nmoedas, V, max_geracoes, max_iter,solucao, penalidade,tipo_vizinhanca);
            } else if (metodo == 4) {
                printf("Iniciando Algoritmo Hibrido 2....\n");
                algoritmo_hibrido2(moedas, nmoedas, V, max_geracoes, max_iter,solucao, penalidade,tipo_vizinhanca);
            }else {
                printf("Metodo Invalido!!!\n");
                exit(EXIT_FAILURE);
            }

           fitness_atual = avaliar_solucao(moedas, solucao, nmoedas, V, penalidade);
            printf("Fitness da execucao %d: %.2f\n", exec + 1, fitness_atual);

            soma_fitness += fitness_atual;
            if (fitness_atual < melhor_fitness_global) {
                melhor_fitness_global = fitness_atual;
            }
        }

        float MBF = soma_fitness / 10.0;
        printf("\nResultados finais:\n");
        printf("Melhor Fitness Global: %.2f\n", melhor_fitness_global);
        printf("MBF : %.2f\n", MBF);
    }
}
