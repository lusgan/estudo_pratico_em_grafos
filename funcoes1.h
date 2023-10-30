#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

typedef struct Iris {
    float sep_len;
    float sep_width;
    float pet_len;
    float pet_width;
    char *variety;
} Iris;

float distancia_euclidiana(Iris *flor1, Iris *flor2){
    float DE = sqrt(pow((flor1->sep_len - flor2->sep_len), 2) +
                     pow((flor1->sep_width - flor2->sep_width), 2) +
                     pow((flor1->pet_len - flor2->pet_len), 2) +
                     pow((flor1->pet_width - flor2->pet_width), 2));
    return DE;
}

float limiar = 0.3;

Iris *lista_de_vertices[150];
char lista_De_variedades[150][100];
float tabela_euclidiana_Norm[150][150];

void ler_arquivo_CSV() {
    FILE *file = fopen("IrisDataset.csv","r");

    char linha[100];
    char *token;

    if (file == NULL) {
        printf("Erro ao abrir o arquivo");
        return;
    }

    int conta_iris = 0;

    while (fgets(linha, 100, file) != NULL) {

        if (conta_iris >= 1) {
            Iris *flor = malloc(sizeof(Iris));

            token = strtok(linha , ",");
            flor->sep_len = atof(token);

            token = strtok(NULL, ",");
            flor->sep_width = atof(token);

            token = strtok(NULL, ",");
            flor->pet_len = atof(token);

            token = strtok(NULL, ",");
            flor->pet_width = atof(token);

            token = strtok(NULL,",");
            strcpy(lista_De_variedades[conta_iris - 1], token);
            flor->variety = lista_De_variedades[conta_iris - 1];


            lista_de_vertices[conta_iris - 1] = flor;
        }

        conta_iris++;
    }

    fclose(file);

    // printf("A quantidade de linhas lidas é: %d\n\n", conta_iris);
}


float tabela_euclidiana[150][150];

void criar_tabela_euclidiana(){

    for (int i = 0; i < 150; i++) {
        for (int j = 0; j < 150; j++) {
            tabela_euclidiana[i][j] = distancia_euclidiana(lista_de_vertices[i],lista_de_vertices[j]);
        }
    }

}

float menorValor_da_Linha(int linha){
    float menor = tabela_euclidiana[linha][linha]+10;
    for(int i = 0; i<150;i++){
        if(tabela_euclidiana[linha][i]<menor && i!=linha){
            menor = tabela_euclidiana[linha][i];
        }
    }
    return menor;

}

float maiorValor_da_Linha(int linha){
    float maior = tabela_euclidiana[linha][linha]-10;
    for(int i = 0; i<150;i++){
        if(tabela_euclidiana[linha][i]>maior){
            maior = tabela_euclidiana[linha][i];
        }
    }
    return maior;

}

void criar_tabela_euclidianaNorm(){

    for(int i = 0; i<150;i++){
        for(int j = 0; j<150; j++){

            if(i!=j){
                float DE = distancia_euclidiana(lista_de_vertices[i],lista_de_vertices[j]);
                tabela_euclidiana_Norm[i][j] = (DE - menorValor_da_Linha(i))/(maiorValor_da_Linha(i) - menorValor_da_Linha(i));
            }

            else{
                tabela_euclidiana_Norm[i][j] = 10;
            }

        }
    }
}

void salvar_grafo() {
    FILE *file = fopen("Grafo.csv","w");

    char linha[100];
    char *token;

    if (file == NULL) {
        printf("Erro ao abrir o arquivo");
        return;
    }

    for (int i = 0; i < 150; i++) {
        for (int z = i; z < 150; z++) {
            if (i != z && tabela_euclidiana_Norm[i][z] <= limiar)
                fprintf(file, "%d %d\n",i ,z);
        }
    }

    fclose(file);
}

void carregar_grafo() {
    FILE *file = fopen("Grafo.csv", "r");

    char linha[100];

    if (file == NULL) {
        printf("Erro ao abrir o arquivo");
        return;
    }

    fgets(linha, sizeof(linha), file);
    while (fgets(linha, sizeof(linha), file) != NULL) {
        char *token = strtok(linha, " ");

        int valor = atoi(token);
        printf("%d ",valor);

        token = strtok(NULL, " ");
        valor = atoi(token);

        printf("%d\n",valor);
    }

    printf("\n");

    fclose(file);
}


void imprime_lista_de_adjacencias() {

    ler_arquivo_CSV();
    criar_tabela_euclidiana();
    criar_tabela_euclidianaNorm();
    salvar_grafo();

    int counter = 0;

    for (int i = 0; i < 150; i++) {
        for (int z = 0; z < 150; z++) {
            if (i!=z && tabela_euclidiana_Norm[i][z]<=limiar ) { // ignora os elementos vazios "0" da lista
                // printf("%d %d\n",i , z);
                counter++;
            }
        }
    }
    printf("Arestas = %d\n\n", counter);
}

int** criar_matriz_adjacencias( ){
    ler_arquivo_CSV();
    criar_tabela_euclidiana();
    criar_tabela_euclidianaNorm();

    int numVertices = 150;
    int** matriz = (int**)malloc(numVertices * sizeof(int*));

    for (int i = 0; i < numVertices; i++) {
        matriz[i] = (int*)malloc(numVertices * sizeof(int));
        for (int j = 0; j < numVertices; j++) {

            if(i!=j && tabela_euclidiana_Norm[i][j] <= limiar){
                matriz[i][j] = 1;
            }

        }
    }
    return matriz;
}

// Funcao que retorna o tamanho de uma pilha, onde caso pilha esteja vazia seus elementos pilha[0], pilha[1], pilha[2]... terao todos valor = -1.
int len(int v[]){
    int count = 0;
    for(int i = 0; i<150;i++){
       if(v[i]!=-1){
            count++;
       }
    }
    return count;
}

// Cria pilha com todos os elementos iguais a -1
int *criar_pilha(){
    int *v = malloc(sizeof(int)*150);

    for(int i = 0; i<150;i++){
        v[i] = -1;
    }
    return v;
}

void push(int *pilha, int valor){
    pilha[len(pilha)] = valor;
}

void pop(int *pilha){
    if(len(pilha)==0){
            printf("Pilha vazia!");
            return;
    }
    pilha[len(pilha)-1] = -1;
}

int visitados[2][150]; //linha 0 -> visitado : T/F         linha 1 -> vertice predecessor ao visitado : int v
int** tabela_de_conexoes;

int DFS(int vertice) {
    /*int** */ tabela_de_conexoes = criar_matriz_adjacencias();

    visitados[0][vertice] = 1; // Define o vertice como "true" para visitado
    int comp_conexos = 1;
    int *pilha = criar_pilha();
    push(pilha, vertice);

    while(len(pilha)>0){
        int topo = pilha[len(pilha)-1];
        int ha_mais_vizinhos = 0;

        for(int i = 0; i<150;i++){
            if(tabela_de_conexoes[topo][i] == 1 && visitados[0][i] == 0){
                visitados[0][i] = 1;
                visitados[1][i] = topo;
                push(pilha,i);
                ha_mais_vizinhos = 1;
                comp_conexos++;
            }
        }

        if(ha_mais_vizinhos == 0){
            pop(pilha);
        }

    }
    return comp_conexos;
}

void zerarVisitados(){
    //zerar lista de visitados essa funcao eh util para contar componentes conexos de diferentes clusters
    //pois a variavel global precisa ser zerada
    for(int i = 0; i<150; i++){
        visitados[0][i] = 0;
        visitados[1][i] = 0;
    }
}

int contarComponentesConexos(int v) {

    int num_comps = 0;
    DFS(v);

    for(int i = 0; i<150;i++){
        if(visitados[0][i] == 1){
            num_comps++;
        }
    }

    return num_comps; // Retorna o n�mero de componentes conexos
}

int verticeMaisProximo(int v, int lista_de_verticesRaiz[], int tam){
    int distancias_euclidianas[tam];
    for(int i = 0; i<tam;i++){
        distancias_euclidianas[i] = tabela_euclidiana_Norm[v][lista_de_verticesRaiz[i]]; // retorna a distancia euclidiana entre v e o vertice na posicao lista_de_vertices[i]
    }

    int menor = distancias_euclidianas[0];
    int pos;
    for(int i = 0;i<tam;i++){
        if(distancias_euclidianas[i] < menor){
            menor = distancias_euclidianas[i];
            pos = i; // posicao do vertice mais proximo
        }
    }

    return lista_de_verticesRaiz[pos];

}

void histograma(){
    zerarVisitados();
    int Qtd_clusters = 0;
    int clusters[150][2]; // clusters[vertice][qtd de componentes conexos] [[0,50], [50,100]...]

    printf("------------HISTOGRAMA----------------\n");
    for(int i = 0; i<150;i++){
        if(visitados[0][i] == 0){
            int qtd_componentes_conexos = DFS(i);
            printf("raiz = Vertice %d, componentes conexos = %d\n",i,qtd_componentes_conexos);
            clusters[Qtd_clusters][0] = i;
            clusters[Qtd_clusters][1] = qtd_componentes_conexos;

            Qtd_clusters++;
        }
    }
    printf("--------------------------------------\n");
    printf("quantidade de clusters = %d\n\n",Qtd_clusters);

    int lista_de_verticesRaiz[Qtd_clusters];
    for(int j= 0; j<Qtd_clusters;j++){
        lista_de_verticesRaiz[j] = clusters[j][0];
    }

    for (int i = 0; i<Qtd_clusters;i++){
        if(clusters[i][1]<40){
            int vertice_mais_proximo = verticeMaisProximo(clusters[i][0],lista_de_verticesRaiz,Qtd_clusters);
            tabela_de_conexoes[clusters[i][0]][vertice_mais_proximo] = 1;
        }
    }

}