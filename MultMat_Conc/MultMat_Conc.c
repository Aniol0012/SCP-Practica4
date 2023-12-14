/* ---------------------------------------------------------------
Práctica 4.
Código fuente: MultMat_Conc.c
Grau Informàtica
48056540H - Aleix Segura Paz.
21161168H - Aniol Serrano Ortega.
--------------------------------------------------------------- */

//
// Created by Fernando Cores Prado on 4/12/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Strassens_MultMat.h"
#include "Standard_MultMat.h"
#include "Matrix.h"
#include "Errors.h"

// Constants
#define DEBUG 0
#define DEFAULT_THREADS 4
char *usage_msg = "Usage: MultMat_Conc <MatrixA_File> <MatrixB_File> <ResultMatrix_File>\n       or\n       Strassen_Sec <Matrix_size> <Matrix_RootFolder>\n    or\n       Strassen_Sec <Matrix_size> <Matrix_RootFolder> <Threads>\n";
char *input_folder = "Input";
char *results_folder = "Results";

// Global variables
char debug_msg[256];
struct timespec start, finish;
double elapsed;
int threads = DEFAULT_THREADS;

/*
* Main function where the execution starts.
*/
int main(int argc, char **argv) {
    int n = 0;
    char random_matrixA_name[256], random_matrixB_name[256], std_result_matrix_name[256], str_result_matrix_name[256];
    char *matrixA_name, *matrixB_name;
    float **matrixA, **matrixB;
    float **standardRes, **strassenRes;

    if (DEBUG) {
        sprintf(debug_msg, "[Main] Start Program with %d parameters.\n", argc);
        printMessage(debug_msg, COLOR_MAGENTA);
    }

    threads = atoi(argv[argc - 1]);

    if (threads < 1) {
        threads = DEFAULT_THREADS;
    } else if (DEBUG) {
        sprintf(debug_msg, "[Main] Number of threads2: %d.\n", threads);
        printMessage(debug_msg, COLOR_GREEN);
    }

    if (argc == 3) {
        n = atoi(argv[1]);
        if (n < 1)
            Error("[Main] Error: Invalid matrix dimension!");

        //To handle when n is not power of k we do the padding with zero
        int pow = 1;
        while (pow < n) {
            pow = pow * 2;
        }
        matrixA = create(n, pow);
        matrixB = create(n, pow);
        n = pow;

        sprintf(debug_msg, "[Main] Calculating random %dx%d matrices multiplication.\n", n, n);
        printMessage(debug_msg, COLOR_CYAN);

        sprintf(random_matrixA_name, "%s/%s/MatrixA_%dx%d.txt", argv[2], input_folder, n, n);
        saveMatrix(random_matrixA_name, matrixA, n);
        sprintf(random_matrixB_name, "%s/%s/MatrixB_%dx%d.txt", argv[2], input_folder, n, n);
        saveMatrix(random_matrixB_name, matrixB, n);
        matrixA_name = random_matrixA_name;
        matrixB_name = random_matrixB_name;
        sprintf(std_result_matrix_name, "%s/%s/MatrixRes_%dx%d.txt.std", argv[2], results_folder, n, n);
        sprintf(str_result_matrix_name, "%s/%s/MatrixRes_%dx%d.txt.str", argv[2], results_folder, n, n);
    } else if (argc == 4) {
        int n1, n2;
        double int_part;
        openMatrix(argv[1], &matrixA, &n1);
        openMatrix(argv[2], &matrixB, &n2);
        if (n1 != n2)
            Error("[Main]: Error input matrices have differente dimensions!");
        n = n1;

        int pow = 1;
        while (pow < n) {
            pow = pow * 2;
        }
        if (n != pow)
            Error("[Main]: Error input matrices are not power of k!");

        matrixA_name = argv[1];
        matrixA_name = argv[2];
        sprintf(std_result_matrix_name, "%s.std", argv[3]);
        sprintf(str_result_matrix_name, "%s.str", argv[3]);
    } else {
        printMessage(usage_msg, COLOR_RED);
        Error("[Main]: Invalid number of arguments!\n\n");
    }

    if (n < 10) {
        printMessage("Matrix A:\n", COLOR_GREEN_B);
        printMatrixC(matrixA, n, COLOR_GREEN_B);

        printMessage("Matrix B:\n", COLOR_GREEN_B);
        printMatrixC(matrixB, n, COLOR_GREEN_B);
    }

    float **stdRes = standardMultiplication(matrixA, matrixB, n);
    if (n < 10) {
        print("Standard Multiplication Result:\n");
        printMatrix(stdRes, n);
    }
    saveMatrix(std_result_matrix_name, stdRes, n);
    sprintf(debug_msg, "[Standard Mult] Multiplication time: %05.6f.\n", elapsed_std);
    printMessage(debug_msg, COLOR_CYAN_B);


    float **strassensRes = strassensMultiplication(matrixA, matrixB, n);
    if (n < 10) {
        print("Strassen's Multiplication Result:\n");
        printMatrix(strassensRes, n);
    }
    saveMatrix(str_result_matrix_name, strassensRes, n);
    sprintf(debug_msg, "[Strassen Mult] Multiplication time: %05.6f.\n", elapsed_str);
    printMessage(debug_msg, COLOR_CYAN_B);

    return 0;
}

