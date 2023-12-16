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

typedef struct {
    float **matrixA;
    float **matrixB;
    int n;
    float **result;
} matrix_data;

/*
* Wrapper function over strassensMultRec.
*/
float **strassensMultiplication(float **matrixA, float **matrixB, int n) {
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    if (n > 32)
        Dim2StopRecursivity = n / 16;

    float **result = strassensMultRec(matrixA, matrixB, n);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed_str = (finish.tv_sec - start.tv_sec);
    elapsed_str += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    return result;
}

void freeMatrix(float **matrix, int n) {
    if (matrix != NULL) {
        for (int i = 0; i < n; i++) {
            if (matrix[i] != NULL) {
                free(matrix[i]);
            }
        }
        free(matrix);
    }
}


void *calculate_m1(void *arg) {
    matrix_data *data = (matrix_data *) arg;
    float **a11 = divide(data->matrixA, data->n, 0, 0);
    float **a22 = divide(data->matrixA, data->n, data->n / 2, data->n / 2);
    float **b11 = divide(data->matrixB, data->n, 0, 0);
    float **b22 = divide(data->matrixB, data->n, data->n / 2, data->n / 2);

    float **sumA = addMatrix(a11, a22, data->n / 2);
    float **sumB = addMatrix(b11, b22, data->n / 2);

    data->result = standardMultiplication(sumA, sumB, data->n / 2);

    freeMatrix(a11, data->n / 2);
    freeMatrix(a22, data->n / 2);
    freeMatrix(b11, data->n / 2);
    freeMatrix(b22, data->n / 2);
    freeMatrix(sumA, data->n / 2);
    freeMatrix(sumB, data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m2(void *arg) {
    matrix_data *data = (matrix_data *) arg;
    float **a21 = divide(data->matrixA, data->n, data->n / 2, 0);
    float **a22 = divide(data->matrixA, data->n, data->n / 2, data->n / 2);
    float **b11 = divide(data->matrixB, data->n, 0, 0);

    float **sumA = addMatrix(a21, a22, data->n / 2);

    data->result = standardMultiplication(sumA, b11, data->n / 2);

    freeMatrix(a21, data->n / 2);
    freeMatrix(a22, data->n / 2);
    freeMatrix(b11, data->n / 2);
    freeMatrix(sumA, data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m3(void *arg) {
    matrix_data *data = (matrix_data *) arg;
    float **a11 = divide(data->matrixA, data->n, 0, 0);
    float **b12 = divide(data->matrixB, data->n, 0, data->n / 2);
    float **b22 = divide(data->matrixB, data->n, data->n / 2, data->n / 2);

    float **subB = subMatrix(b12, b22, data->n / 2);

    data->result = standardMultiplication(a11, subB, data->n / 2);

    freeMatrix(a11, data->n / 2);
    freeMatrix(b12, data->n / 2);
    freeMatrix(b22, data->n / 2);
    freeMatrix(subB, data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m4(void *arg) {
    matrix_data *data = (matrix_data *) arg;
    float **a22 = divide(data->matrixA, data->n, data->n / 2, data->n / 2);
    float **b21 = divide(data->matrixB, data->n, data->n / 2, 0);
    float **b11 = divide(data->matrixB, data->n, 0, 0);

    float **subB = subMatrix(b21, b11, data->n / 2);

    data->result = standardMultiplication(a22, subB, data->n / 2);

    freeMatrix(a22, data->n / 2);
    freeMatrix(b21, data->n / 2);
    freeMatrix(b11, data->n / 2);
    freeMatrix(subB, data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m5(void *arg) {
    matrix_data *data = (matrix_data *) arg;
    float **a11 = divide(data->matrixA, data->n, 0, 0);
    float **a12 = divide(data->matrixA, data->n, 0, data->n / 2);
    float **b22 = divide(data->matrixB, data->n, data->n / 2, data->n / 2);

    float **sumA = addMatrix(a11, a12, data->n / 2);

    data->result = standardMultiplication(sumA, b22, data->n / 2);

    freeMatrix(a11, data->n / 2);
    freeMatrix(a12, data->n / 2);
    freeMatrix(b22, data->n / 2);
    freeMatrix(sumA, data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m6(void *arg) {
    matrix_data *data = (matrix_data *) arg;
    float **a21 = divide(data->matrixA, data->n, data->n / 2, 0);
    float **a11 = divide(data->matrixA, data->n, 0, 0);
    float **b11 = divide(data->matrixB, data->n, 0, 0);
    float **b12 = divide(data->matrixB, data->n, 0, data->n / 2);

    float **subA = subMatrix(a21, a11, data->n / 2);
    float **sumB = addMatrix(b11, b12, data->n / 2);

    data->result = standardMultiplication(subA, sumB, data->n / 2);

    freeMatrix(a21, data->n / 2);
    freeMatrix(a11, data->n / 2);
    freeMatrix(b11, data->n / 2);
    freeMatrix(b12, data->n / 2);
    freeMatrix(subA, data->n / 2);
    freeMatrix(sumB, data->n / 2);

    pthread_exit(NULL);
}

void *calculate_m7(void *arg) {
    matrix_data *data = (matrix_data *) arg;
    float **a12 = divide(data->matrixA, data->n, 0, data->n / 2);
    float **a22 = divide(data->matrixA, data->n, data->n / 2, data->n / 2);
    float **b21 = divide(data->matrixB, data->n, data->n / 2, 0);
    float **b22 = divide(data->matrixB, data->n, data->n / 2, data->n / 2);

    float **subA = subMatrix(a12, a22, data->n / 2);
    float **sumB = addMatrix(b21, b22, data->n / 2);

    data->result = standardMultiplication(subA, sumB, data->n / 2);

    freeMatrix(a12, data->n / 2);
    freeMatrix(a22, data->n / 2);
    freeMatrix(b21, data->n / 2);
    freeMatrix(b22, data->n / 2);
    freeMatrix(subA, data->n / 2);
    freeMatrix(sumB, data->n / 2);

    pthread_exit(NULL);
}


/*
* Strassen's Multiplication algorithm using Divide and Conquer technique.
*/
float **strassensMultRec(float **matrixA, float **matrixB, int n) {
    float **result = createZeroMatrix(n);
    if (n > Dim2StopRecursivity) {
        pthread_t threads[7];
        matrix_data data[7];

        for (int i = 0; i < 7; i++) {
            data[i].matrixA = matrixA;
            data[i].matrixB = matrixB;
            data[i].n = n;
            data[i].result = NULL;
        }

        //Recursive call for Divide and Conquer
        pthread_create(&threads[0], NULL, calculate_m1, (void *) &data[0]);
        pthread_create(&threads[1], NULL, calculate_m2, (void *) &data[1]);
        pthread_create(&threads[2], NULL, calculate_m3, (void *) &data[2]);
        pthread_create(&threads[3], NULL, calculate_m4, (void *) &data[3]);
        pthread_create(&threads[4], NULL, calculate_m5, (void *) &data[4]);
        pthread_create(&threads[5], NULL, calculate_m6, (void *) &data[5]);
        pthread_create(&threads[6], NULL, calculate_m7, (void *) &data[6]);


        for (int i = 0; i < 7; i++) {
            pthread_join(threads[i], NULL);
        }

        float **m1 = data[0].result;
        float **m2 = data[1].result;
        float **m3 = data[2].result;
        float **m4 = data[3].result;
        float **m5 = data[4].result;
        float **m6 = data[5].result;
        float **m7 = data[6].result;

        // Calcular c11, c12, c21, c22
        float **temp1, **temp2;

        temp1 = addMatrix(m1, m4, n / 2);
        temp2 = subMatrix(temp1, m5, n / 2);
        float **c11 = addMatrix(temp2, m7, n / 2);
        freeMatrix(temp1, n / 2);
        freeMatrix(temp2, n / 2);

        float **c12 = addMatrix(m3, m5, n / 2);

        float **c21 = addMatrix(m2, m4, n / 2);

        temp1 = subMatrix(m1, m2, n / 2);
        temp2 = addMatrix(temp1, m3, n / 2);
        float **c22 = addMatrix(temp2, m6, n / 2);
        freeMatrix(temp1, n / 2);
        freeMatrix(temp2, n / 2);

        // Componer la matriz resultante
        compose(c11, result, 0, 0, n / 2);
        compose(c12, result, 0, n / 2, n / 2);
        compose(c21, result, n / 2, 0, n / 2);
        compose(c22, result, n / 2, n / 2, n / 2);

        // Liberar matrices temporales
        freeMatrix(c11, n / 2);
        freeMatrix(c12, n / 2);
        freeMatrix(c21, n / 2);
        freeMatrix(c22, n / 2);


        for (int i = 0; i < 7; i++) {
            free(data[i].result);
        }
    } else {
        //This is the terminating condition for recurssion.
        //result[0][0]=matrixA[0][0]*matrixB[0][0];
        result = standardMultiplication(matrixA, matrixB, n);
    }
    return result;
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

