/* ---------------------------------------------------------------
Práctica 4.
Código fuente: Strassens_MultMat.c
Grau Informàtica
48056540H - Aleix Segura Paz.
21161168H - Aniol Serrano Ortega.
--------------------------------------------------------------- */

//
// Created by Fernando Cores Prado on 4/12/23.
//

#include "Strassens_MultMat.h"
#include <time.h>
#include <stdlib.h>
#include "Matrix.h"
#include "Errors.h"
#include <pthread.h>

double elapsed_str;
int Dim2StopRecursivity = 10;
extern int threads;
int actual_threads = 0;

/*
 * Struct to store the data of a section of the matrix multiplication.
 * float** matrixA: First matrix.
 * float** matrixB: Second matrix.
 * float** result: Result matrix.
 * int n: Matrix dimension.
*/
typedef struct {
    float **matrixA;
    float **matrixB;
    float **result;
    int n;
    int mx;
    int start_index;
    int end_index;
} matrix_data;

void cancel_threads_strassens(pthread_t *threads_list, int index);

void free_matrix(float **matrix, int n);

void *calculate_mx(matrix_data *data);

void *calculate_m1(matrix_data *data);

void *calculate_m2(matrix_data *data);

void *calculate_m3(matrix_data *data);

void *calculate_m4(matrix_data *task_data);

void *calculate_m5(matrix_data *task_data);

void *calculate_m6(matrix_data *task_data);

void *calculate_m7(matrix_data *task_data);

/*
 * Wrapper function over strassensMultRec.
*/
float **strassensMultiplication(float **matrixA, float **matrixB, int n) {
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    if (n > 32)
        Dim2StopRecursivity = n / 16;

    // Max number of concurrency (tasks) is 7
    if (threads > 7) {
        actual_threads = 7;
    } else {
        actual_threads = threads;
    }

    float **result = strassensMultRec(matrixA, matrixB, n);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed_str = (finish.tv_sec - start.tv_sec);
    elapsed_str += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    return result;
}

/* Todo:
 * - Pasar los threads como parametro
 * - Crear una funcion para calcular los mX
 * - Crear los hilos unicamente una vez
 * - En cas que fallin els fils al crearse o el join, cancelar els fils
*/

/*
 * Strassen's Multiplication algorithm using Divide and Conquer technique.
*/
float **strassensMultRec(float **matrixA, float **matrixB, int n) {
    float **result = createZeroMatrix(n);

    if (n > Dim2StopRecursivity) {
        pthread_t threads_list[actual_threads];
        matrix_data data[7];

        int task_per_thread = 7 / actual_threads;
        int extra_tasks = 7 % actual_threads;

        int start_index = 0;
        for (int i = 0; i < 7; i++) {
            data[i].matrixA = matrixA;
            data[i].matrixB = matrixB;
            data[i].result = NULL;
            data[i].n = n;
            data[i].mx = i + 1;
            data[i].start_index = start_index;
            data[i].end_index = start_index + task_per_thread + (i < extra_tasks ? 1 : 0) - 1;
            start_index = data[i].end_index + 1;
        }

        // Recursive call for Divide and Conquer
        for (int i = 0; i < actual_threads; i++) {
            if (pthread_create(&threads_list[i], NULL, (void *) calculate_mx, &data[i])) {
                cancel_threads_strassens(threads_list, i);
                Error("Error creating threads");
            }
        }

        for (int i = 0; i < actual_threads; i++) {
            if (pthread_join(threads_list[i], NULL)) {
                if (pthread_cancel(threads_list[i])) {
                    Error("Error canceling threads (join)");
                }
                Error("Error joining threads");
            }
        }

        float **m1 = data[0].result;
        float **m2 = data[1].result;
        float **m3 = data[2].result;
        float **m4 = data[3].result;
        float **m5 = data[4].result;
        float **m6 = data[5].result;
        float **m7 = data[6].result;

        float **temp_A, **temp_B;

        temp_A = addMatrix(m1, m4, n / 2);
        temp_B = subMatrix(temp_A, m5, n / 2);

        float **c11 = addMatrix(temp_B, m7, n / 2);

        free_matrix(temp_A, n / 2);
        free_matrix(temp_B, n / 2);

        float **c12 = addMatrix(m3, m5, n / 2);
        float **c21 = addMatrix(m2, m4, n / 2);

        temp_A = subMatrix(m1, m2, n / 2);
        temp_B = addMatrix(temp_A, m3, n / 2);

        float **c22 = addMatrix(temp_B, m6, n / 2);

        free_matrix(temp_A, n / 2);
        free_matrix(temp_B, n / 2);

        // Compose the matrix
        compose(c11, result, 0, 0, n / 2);
        compose(c12, result, 0, n / 2, n / 2);
        compose(c21, result, n / 2, 0, n / 2);
        compose(c22, result, n / 2, n / 2, n / 2);

        free_matrix(c11, n / 2);
        free_matrix(c12, n / 2);
        free_matrix(c21, n / 2);
        free_matrix(c22, n / 2);

        for (int i = 0; i < 7; i++) {
            free(data[i].result);
        }
    } else {
//This is the terminating condition for recursion.
//result[0][0]=matrixA[0][0]*matrixB[0][0];
        result = standardMultiplication(matrixA, matrixB, n);
    }
    return
            result;
}

/*
 * This method combines the matrix in the result matrix
*/
void compose(float **matrix, float **result, int row, int col, int n) {
    int i, j, r = row, c = col;
    for (i = 0; i < n; i++) {
        c = col;
        for (j = 0; j < n; j++) {
            result[r][c] = matrix[i][j];
            c++;
        }
        r++;
    }
}

/*
 * Sub-divide the matrix according to row and col specified
*/
float **divide(float **matrix, int n, int row, int col) {
    int n_new = n / 2;

    float **array = createZeroMatrix(n_new);
    int i, j, r = row, c = col;
    for (i = 0; i < n_new; i++) {
        c = col;
        for (j = 0; j < n_new; j++) {
            array[i][j] = matrix[r][c];
            c++;
        }
        r++;
    }
    return array;
}

/*
 * Add the two input matrix
*/
float **addMatrix(float **matrixA, float **matrixB, int n) {
    float **res = createZeroMatrix(n);
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            res[i][j] = matrixA[i][j] + matrixB[i][j];

    return res;
}

/*
 * Substract the two matrix
*/
float **subMatrix(float **matrixA, float **matrixB, int n) {
    float **res = createZeroMatrix(n);
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            res[i][j] = matrixA[i][j] - matrixB[i][j];

    return res;
}

/*
 * Auxiliary function to cancel all threads.
*/
void cancel_threads_strassens(pthread_t *threads_list, int index) {
    for (int i = 0; i < index; i++) {
        if (pthread_cancel(threads_list[i])) {
            Error("Error canceling threads (creation)");
        }
    }
}

/*
 * Free data from a matrix if not null
*/
void free_matrix(float **matrix, int n) {
    if (matrix != NULL) {
        for (int i = 0; i < n; i++) {
            if (matrix[i] != NULL) {
                free(matrix[i]);
            }
        }
        free(matrix);
    }
}

void *calculate_mx(matrix_data *data) {
    for (int i = data->start_index; i <= data->end_index; i++) {
        matrix_data *task_data = data;

        switch (i +1) {
            case 1:
                print("m1\n");
                calculate_m1(task_data);
                break;
            case 2:
                print("m2\n");
                calculate_m2(task_data);
                break;
            case 3:
                print("m3\n");
                calculate_m3(task_data);
                break;
            case 4:
                print("m4\n");
                calculate_m4(task_data);
                break;
            case 5:
                print("m5\n");
                calculate_m5(task_data);
                break;
            case 6:
                print("m6\n");
                calculate_m6(task_data);
                break;
            case 7:
                print("m7\n");
                calculate_m7(task_data);
                break;
            default:
                Error("Error creating threads");
        }
    }
    pthread_exit(NULL);
}

void *calculate_m1(matrix_data *data) {
    matrix_data *task_data = data;

    float **a11 = divide(task_data->matrixA, task_data->n, 0, 0);
    float **a22 = divide(task_data->matrixA, task_data->n, task_data->n / 2, task_data->n / 2);
    float **b11 = divide(task_data->matrixB, task_data->n, 0, 0);
    float **b22 = divide(task_data->matrixB, task_data->n, task_data->n / 2, task_data->n / 2);

    float **add_A = addMatrix(a11, a22, task_data->n / 2);
    float **add_B = addMatrix(b11, b22, task_data->n / 2);

    task_data->result = strassensMultRec(add_A, add_B, task_data->n / 2);

    free_matrix(a11, task_data->n / 2);
    free_matrix(a22, task_data->n / 2);
    free_matrix(b11, task_data->n / 2);
    free_matrix(b22, task_data->n / 2);
    free_matrix(add_A, task_data->n / 2);
    free_matrix(add_B, task_data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m2(matrix_data *data) {
    matrix_data *task_data = data;

    float **a21 = divide(task_data->matrixA, task_data->n, task_data->n / 2, 0);
    float **a22 = divide(task_data->matrixA, task_data->n, task_data->n / 2, task_data->n / 2);
    float **b11 = divide(task_data->matrixB, task_data->n, 0, 0);

    float **add_A = addMatrix(a21, a22, task_data->n / 2);

    task_data->result = strassensMultRec(add_A, b11, task_data->n / 2);

    free_matrix(a21, task_data->n / 2);
    free_matrix(a22, task_data->n / 2);
    free_matrix(b11, task_data->n / 2);
    free_matrix(add_A, task_data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m3(matrix_data *data) {
    matrix_data *task_data = data;

    float **a11 = divide(task_data->matrixA, task_data->n, 0, 0);
    float **b12 = divide(task_data->matrixB, task_data->n, 0, task_data->n / 2);
    float **b22 = divide(task_data->matrixB, task_data->n, task_data->n / 2, task_data->n / 2);

    float **sub_B = subMatrix(b12, b22, task_data->n / 2);

    task_data->result = strassensMultRec(a11, sub_B, task_data->n / 2);

    free_matrix(a11, task_data->n / 2);
    free_matrix(b12, task_data->n / 2);
    free_matrix(b22, task_data->n / 2);
    free_matrix(sub_B, task_data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m4(matrix_data *data) {
    matrix_data *task_data = data;

    float **a22 = divide(task_data->matrixA, task_data->n, task_data->n / 2, task_data->n / 2);
    float **b21 = divide(task_data->matrixB, task_data->n, task_data->n / 2, 0);
    float **b11 = divide(task_data->matrixB, task_data->n, 0, 0);

    float **sub_B = subMatrix(b21, b11, task_data->n / 2);

    task_data->result = strassensMultRec(a22, sub_B, task_data->n / 2);

    free_matrix(a22, task_data->n / 2);
    free_matrix(b21, task_data->n / 2);
    free_matrix(b11, task_data->n / 2);
    free_matrix(sub_B, task_data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m5(matrix_data *data) {
    matrix_data *task_data = data;

    float **a11 = divide(task_data->matrixA, task_data->n, 0, 0);
    float **a12 = divide(task_data->matrixA, task_data->n, 0, task_data->n / 2);
    float **b22 = divide(task_data->matrixB, task_data->n, task_data->n / 2, task_data->n / 2);

    float **add_A = addMatrix(a11, a12, task_data->n / 2);

    task_data->result = strassensMultRec(add_A, b22, task_data->n / 2);

    free_matrix(a11, task_data->n / 2);
    free_matrix(a12, task_data->n / 2);
    free_matrix(b22, task_data->n / 2);
    free_matrix(add_A, task_data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m6(matrix_data *data) {
    matrix_data *task_data = data;

    float **a21 = divide(task_data->matrixA, task_data->n, task_data->n / 2, 0);
    float **a11 = divide(task_data->matrixA, task_data->n, 0, 0);
    float **b11 = divide(task_data->matrixB, task_data->n, 0, 0);
    float **b12 = divide(task_data->matrixB, task_data->n, 0, task_data->n / 2);

    float **sub_A = subMatrix(a21, a11, task_data->n / 2);
    float **add_B = addMatrix(b11, b12, task_data->n / 2);

    task_data->result = strassensMultRec(sub_A, add_B, task_data->n / 2);

    free_matrix(a21, task_data->n / 2);
    free_matrix(a11, task_data->n / 2);
    free_matrix(b11, task_data->n / 2);
    free_matrix(b12, task_data->n / 2);
    free_matrix(sub_A, task_data->n / 2);
    free_matrix(add_B, task_data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m7(matrix_data *data) {
    matrix_data *task_data = data;

    float **a12 = divide(task_data->matrixA, task_data->n, 0, task_data->n / 2);
    float **a22 = divide(task_data->matrixA, task_data->n, task_data->n / 2, task_data->n / 2);
    float **b21 = divide(task_data->matrixB, task_data->n, task_data->n / 2, 0);
    float **b22 = divide(task_data->matrixB, task_data->n, task_data->n / 2, task_data->n / 2);

    float **sub_A = subMatrix(a12, a22, task_data->n / 2);
    float **add_B = addMatrix(b21, b22, task_data->n / 2);

    task_data->result = strassensMultRec(sub_A, add_B, task_data->n / 2);

    free_matrix(a12, task_data->n / 2);
    free_matrix(a22, task_data->n / 2);
    free_matrix(b21, task_data->n / 2);
    free_matrix(b22, task_data->n / 2);
    free_matrix(sub_A, task_data->n / 2);
    free_matrix(add_B, task_data->n / 2);

    pthread_exit(NULL);
}