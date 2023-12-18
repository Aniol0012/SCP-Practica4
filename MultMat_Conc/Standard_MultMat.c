/* ---------------------------------------------------------------
Práctica 4.
Código fuente: Standard_MultMat.c
Grau Informàtica
48056540H - Aleix Segura Paz.
21161168H - Aniol Serrano Ortega.
--------------------------------------------------------------- */

//
// Created by Fernando Cores Prado on 4/12/23.
//

#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include "Standard_MultMat.h"
#include "Matrix.h"
#include "Errors.h"
#include <pthread.h>

double elapsed_std;
extern int threads;

/*
 * Struct to store the data of a section of the matrix multiplication.
 * float** matrixA: First matrix.
 * float** matrixB: Second matrix.
 * float** result: Result matrix.
 * int n: Matrix dimension.
 * int start_row: Start row of the section.
 * int end_row: End row of the section.
*/
typedef struct {
    float **matrixA;
    float **matrixB;
    float **result;
    int n;
    int start_row;
    int end_row;
} section_data;

void *process_section(section_data *data);

int get_end_row(int index, int n, int rows_per_section);

void cancel_threads(pthread_t *threads_list, int threads_created);


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

    pthread_t threads_list[threads];
    section_data data[threads];

    int rows_per_section = n / threads;
    int i;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < threads; i++) {
        data[i].matrixA = matrixA;
        data[i].matrixB = matrixB;
        data[i].result = result;
        data[i].n = n;
        data[i].start_row = i * rows_per_section;
        data[i].end_row = get_end_row(i, n, rows_per_section);

        if (pthread_create(&threads_list[i], NULL, (void *) process_section, &data[i]) != 0) {
            cancel_threads(threads_list, i);
            Error("Error creating threads");
        }
    }

    for (i = 0; i < threads; i++) {
        if (pthread_join(threads_list[i], NULL)) {
            if (pthread_cancel(threads_list[i])) {
                Error("Error canceling threads (join)");
            }
            Error("Error joining threads");
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

/*
 * Auxiliary function to process a section of the matrix multiplication.
*/
void *process_section(section_data *data) {
    int i, j, k;
    for (i = data->start_row; i < data->end_row; i++) {
        for (j = 0; j < data->n; j++) {
            for (k = 0; k < data->n; k++) {
                data->result[i][j] += data->matrixA[i][k] * data->matrixB[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

/*
 * Auxiliary function to get the end row of a section.
*/
int get_end_row(int index, int n, int rows_per_section) {
    if (index == threads - 1) {
        return n;
    } else {
        return (index + 1) * rows_per_section;
    }
}

/*
 * Auxiliary function to cancel all threads already created.
*/
void cancel_threads(pthread_t *threads_list, int threads_created) {
    for (int i = 0; i < threads_created; i++) {
        if (pthread_cancel(threads_list[i])) {
            Error("Error canceling threads (creation)");
        }
    }
}