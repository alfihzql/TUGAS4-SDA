#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <psapi.h>
#include "sorts_combined.h"

// Fungsi untuk membangkitkan kata acak
void random_word(char *word, int length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < length; i++) {
        int key = rand() % (int)(sizeof(charset) - 1);
        word[i] = charset[key];
    }
    word[length] = '\0';
}

// Fungsi untuk menghasilkan kata acak
void generate_random_words(char **data, int count, int max_word_length) {
    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        int length = (rand() % (max_word_length - 3)) + 3; // panjang kata minimal 3
        data[i] = malloc((length + 1) * sizeof(char));
        random_word(data[i], length);
    }
}

// Konversi array string menjadi array panjang string untuk pemetaan indeks
int *map_words_to_keys(char **data, int n) {
    int *keys = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (char *p = data[i]; *p; p++) sum += *p;
        keys[i] = sum;
    }
    return keys;
}

// Reorganisasi array string berdasarkan urutan kunci
void reorder_words(char **data, int *keys, int *sorted_keys, int n) {
    char **temp = malloc(n * sizeof(char*));
    int *used = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (used[j]) continue;
            int sum = 0;
            for (char *p = data[j]; *p; p++) sum += *p;
            if (sum == sorted_keys[i]) {
                temp[i] = data[j];
                used[j] = 1;
                break;
            }
        }
    }
    memcpy(data, temp, n * sizeof(char*));
    free(temp);
    free(used);
}

// Fungsi wrapper untuk mergeSort yang disesuaikan
void mergeSortWrapper(int *arr, int n) {
    mergeSort(arr, 0, n - 1);
}

// Fungsi wrapper untuk quickSort yang disesuaikan
void quickSortWrapper(int *arr, int n) {
    quickSort(arr, 0, n - 1);
}

// Ukur performa algoritma sorting
void measureAndSort(const char *name, void (*sortFunc)(int *, int), char **data, int n) {
    char **temp_data = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        temp_data[i] = strdup(data[i]);
    }

    int *keys = map_words_to_keys(temp_data, n);
    int *sorted_keys = malloc(n * sizeof(int));
    memcpy(sorted_keys, keys, n * sizeof(int));

    clock_t start = clock();
    sortFunc(sorted_keys, n);
    clock_t end = clock();

    reorder_words(temp_data, keys, sorted_keys, n);

    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    SIZE_T memUsed = pmc.PeakWorkingSetSize;
    double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("==============================================\n");
    printf("Algoritma   : %s\n", name);
    printf("Waktu       : %.3f detik\n", timeTaken);
    printf("Memori      : %.2f MB\n", memUsed / (1024.0 * 1024.0));
    printf("==============================================\n\n");

    for (int i = 0; i < n; i++) free(temp_data[i]);
    free(temp_data);
    free(keys);
    free(sorted_keys);
}

int main() {
    int options[] = {10000, 50000, 100000, 250000, 500000, 1000000, 1500000, 2000000};
    int choice;

    while (1) {
        printf("=========================================\n");
        printf("Pilih jumlah data acak:\n");
        for (int i = 0; i < 8; i++) {
            printf("%d. %d\n", i + 1, options[i]);
        }
        printf("9. Keluar\n");
        printf("Pilihan Anda: ");
        if (scanf("%d", &choice) != 1) {
            printf("Input tidak valid! Silakan coba lagi.\n");
            while(getchar() != '\n');
            continue;
        }

        if (choice == 9) {
            printf("Terima kasih! Keluar dari program.\n");
            break;
        }

        if (choice < 1 || choice > 8) {
            printf("Pilihan tidak valid.\n\n");
            continue;
        }

        int n = options[choice - 1];
        char **data = malloc(n * sizeof(char*));
        generate_random_words(data, n, 20);
        printf("\nJumlah data: %d\n\n", n);

        measureAndSort("Bubble Sort", bubbleSort, data, n);
        measureAndSort("Selection Sort", selectionSort, data, n);
        measureAndSort("Insertion Sort", insertionSort, data, n);
        measureAndSort("Merge Sort", mergeSortWrapper, data, n);
        measureAndSort("Quick Sort", quickSortWrapper, data, n);
        measureAndSort("Shell Sort", shellSort, data, n);

        for (int i = 0; i < n; i++) {
            free(data[i]);
        }
        free(data);

        printf("Tekan ENTER untuk kembali ke menu...\n");
        getchar(); getchar();
    }

    return 0;
}