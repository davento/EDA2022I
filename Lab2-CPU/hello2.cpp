#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <cmath>
using namespace std;

void hello() {
	size_t idThread = omp_get_thread_num();
	printf("Hi from thread %ld\n", idThread);
}

int main(int argc, char const *argv[]) {
	size_t count = 4;
#	pragma omp parallel num_threads(count);
	hello();
	return 0;
}
