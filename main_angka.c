#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <psapi.h>
#include <process.h> // untuk _beginthread
#include "sorts_combined.h"

#define NUM_SORTS 6

void copyArray(int *src, int *dest, int n) {
    for (int i = 0; i < n; i++) dest[i] = src[i];
}

typedef struct {
    const char *name;
    void (*sortFunc)(int*, int);
    int *data;
    int n;
} SortParams;

void __cdecl sortThread(void *args) {
    SortParams *params = (SortParams *)args;

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    params->sortFunc(params->data, params->n);

    QueryPerformanceCounter(&end);
    double timeTaken = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;

    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    SIZE_T memUsed = pmc.PeakWorkingSetSize;

    printf("==============================================\n");
    printf("Algoritma   : %s\n", params->name);
    printf("Waktu       : %.3f detik\n", timeTaken);
    printf("Memori      : %.2f MB\n", memUsed / (1024.0 * 1024.0));
    printf("==============================================\n\n");

    free(params->data);
    free(params);
    _endthread();
}

void mergeSortWrapper(int *arr, int n) {
    mergeSort(arr, 0, n - 1);
}

void quickSortWrapper(int *arr, int n) {
    quickSort(arr, 0, n - 1);
}

int main() {
    int options[] = {10000, 50000, 100000, 250000, 500000, 1000000, 1500000, 2000000};
    int choice;

    while (1) {
        printf("=========================================");
        printf("\nPilih jumlah data acak:\n");
        for (int i = 0; i < 8; i++) {
            printf("%d. %d\n", i + 1, options[i]);
        }
        printf("9. Keluar\n");
        printf("Pilihan Anda: ");
        scanf("%d", &choice);

        if (choice == 9) {
            printf("Terima kasih! Keluar dari program.\n");
            break;
        }

        if (choice < 1 || choice > 8) {
            printf("Pilihan tidak valid.\n\n");
            continue;
        }

        int n = options[choice - 1];
        int *data = malloc(n * sizeof(int));
        srand((unsigned)time(NULL));
        for (int i = 0; i < n; i++)
            data[i] = rand() % 1000000;

        printf("\nJumlah data: %d\n\n", n);

        const char *names[] = {
            "Bubble Sort", "Selection Sort", "Insertion Sort",
            "Merge Sort", "Quick Sort", "Shell Sort"
        };
        void (*functions[])(int*, int) = {
            bubbleSort, selectionSort, insertionSort,
            mergeSortWrapper, quickSortWrapper, shellSort
        };

        HANDLE handles[NUM_SORTS];

        for (int i = 0; i < NUM_SORTS; i++) {
            SortParams *params = malloc(sizeof(SortParams));
            params->name = names[i];
            params->sortFunc = functions[i];
            params->n = n;
            params->data = malloc(n * sizeof(int));
            copyArray(data, params->data, n);

            handles[i] = (HANDLE)_beginthread(sortThread, 0, params);
        }

        // Tunggu semua thread selesai
        WaitForMultipleObjects(NUM_SORTS, handles, TRUE, INFINITE);

        free(data);
        printf("Tekan ENTER untuk kembali ke menu...\n");
        getchar(); getchar();
    }

    return 0;
}
