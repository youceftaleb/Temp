#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_SEQUENCES 100
#define MAX_LENGTH 20000

int main() {
    const char* filename = "seqs-RNA.fasta.txt";
    char** sequences = malloc(MAX_SEQUENCES * sizeof(char*));
    int num_sequences = 0;

    FILE* file = fopen(filename, "r");
    if (!file) { perror("open"); return 1; }

    char buffer[MAX_LENGTH + 1];
    while (fgets(buffer, sizeof(buffer), file) && num_sequences < MAX_SEQUENCES) {
        buffer[strcspn(buffer, "\n")] = '\0';
        sequences[num_sequences] = strdup(buffer);
        if (!sequences[num_sequences]) { perror("malloc"); return 1; }
        num_sequences++;
    }
    fclose(file);

    int *gc_count = malloc(num_sequences * sizeof(int));
    int sequences_high_gc = 0;

    double t0 = omp_get_wtime();

    #pragma omp parallel for num_threads(4) shared(sequences, gc_count, num_sequences, sequences_high_gc) 
    for (int i = 0; i < num_sequences; ++i) {
        int gc = 0;
        int len = strlen(sequences[i]);
        for (int j = 0; j < len; ++j) {
            char c = sequences[i][j];
            if (c == 'G' || c == 'C' || c == 'c' || c == 'g') gc++;
        }
        gc_count[i] = gc;

        if ((gc * 100 / (len ? len : 1)) > 50) {
            #pragma omp critical
            {
                sequences_high_gc++;
            }
        }
    }

    double t1 = omp_get_wtime();

    printf("Motif 'GCTA' :\n");
    #pragma omp parallel for default(none) shared(sequences, num_sequences)
    for (int i = 0; i < num_sequences; ++i) {
        if ((strstr(sequences[i], "GCTA") != NULL) || (strstr(sequences[i], "gcta") != NULL)) {
            #pragma omp critical
            printf("Motif 'GCTA' found in sequence %d : %s\n", i, sequences[i]);
        }
    }

    printf("Motif 'ATCG' :\n");
    #pragma omp parallel for default(none) shared(sequences, num_sequences)
    for (int i = 0; i < num_sequences; ++i) {
        if ((strstr(sequences[i], "ATCG") != NULL) || (strstr(sequences[i], "atcg") != NULL)) {
            #pragma omp critical
            printf("Motif 'ATCG' found in sequence %d : %s\n", i, sequences[i]);
        }
    }

    printf("\nNumber of sequences with GC > 50%% : %d\n", sequences_high_gc);
    printf("Elapsed time (parallel critical) : %f seconds\n", t1 - t0);

    for (int i = 0; i < num_sequences; ++i) free(sequences[i]);
    free(sequences);
    free(gc_count);
    return 0;
}
