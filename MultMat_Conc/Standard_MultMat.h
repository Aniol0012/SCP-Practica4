/* ---------------------------------------------------------------
Práctica 4.
Código fuente: Standard_MultMat.h
Grau Informàtica
48056540H - Aleix Segura Paz.
21161168H - Aniol Serrano Ortega.
--------------------------------------------------------------- */

//
// Created by Fernando Cores Prado on 4/12/23.
//

#ifndef MULTMAT_SEC3_STANDARD_MULTMAT_H
#define MULTMAT_SEC3_STANDARD_MULTMAT_H

extern double elapsed_std;
extern int threads;

float **standardMultiplication(float **matrixA, float **matrixB, int n);

float **standardMultiplication_ijk(float **matrixA, float **matrixB, int n);

float **standardMultiplication_ikj(float **matrixA, float **matrixB, int n);

#endif //MULTMAT_SEC3_STANDARD_MULTMAT_H
