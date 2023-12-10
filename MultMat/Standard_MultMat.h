//
// Created by Fernando Cores Prado on 4/12/23.
//

#ifndef MULTMAT_SEC3_STANDARD_MULTMAT_H
#define MULTMAT_SEC3_STANDARD_MULTMAT_H

extern double elapsed_std;

float ** standardMultiplication(float ** matrixA,float ** matrixB,int n);
float ** standardMultiplication_ijk(float ** matrixA,float ** matrixB,int n);
float ** standardMultiplication_ikj(float ** matrixA,float ** matrixB,int n);

#endif //MULTMAT_SEC3_STANDARD_MULTMAT_H
