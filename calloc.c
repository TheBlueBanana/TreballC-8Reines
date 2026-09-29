#include<stdio.h>
#include<stdlib.h>

void basic(long n);
void matriu(int m, int n);

void extend_vector(int* v, int n);
void extend_matriu(int** matriu);

int main(int argn, char* argv[]){

    return 0;
}

void extend_vector(int* v, int n) { //????
    int* buffer = v;
    
    v = realloc(v , 10 + (n * sizeof( int )));
    if(buffer ==  NULL)
    {
        v = buffer;
        printf("No s'ha pogut extendre el vector\n");
        free(buffer);
        return;
    }
    printf("Todo en orden");

    free( buffer );
}

void matriu(int m, int n) {
    //Equivalent a int matriu[m][n]
    int** matriu = (int**) calloc(m, sizeof(int));

    for (int i = 0; i<m; i++) {
        matriu[i] = (int*) calloc(n, sizeof(int));
    }

    // codi

    for (int i = 0; i<m; i++) {
        free(matriu[i]); // Alliberem cada fila
    }
    free(matriu); // Alliberem la matriu gran
}

void basic(long n) {
    
    int* v = calloc(n, sizeof(int));
    if (v == NULL) {
        printf("infartito...\n");
        return;
    }
    printf("Sin problema, jefe!\n");
    // codigo
    free(v);
}
