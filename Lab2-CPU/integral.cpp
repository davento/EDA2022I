#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

float fun(float a) {
    return 1/a;
}

void reglaTrapecio(float a, float b, int n, float* resultado) {
    float localA, localB;
    int localN, i;
    float h, resultadoLocal, x;

    int idHilo = omp_get_thread_num();
    int cantidadHilos = omp_get_num_threads();

    h = (b-a)/n;

    localN= n/cantidadHilos;
    localA = a + idHilo * localN * h;
    localB = localA + h*localN;

    resultadoLocal = (fun(localA)+fun(localB))/2.0;

    for(i = 1; i < localN-1; i++) {
        x = localA+i*h;
        resultadoLocal += fun(x);
    }

    resultadoLocal = resultadoLocal*h;

#   pragma omp critical
    *resultado += resultadoLocal;
}

int main() {
    float a, b;
    int n, cantidadHilos;
    float resultado = 0.0;
    cantidadHilos = 4;
    a = 1; b = 5; n = 10000000;

#   pragma omp parallel num_threads(cantidadHilos)
    reglaTrapecio(a,b,n,&resultado);
    printf("La integral de 1/x es igual a %f\n", resultado);

    return 0;
}