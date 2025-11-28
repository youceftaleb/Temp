#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    long num_steps = 10000000;
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;
    double pi;
    double start = omp_get_wtime();
    #pragma omp parallel for num_threads(8)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        double local = 4.0 / (1.0 + x*x);

        // Protect the shared variable 'sum'
        #pragma omp critical
        {
            sum += local;
        }
    }

    pi = step * sum;
    double end = omp_get_wtime();

    printf("calcule de Pi = %f\n", pi);
    printf("Time = %f seconds\n", end - start);
    return 0;
}
