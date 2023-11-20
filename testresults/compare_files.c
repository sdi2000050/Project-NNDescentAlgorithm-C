#include <stdio.h>
#include <stdlib.h>

double compareFiles(FILE *file1, FILE *file2) {
    int ch1, ch2;
    int matchingCount = 0;
    int totalCharacters = 0;

    while (1) {
        ch1 = fgetc(file1);
        ch2 = fgetc(file2);

        if (ch1 == EOF || ch2 == EOF) {
            break; // Break if either file reaches the end
        }

        if (ch1 == ch2) {
            matchingCount++;
        }

        totalCharacters++;
    }

    if (totalCharacters == 0) {
        return 100.0; // Both files are empty, consider them 100% similar
    }

    double similarityPercentage = (double)matchingCount / totalCharacters * 100.0;
    return similarityPercentage;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file1 = fopen(argv[1], "rb");
    FILE *file2 = fopen(argv[2], "rb");

    if (file1 == NULL || file2 == NULL) {
        perror("Error opening files");
        return EXIT_FAILURE;
    }

    double similarityPercentage = compareFiles(file1, file2);

    printf("Similarity Percentage: %.2f%%\n", similarityPercentage);

    fclose(file1);
    fclose(file2);

    return EXIT_SUCCESS;
}
