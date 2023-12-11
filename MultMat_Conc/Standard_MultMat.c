//
// Created by Fernando Cores Prado on 4/12/23.
//

#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include "Standard_MultMat.h"
#include "Matrix.h"
#include <pthread.h>

double elapsed_std;
int THREADS = 10;

typedef struct {
    float **matrixA;
    float **matrixB;
    int n; // Matrix dimension
    int start_row;
    int end_row;
    float **result;
} section_data;

/*
 * Auxiliary function to process a section of the matrix multiplication.
 */
int *process_section(section_data *section_data_thread) {
    section_data *task_data = section_data_thread;
    int i, j, k;
    for (i = task_data->start_row; i < task_data->end_row; i++) {
        for (j = 0; j < task_data->n; j++) {
            for (k = 0; k < task_data->n; k++) {
                task_data->result[i][j] += task_data->matrixA[i][k] * task_data->matrixB[k][j];
            }
        }
    }
    pthread_exit(NULL);
    return (NULL);
}

int get_end_row(int index, int n, int rows_per_section) {
    if (index == THREADS - 1) {
        return n;
    } else {
        return (index + 1) * rows_per_section;
    }
}

/*
* Standard Matrix multiplication with O(n^3) time complexity.
*/
float **standardMultiplication(float **matrixA, float **matrixB, int n) {
    return standardMultiplication_ijk(matrixA, matrixB, n);
    // Sequential versions:
    // return standardMultiplication_ijk_sec(matrixA, matrixB, n);
    // return standardMultiplication_ikj_sec(matrixA,matrixB,n);
}


/*
* Standard ijk Matrix multiplication with O(n^3) time complexity.
*/
float **standardMultiplication_ijk(float **matrixA, float **matrixB, int n) {
    struct timespec start, finish;
    float **result = createZeroMatrix(n);
    pthread_t threads[THREADS];
    section_data section_data[THREADS];

    int rows_per_section = n / THREADS;
    int i;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < THREADS; i++) {
        section_data[i].matrixA = matrixA;
        section_data[i].matrixB = matrixB;
        section_data[i].result = result;
        section_data[i].n = n;
        section_data[i].start_row = i * rows_per_section;
        section_data[i].end_row = get_end_row(i, n, rows_per_section);

        if (pthread_create(&threads[i], NULL, (void *(*)(void *)) process_section, &section_data[i]) != 0) {
            perror("Error creating thread number ");
            exit(1);
        }
    }

    for (i = 0; i < THREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            perror("Error joining threads");
            exit(2);
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed_std = (finish.tv_sec - start.tv_sec);
    elapsed_std += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    return result;
}


/*
* Standard ijk Matrix multiplication with O(n^3) time complexity. (sequential)
*/
float **standardMultiplication_ijk_sec(float **matrixA, float **matrixB, int n) {
    struct timespec start, finish;
    float **result;
    int i, j, k;

    clock_gettime(CLOCK_MONOTONIC, &start);

    result = (float **) malloc(n * sizeof(float *));
    for (i = 0; i < n; i++) {
        result[i] = (float *) malloc(n * sizeof(float));
        memset(result[i], 0, n * sizeof(float));
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) {
                result[i][j] = result[i][j] + (matrixA[i][k] * matrixB[k][j]);
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed_std = (finish.tv_sec - start.tv_sec);
    elapsed_std += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    return result;
}


/*
* Standard ikj Matrix multiplication with O(n^3) time complexity.
*/
float **standardMultiplication_ikj_sec(float **matrixA, float **matrixB, int n) {
    struct timespec start, finish;
    float **result;
    int i, j, k;

    clock_gettime(CLOCK_MONOTONIC, &start);

    result = (float **) malloc(n * sizeof(float *));
    for (i = 0; i < n; i++) {
        result[i] = (float *) malloc(n * sizeof(float));
        memset(result[i], 0, n * sizeof(float));
        for (k = 0; k < n; k++) {
            for (j = 0; j < n; j++) {
                result[i][j] = result[i][j] + (matrixA[i][k] * matrixB[k][j]);
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed_std = (finish.tv_sec - start.tv_sec);
    elapsed_std += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    return result;
}
