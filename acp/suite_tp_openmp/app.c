#include <stdio.h>
#include <string.h>
#define NUM_SEQUENCES 4
#define SEQ_LENGTH 12
// Simule des séquences
char sequences[NUM_SEQUENCES][SEQ_LENGTH] = {
"ATGCGTAGCTAG",
"CGTACGTAGCTA",
"GCTAGCTAGCAT",
"ATCGATCGTAGC"
};
int main() {
int gc_count[NUM_SEQUENCES] = {0};
int sequences_high_gc = 0;
for (int i = 0; i < NUM_SEQUENCES; i++) {
int gc = 0;
for (int j = 0; j < SEQ_LENGTH; j++) {
if (sequences[i][j] == 'G' || sequences[i][j] == 'C') {
gc++;
}}
gc_count[i] = gc;
// Mettre à jour le compteur global
if ((gc * 100 / SEQ_LENGTH) > 50) {
sequences_high_gc++;
}
}
// Recherche du motif "GCTA"
printf("Recherche du motif 'GCTA' :\n");
for (int i = 0; i < NUM_SEQUENCES; i++) {
if (strstr(sequences[i], "GCTA") != NULL) {
printf("Motif 'GCTA' trouvé dans la séquence %d : %s\n", i, sequences[i]);
}
}
// Recherche du motif "ATCG"
printf("Recherche du motif 'ATCG' :\n");
for (int i = 0; i < NUM_SEQUENCES; i++) {
if (strstr(sequences[i], "ATCG") != NULL) {
printf("Motif 'ATCG' trouvé dans la séquence %d : %s\n", i, sequences[i]);
}
}
// Affichage des résultats
printf("\nNombre de séquences avec un GC > 50%% : %d\n", sequences_high_gc);
return 0;
}