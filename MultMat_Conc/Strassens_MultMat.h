/* ---------------------------------------------------------------
Práctica 4.
Código fuente: Strassens_MultMat.h
Grau Informàtica
48056540H - Aleix Segura Paz.
21161168H - Aniol Serrano Ortega.
--------------------------------------------------------------- */

//
// Created by Fernando Cores Prado on 4/12/23.
//

#ifndef MULTMAT_SEC3_STRASSENS_MULTMAT_H
#define MULTMAT_SEC3_STRASSENS_MULTMAT_H


extern double elapsed_str;

// Functions Prototypes
float **strassensMultiplication(float **, float **, int);

float **standardMultiplication(float **, float **, int);

float **strassensMultRec(float **, float **, int n);

float **divide(float **matrixA, int n, int row, int col);

float **addMatrix(float **, float **, int);

float **subMatrix(float **, float **, int);

void compose(float **, float **, int, int, int);


#endif //MULTMAT_SEC3_STRASSENS_MULTMAT_H
