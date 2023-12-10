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
    float **result;
    int n; // Matrix dimension
    int start_row;
    int end_row;
} section_data;

/*
 * Auxiliary function to process a section of the matrix multiplication.
 */
void *process_section(void *arg) {
    // Todo: Try struct *PtrRango -> void *process_section(PtrRango rango){...}
    section_data *data = (section_data *) arg;
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
* Standard Matrix multiplication with O(n^3) time complexity.
*/
float **standardMultiplication(float **matrixA, float **matrixB, int n) {
    return standardMultiplication_ijk(matrixA, matrixB, n);
    //return standardMultiplication_ikj(matrixA,matrixB,n);
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
        // Todo: Make get end row function
        if (i == THREADS - 1) {
            section_data[i].end_row = n;
        } else {
            section_data[i].end_row = (i + 1) * rows_per_section;
        }
        pthread_create(&threads[i], NULL, process_section, &section_data[i]);
    }

    // Todo: Make join function
    for (i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed_std = (finish.tv_sec - start.tv_sec);
    elapsed_std += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    return result;
}

/*
* Standard ikj Matrix multiplication with O(n^3) time complexity.
*/
float **standardMultiplication_ikj(float **matrixA, float **matrixB, int n) {
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
