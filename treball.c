/* VICTOR RUIZ AGUILERA 1707258
Resultats obtinguts en el peu PC:
- aleatori1, aleatori2 : no he trobat cota superior en un temps raonable
- aleatori3: n<=8 - instantani; n=13 - 423s
- backtrack: n<=12 - instantani; n=16 - 639s
- matriu: n<=12 - instantani; n=16 - 669s
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

// Gestió de memòria
int** assigna_matriu(int m, int n);
void allibera_matriu(int** matriu, int m);
int canvia_matriu(int** matriu, int n, long size, long newsize);

// Llegir / escriure
void print_vector(int n, int vector[]);
void print_matrix(int m, int n, int** matrix);
void read_vector(int n, int* v, char* str);
void escriu_vector(FILE* fitxer, int n, int vector[n]);

// Tractar vectors
int compare_vectors(int n, int v1[], int v2[]);
void generar_permutacio(int n, int vector[n]);
int es_solucio(int n, int vector[n]);
int generar_simetries(int n, int** vector);
int classifica_orbita(int n, int* v);

void vector_aleatori(int n);
void totes_les_solucions_aleatories(int n, char* sortida);
void backtracking(int n, int* tauler, int columna, int* k, FILE* fSortida);

void backtracking_matriu(int n, int* tauler, int columna, int* k, int* sol_size, int** solucions);
void matriu_compta_solucions (int n);

// ARGV: 0: .\vuit_reines.exe; 1: mida; 2: algorisme; 3: fitxer_entrada.txt; 4: fitxer_sortida.txt
int main(int argc, char* argv[]) {
    srand((unsigned) time(NULL));

    int n = atoi(argv[1]);
    if (n < 0) {
        printf("Dimensio %s no suportada, siusplau introdueix numeros naturals superiors o iguals a 1\n", argv[1]);
        return 0;
    } else if (n == 2 || n == 3) {
        printf("Error: no existeixen solucions per al cas %sx%s\n", argv[1], argv[1]);
        return 0;
    }

    printf("n = %s (%s)\n", argv[1], argv[2]); // linea inicial

    if (strcmp("aleatori1", argv[2]) == 0)
    {
        FILE* fEntrada = fopen(argv[3], "r");
        if (fEntrada == NULL){
            printf("No s'ha trobat el fitxer d'entrada!");
            return 0;
        }

        int v[n];
        char str[n*n];
        fgets(str, n*n*sizeof(char), fEntrada);
        read_vector(n, v, str); 
        printf("El vector proposat ");
        print_vector(n, v);
        printf("%s es solucio\n", es_solucio(n, v)? "SI":"NO");
        fclose(fEntrada);
    } else if (strcmp("aleatori2", argv[2]) == 0)
    {
        vector_aleatori(n);
    } else if (strcmp("aleatori3", argv[2]) == 0)
    {
        totes_les_solucions_aleatories(n, argv[4]);
        
    } else if (strcmp("backtrack", argv[2]) == 0)
    {
        FILE* fSortida = fopen(argv[4], "w");


        // unsigned long t0 = (unsigned long) time(NULL); 

        int k = 0, sol_size = (int) pow(10, n/2); // un bon estimat pels rangs que calcularem
        int tauler[n];
        int** solucions = assigna_matriu( sol_size, n);
        if (n == 1) {
            tauler[0] = 1;
            printf("1 solucio\n");
            escriu_vector(fSortida, 1, tauler);
            fclose(fSortida);
            return 1;
        }

        backtracking_matriu(n, tauler, 0, &k, &sol_size, solucions);

        canvia_matriu(solucions, n, sol_size, k*sizeof(int));

        for (int i = 0; i<k; i++) {
            escriu_vector(fSortida, n, solucions[i]);
        }

        // printf("temps de calcul: %lds\n", (long) difftime(time(NULL), t0));
        printf("%d solucions\n", k);

        allibera_matriu(solucions, k);
        fclose(fSortida);
    } else if (strcmp("matriu", argv[2]) == 0)
    {        
        // unsigned long t0 = (unsigned long) time(NULL);
        matriu_compta_solucions(n);
        
        // printf("temps de calcul: %lds\n", (long) difftime(time(NULL), t0));
    } else {
        printf("ERROR: Comanda %s no reconeguda", argv[2]);
    }
}

void matriu_compta_solucions (int n) {
    if (n == 1) {
        
        printf("MS = M1 = (1)\n");
        printf("Tenim una unica solucio v = (1)\n");
        return;
    }

    int k = 0, sol_size = (int) pow(10, n/2); // un bon estimat pels rangs que calcularem
    int tauler[n];
    int** solucions = assigna_matriu(sol_size, n);
    backtracking_matriu(n, tauler, 0, &k, &sol_size, solucions);

    canvia_matriu(solucions, n, sol_size, k*sizeof(int));

    int** MS = assigna_matriu(n, n); int** M2 = assigna_matriu(n, n); int** M4 = assigna_matriu(n, n); int** M8 = assigna_matriu(n, n);
    int** matrius_sim[] = {MS, M2, M4, M8};
    int solucions_essencials[] = {0, 0, 0};
    
    printf("Classificant \n");
    for (int i = 0; i<k; i++) {
        int classificacio = classifica_orbita(n, solucions[i]);
        
        for (int j = 0; j<n; j++) {
            matrius_sim[0][j][solucions[i][j]-1] += 1; 
            matrius_sim[classificacio][j][solucions[i][j]-1] += 1; 
        }

        solucions_essencials[classificacio-1] += 1;
    }
    printf("MS = \n");
    print_matrix(n, n, matrius_sim[0]);
    printf("------------------------------\n");
    printf("M2 = \n");
    print_matrix(n, n, matrius_sim[1]);
    printf("------------------------------\n");
    printf("M4 = \n");
    print_matrix(n, n, matrius_sim[2]);
    printf("------------------------------\n");
    printf("M8 = \n");
    print_matrix(n, n, matrius_sim[3]);
    printf("------------------------------\n");

    solucions_essencials[0] /= 2;
    solucions_essencials[1] /= 4;
    solucions_essencials[2] /= 8;

    printf("Vector de solucions essencials: ");
    print_vector(3, solucions_essencials);

    printf("Nombre de solucions essencials= %d\n", solucions_essencials[0] + solucions_essencials[1] + solucions_essencials[2]);
    printf("Nombre de solucions totals= %d\n", solucions_essencials[0]*2 + solucions_essencials[1]*4 + solucions_essencials[2]*8);
    allibera_matriu(MS, n);
    allibera_matriu(M2, n);
    allibera_matriu(M4, n);
    allibera_matriu(M8, n);
    allibera_matriu(solucions, k);
}

int classifica_orbita(int n, int* v) {
    int r[n], r2[n];
    for(int i = 0; i<n; i++) {
        r[v[i]-1] = n - i;
        r2[i] = n +1 - v[n-i-1];
    }

    if (compare_vectors(n, r, v)) 
        return 1;
    if (compare_vectors(n, r2, v)) 
        return 2;
    return 3;
}

void backtracking_matriu(int n, int* tauler, int columna, int* k, int* sol_size, int** solucions) {
    for (tauler[columna] = 1; tauler[columna]<=n && !(tauler[0] > ceil((float) n/2)); tauler[columna]++) {        
        int acceptable = 1;
        for (int j = 0; j<columna; j++){
            if ((tauler[j] == tauler[columna]) || (abs(tauler[columna] - tauler[j]) == abs(columna - j))) {
                acceptable = 0;
                break;
            }
        }
        if (!acceptable)
            continue;

        if (columna < n -1) {
            backtracking_matriu(n, tauler, columna +1, k, sol_size, solucions);
            continue;
        }

        if (*sol_size - *k <=2) { // memory management
            if (!canvia_matriu(solucions, n, *sol_size, *sol_size + n*n)) {
                *sol_size += n*n*sizeof(int);
                printf("Memoria insuficient pel cas n = %d", n);
                return;
            }
        }

        if (tauler[0] < (float) (n+1)/2) { // si s'ha d'afegir simetria vertical 
            for(int j = 0; j<n;j++) {
                solucions[*k][j] = tauler[j];
                solucions[*k+1][j] = n - tauler[j] + 1;
            }
            *k += 2;
            continue;
        }

        for(int j = 0; j<n;j++) { // si no
            solucions[*k][j] = tauler[j];
        }
        *k += 1;
    }
    return;
}

void backtracking(int n, int* tauler, int columna, int* k, FILE* fSortida) {
    for (tauler[columna] = 1; tauler[columna]<=n && !(tauler[0] > ceil((float) n/2)); tauler[columna]++) {        
        int acceptable = 1;
        for (int j = 0; j<columna; j++){
            if ((tauler[j] == tauler[columna]) || (abs(tauler[columna] - tauler[j]) == abs(columna - j))) {
                acceptable = 0;
                break;
            }
        }
        if (!acceptable)
            continue;

        if (columna < n -1) {
            backtracking(n, tauler, columna +1, k, fSortida);
            continue;
        }

        escriu_vector(fSortida, n, tauler);
        *k += 1;

        if (tauler[0] < (float) (n+1)/2) { // afegim simetria vertical
            int v[n];
            for (int j = 0; j<n; j++) {
                v[j] = n - tauler[j] + 1;
            }
            escriu_vector(fSortida, n, v);
            *k += 1;
        }
    }
    return;
}

void totes_les_solucions_aleatories(int n, char* sortida) {
    int numero_solucions[] = {1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712, 365596, 2279184, 14772512}; // alguns totals de solucions per saber quan parar
    // printf("Amb %d caselles tenim %d solucions\n", n, numero_solucions[n-1]);
    
    int** solucions = assigna_matriu(numero_solucions[n-1], n);
    if (solucions == NULL) {
        printf("MEMORY ERROR: espai insuficient per emmagatzemar totes les solucions!");
        return;
    }

    FILE* fSortida = fopen(sortida, "w");
    
    for(long i = 0; i<numero_solucions[n-1];){
        generar_permutacio(n, solucions[i]);

        if (es_solucio(n, solucions[i])){
            int solucio_nova = 1;
            for (int k = 0; k<i; k++) {
                if (compare_vectors(n, solucions[i], solucions[k])){
                    solucio_nova = 0;
                    break;
                }
            }
            if (!solucio_nova)  {
                continue;
            }

            escriu_vector(fSortida, n, solucions[i]);
            print_vector(n, solucions[i]);
            i++;
        }
    }
    allibera_matriu(solucions, numero_solucions[n-1]);
    fclose(fSortida);
}

void escriu_vector(FILE* fitxer, int n, int vector[n]) {
    fprintf(fitxer, "%d", vector[0]);
    for(int i = 1; i<n; i++) {
        fprintf(fitxer, " %d", vector[i]);
    }
    fprintf(fitxer, "\n");
}

void vector_aleatori(int n) {
    int* v = calloc(n, sizeof(int));
    if (!v) {
        printf("MEMORY ERROR: no s'ha pogut crear un vector");
    }
    generar_permutacio(n, v);

    // // Per comprobar fins que trobi un
    // for(int i =0; i<n*n && !es_solucio(n, vector); i++){
    //     generar_permutacio(n, vector);
    // }

    printf("El vector generat ");
    print_vector(n, v);
    printf("%s es solucio\n", es_solucio(n, v)? "SI":"NO");

    free(v);
}

void generar_permutacio(int n, int vector[n]){
    vector[0] = 1;
    for (int i = 1; i<n; i++) {
        int j = rand() % (i+1); // barreja nomes posicions anteriors (ja asignades)
        vector[i] = vector[j];
        vector[j] = i+1;
    }
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

int compare_vectors(int n, int v1[], int v2[]){
    for (int i = 0; i<n; i++){
        if (v1[i] != v2[i]){
            return 0;
        }
    }
    return 1;
}

void read_vector(int n, int* v, char* str) {
    for(int i = 0, j = 0, nums = 0; nums<n;) {
        char* num = calloc((int) ceil(log10(n)), sizeof(char));
        for(j = 0; str[i+j] != ' ' && str[i+j] != '\0' && str[i+j] != EOF;j++) {
            num[j] = str[i+j];
        }
        i+=j+1;
        
        v[nums++] = atoi(num);
        free(num);
    }
}

void print_vector(int n, int vector[]) {
    printf("v = (%d", vector[0]);
    for (int i = 1; i<n; i++){
        printf(", %d", vector[i]);
    }
    printf(");\n");
}

void print_matrix(int m, int n, int** matrix) {
    for (int i = 0; i<m; i++) {
        printf("(%d", matrix[i][0]);
        for (int j = 1; j<n; j++){
            printf(", %d", matrix[i][j]);
        }
        printf(")\n");
    }
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

int canvia_matriu(int** matriu, int n, long size, long newsize) {
    int** newPtr = realloc(matriu, newsize*sizeof(int*));
    if (newPtr == NULL) {
        printf("MEMORY ERROR: memoria insuficient per realocar-la");
        return 0;
    }
    matriu = newPtr;
    for (; size < newsize; size++)
    {
        matriu[size] = (int*) calloc(n, sizeof(int));

        if (matriu[size] == NULL) {
            printf("MEMORY ERROR!");
            allibera_matriu(matriu, size);
            return 0;
        }
    }
    return 1;
}