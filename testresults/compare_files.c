#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareIntegers(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s file1 file2 size\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file1 = fopen(argv[1], "r");
    FILE *file2 = fopen(argv[2], "r");
    int size = atoi(argv[3]);

    if (file1 == NULL || file2 == NULL) {
        perror("Error opening files");
        return EXIT_FAILURE;
    }

    int totalLines = 0;
    int commonNumbers = 0;
    int totalNumbers = 0;

    char line1[size*100]; 
    char line2[size*100]; 

    while (fgets(line1, sizeof(line1), file1) && fgets(line2, sizeof(line2), file2)) {
        totalLines++;

        // Remove trailing newline characters
        line1[strcspn(line1, "\r\n")] = '\0';
        line2[strcspn(line2, "\r\n")] = '\0';

        // Tokenize and sort the numbers in each line
        int numbers1[size]; 
        int numbers2[size]; 

        int num1, num2;

        int i = 0;
        char *token1 = strtok(line1, " ");
        while (token1 != NULL) {
            sscanf(token1, "%d", &num1);
            numbers1[i++] = num1;
            token1 = strtok(NULL, " ");
        }
        qsort(numbers1, i, sizeof(int), compareIntegers);

        int j = 0;
        char *token2 = strtok(line2, " ");
        while (token2 != NULL) {
            sscanf(token2, "%d", &num2);
            numbers2[j++] = num2;
            token2 = strtok(NULL, " ");
        }
        qsort(numbers2, j, sizeof(int), compareIntegers);

        // Compare the sorted lists
        int index1 = 0, index2 = 0;
        while (index1 < i && index2 < j) {
            if (numbers1[index1] == numbers2[index2]) {
                commonNumbers++;
                index1++;
                index2++;
            } else if (numbers1[index1] < numbers2[index2]) {
                index1++;
            } else {
                index2++;
            }
        }

        totalNumbers += i; // Assuming both lines have the same number of elements
    }

    fclose(file1);
    fclose(file2);

    if (totalNumbers > 0) {
        double similarityPercentage = (double)commonNumbers / totalNumbers * 100.0;
        printf("Similarity Percentage: %.2f%%\n", similarityPercentage);
    } else {
        printf("No numbers to compare.\n");
    }

    return EXIT_SUCCESS;
}
