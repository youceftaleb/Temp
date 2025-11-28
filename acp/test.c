#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    long num_steps = 10000000;
    double step = 1.0 / (double) num_steps;
    double pi, sum = 0.0;

    double start = omp_get_wtime();

    #pragma omp parallel num_threads(8)
    {
        double local = 0.0;

        #pragma omp for
        for (long i = 0; i < num_steps; i++) {
            double x = (i + 0.5) * step;
            local += 4.0 / (1.0 + x * x);
        }

        // ONE critical per thread (much faster)
        #pragma omp critical
        {
            sum += local;
        }
    }

    pi = step * sum;

    double end = omp_get_wtime();

    printf("Pi = %f\n", pi);
    printf("Time = %f seconds\n", end - start);

    return 0;
}
