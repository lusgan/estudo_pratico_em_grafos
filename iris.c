#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <windows.h>
#include "funcoes.h"

void estudo_componentes_conexos() { /* Limiares a se estudar: limiar | componentes
                                     *                        0.086  | 49 
                                     *                        0.196  | 50
                                     *                        0.312  | 50
                                     *                        0.313  | 150
                                    */
    float limiar_original = limiar;

    for (float i = 0; i < 0.4; i += 0.001) {
        limiar = i;
        
        printf("Limiar: %.3f\n", limiar);
        imprime_lista_de_adjacencias();
        int numComponentes = contarComponentesConexos(0);
        printf("Número de Componentes Conexos: %d\n\n", numComponentes);

        /* 
         * Visualizar grafos gerados com cada novo limiar computado
        int status = system("python displayGrafo.py");
        sleep(5);
        if (status == 0)
            printf("Programa executado com sucesso!\n");
        else
            printf("Erro ao executar o programa!\n");
        */
    }
    // printf("\n");

    limiar = limiar_original;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf ("Número de argumentos errado!\n");
        return EXIT_FAILURE;
    }

    limiar = atof(argv[1]);

    int opcao = -1;
    int vertice;
    float novo_limiar;
    while (opcao != 0) {
        printf("[0] Sair\n[1] Ler IrisDataset.csv\n[2] Carregar Grafo.csv\n[3] Contar componentes conexos (vértice)\n[4] Estudo de componentes conexos\n[5] Alterar o limiar\n");
        printf("\nOpção: ");
        scanf("%d", &opcao);
        printf("\n");
        switch (opcao) {
            case 0:
                printf("Finalizando o programa...\n");

                break;
            case 1:
                imprime_lista_de_adjacencias();

                break;
            case 2:
                carregar_grafo();

                break;
            case 3:
                printf("Vértice: ");
                scanf("%d", &vertice);
                printf("Componentes: %d\n", contarComponentesConexos(vertice));

                break;
            case 4:
                estudo_componentes_conexos();

                break;
            case 5:
                printf("Novo limiar: ");
                scanf("%f", &limiar);
                printf("\n");
                imprime_lista_de_adjacencias();

                break;
            default:
                printf("Op��o inv�lida!\n");

                break;
        }
    }

    return EXIT_SUCCESS;
}
