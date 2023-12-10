//
// Created by Fernando Cores Prado on 4/12/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Strassens_MultMat.h"
#include "Standard_MultMat.h"
#include "Matrix.h"
#include "Errors.h"

// Constants
#define DEBUG 0
char *input_path = ".";
char *input_folder = "Input";
char *results_path = ".";
char *results_folder = "Results";
char *tests_path = ".";
char *tests_folder = "Test";

// Functions Prototypes
void DoMatrixMultiplication(int n);

/*
* Main function where the execution starts.
*/
int main(int argc, char ** argv)
{

    DoMatrixMultiplication(4);
    DoMatrixMultiplication(16);
    DoMatrixMultiplication(64);
    DoMatrixMultiplication(128);
    DoMatrixMultiplication(512);

    DoMatrixMultiplication(1024);
    DoMatrixMultiplication(2048);

    return 0;
}

void DoMatrixMultiplication(int n)
{
    int n1, n2;
    double int_part;
    char matrixA_name[256], matrixB_name[256], std_result_matrix_name[256], str_result_matrix_name[256];
    char debug_msg [256];
    float **matrixA, **matrixB;

    sprintf(debug_msg,"[Main] Testing Matrix %dx%d multiplication.\n",n,n);
    printMessage(debug_msg,COLOR_CYAN);

    // Read input matrices.
    sprintf(matrixA_name, "%s/%s/MatrixA_%dx%d.txt", input_path,input_folder, n, n);
    openMatrix(matrixA_name, &matrixA,&n1);
    sprintf(matrixB_name, "%s/%s/MatrixB_%dx%d.txt", input_path, input_folder, n, n);
    openMatrix(matrixB_name, &matrixB,&n2);
    if (n1!=n2)
        Error("[Main]: Error input matrices have differente dimensions!");
    n=n1;

    int pow = 1;
    while (pow < n) {
        pow = pow * 2;
    }
    if (n!=pow)
        Error("[Main]: Error input matrices are not power of k!");

    sprintf(std_result_matrix_name, "%s/%s/MatrixRes_%dx%d.txt.std", tests_path, tests_folder, n, n);
    sprintf(str_result_matrix_name, "%s/%s/MatrixRes_%dx%d.txt.str", tests_path, tests_folder, n, n);

    float ** stdRes = standardMultiplication(matrixA,matrixB,n);
    if (n<10) {
        print("Standard Multiplication Result:\n");
        printMatrix(stdRes, n);
    }
    saveMatrix(std_result_matrix_name, stdRes, n);
    sprintf(debug_msg,"[Standard Mult] %dx%d Multiplication time: %05.6f.\n",n, n, elapsed_std);
    printMessage(debug_msg,COLOR_CYAN_B);

    float ** strassensRes = strassensMultiplication(matrixA,matrixB,n);
    if (n<10) {
        print("Strassen's Multiplication Result:\n");
        printMatrix(strassensRes, n);
    }
    saveMatrix(str_result_matrix_name, strassensRes, n);
    sprintf(debug_msg,"[Strassen Mult] %dx%d Multiplication time: %05.6f.\n",n, n, elapsed_str);
    printMessage(debug_msg,COLOR_CYAN_B);

    free(stdRes);
    free(strassensRes);
}

