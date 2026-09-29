#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// cd Documents\Programacion\C_UAB\treball

//EXECUCIÓ
// .\vuit_reines.exe mida algorisme fitxer_entrada.txt fitxer_sortida.txt
char* algorismes[] = {"aleatori1", "aleatori2", "aleatori3", "backtrack", "matriu"};

void printVector(int n, int vector[]);
void generarPermutacio(int n, int vector[n]);
void generarPermutacio2(int n, int vector[n]);
void generarPermutacio3(int n, int vector[n]);

int** assigna_matriu(int m, int n);
void allibera_matriu(int** matriu, int m);

void generar_simetries(int n, int** vector);
int es_solucio(int n, int vector[n]);
int solucio_nova(int n, int vector[n]);
int compare_vectors(int n, int v1[], int v2[]);

int classifica_orbita(int n, int* v);

void vector_aleatori(int n);

int compareVectors(int n, int v1[], int v2[]);

int main(int argc, char* argv[]) {
    // int n = 5;
    // int v[] = {5, 2, 4, 1, 3};
    // int** sim = assigna_matriu(8, n);
    // for(int i = 0; i<n; i++){
    //     sim[0][i] = v[i];
    // }

    // generar_simetries(n, sim);

    // for(int i = 0; i<8; i++){
    //     printf("%d. ", i);
    //     printVector(n, sim[i]);
    // }
    // return 1;

    // int v[] = {5,2,4,1,3};
    int v[] = {2,5,3,1,4};
    printf("%d", classifica_orbita(5, v));
}

int classifica_orbita(int n, int* v) {
    int r[n], r2[n];
    for(int i = 0; i<n; i++) {
        r[v[i]-1] = n - i;
        r2[i] = n +1 - v[i];
    }

    printVector(n, r);
    printVector(n, r2);
    if (compare_vectors(n, r, v)) 
        return 0;
    if (compare_vectors(n, r2, v)) 
        return 1;

    return 2;
}

void generar_simetries(int n, int** sim) {
    //les simetries son: de rotacio, de translacio

    //int** sim = assigna_matriu(8, n);
    for (int i = 0; i<n; i++){
        ///1. miralls
        // horitzontal
        sim[1][i] = n + 1 - sim[0][i];
        // vertical
        sim[2][i] = sim[0][n-i-1];
        // diagonal1
        sim[3][sim[0][i]] = i;
        // diagonal2
        sim[4][n - sim[0][i]] = n - i-1;
        ///2. Rotacions
        //90º
        sim[5][i] = n + 1 - sim[0][n - i];
        //180º
        sim[6][n - i-1] = n - sim[0][i];
        //270º
        sim[7][sim[0][i]] = n - i-1;
    }
    // int simetries_totals[8][n];
    // *simetries_totals[1] = *vector;
    // // comprovar que les simetries no coincideixen
    // for (int i = 1; i < 8; i++)
    // {
    //     int add = 1;
    //     for (int j = i+1; j < 8; j++)
    //     {
    //         if (compareVectors(n, sim[i], sim[j]))
    //         {
    //             add = 0;
    //             break;
    //         }
    //     }
    //     if (add){
    //         *simetries_totals[i] = *sim[i];
    //     }
    // }
    
}

int solucio_nova(int n, int vector[n]) {
    // ordenar les solucions a la llista output per fer una cerca més acurada
    return 1;
}

void vector_aleatori(int n) {
    int vector[n];
    for(int i =0; i<n*n && !es_solucio(n, vector); i++){
        generarPermutacio3(n, vector);
    }
    printVector(n, vector);
}

int es_solucio(int n, int vector[n]) {
    for (int i = 0; i < n; i++)
    {
        for (int  j = i+1; j < n; j++)
        {
            if ((vector[i] == vector[j]) || (abs(vector[i] - vector[j]) == abs(i - j))) {
                return 0;
            }
        }
    }
    return 1;
}

int compareVectors(int n, int v1[], int v2[]){
    for (int i = 0; i<n; i++){
        if (v1[i] != v2[i]){
            return 0;
        }
    }
    return 1;
}

void printVector(int n, int vector[]) {
    printf("v = (%d", vector[0]);
    for (int i = 1; i<n; i++){
        printf(", %d", vector[i]);
    }
    printf(");\n");
}

void generarPermutacio(int n, int vector[n]){
    int aux[n];
    for (int i = 0; i<n; i++) {
        aux[i] = i;
    }

    for (int i = n; i>0; i--){
        int k = rand() % i;
        vector[i-1] = aux[k];
        if (k != i) {
            aux[k] = aux[i];
        }
    }
}

void generarPermutacio2(int n, int vector[n]){
    for (int i = 0; i<n; i++) {
        vector[i] = i;
    }

    for (int i = 0; i<n; i++){
        int j = rand() % n;
        int aux = vector[j];
        vector[j] = vector[i];
        vector[i] = aux;
    }
}

void generarPermutacio3(int n, int vector[n]){
    vector[0] = 1;
    for (int i = 1; i<n; i++) {
        int j = rand() % (i+1);
        vector[i] = vector[j];
        vector[j] = i+1;
    }
}


int compare_vectors(int n, int v1[], int v2[]){
    for (int i = 0; i<n; i++){
        if (v1[i] != v2[i]){
            return 0;
        }
    }
    return 1;
}

int** assigna_matriu(int m, int n) {
    int** matriu = (int**) calloc(m, sizeof(int*));
    if (matriu == NULL) {
        printf("MEMORY ERROR!");
        return NULL;
    }
    for (int i = 0; i<m; i++) {
        matriu[i] = (int*) calloc(n, sizeof(int));

        if (matriu[i] == NULL) {
            printf("MEMORY ERROR!");
            allibera_matriu(matriu, i);
            return NULL;
        }
    }
    return matriu;
}

void allibera_matriu(int** matriu, int m){
    for (int i = 0; i<m; i++) {
        free(matriu[i]); // Alliberem cada fila
    }
    free(matriu); // Alliberem la matriu gran
}