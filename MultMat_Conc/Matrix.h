/* ---------------------------------------------------------------
Práctica 4.
Código fuente: Matrix.h
Grau Informàtica
48056540H - Aleix Segura Paz.
21161168H - Aniol Serrano Ortega.
--------------------------------------------------------------- */

//
// Created by Fernando Cores Prado on 4/12/23.
//

#ifndef MULTMAT_SEC3_MATRIX_H
#define MULTMAT_SEC3_MATRIX_H

// Functions Prototypes
float **create(int, int);

float **createZeroMatrix(int);

void printMatrix(float **, int);

void printMatrixC(float **matrix, int n, char *color);

void openMatrix(char *inputFile, float ***matrix, int *n);

void saveMatrix(char *outputFile, float **matrix, int n);

#endif //MULTMAT_SEC3_MATRIX_H
