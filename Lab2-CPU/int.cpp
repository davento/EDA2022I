#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

float fun(float a){
    return a;
}
    
float reglaTrapecio(float a, float b, int n){
/*
     -------------------------------
    | Core1 | Core2 | Core3 | Core4 |
*/
    float localA;
    float localB;
    int localN;
    float h;
    float resultadoLocal;
    float x;
    int i;

    
    int idHilo = omp_get_thread_num();
    int cantidadHilos = omp_get_num_threads();

    h = (b - a) / n;

    localN = n / cantidadHilos;
    localA = a + idHilo* localN * h;
    localB = localA + h * localN;

    resultadoLocal = (fun(localA) + fun(localB) / 2.0);

    for(i = 1; i < localN-1; i++) {
        x = localA + i * h;
        resultadoLocal += fun(x);
    }
    return resultadoLocal * h;
}

/*
         ----------------
   fork /                \ join
--------------------------------------
        \                / 
         ----------------
*/


int main() {
    float a;
    float b;
    int n,i;
    float h;
    int cantidadHilos = 4;

    float resultado = 0.0;

    a = 1;
    b = 5;
    n = 10000000;
    h = (b - a) / n;

    resultado = (fun(a) + fun(b) / 2.0);

#   pragma omp parallel for num_threads(cantidadHilos) reduction(+:resultado)
    for (i = 1;i<n-1;++i){
        resultado += fun(a + i*h);
    }
    resultado = resultado*h;
    
    printf("La integral de 1/x es igual a %f\n", resultado);
    return 0;
}