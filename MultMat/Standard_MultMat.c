//
// Created by Fernando Cores Prado on 4/12/23.
//

#include <stdlib.h>
#include <time.h>
#include <memory.h>
#include "Standard_MultMat.h"

double elapsed_std;

/*
* Standard Matrix multiplication with O(n^3) time complexity.
*/
float ** standardMultiplication(float ** matrixA,float ** matrixB,int n)
{
    return standardMultiplication_ijk(matrixA,matrixB,n);
    //return standardMultiplication_ikj(matrixA,matrixB,n);
}

/*
* Standard ijk Matrix multiplication with O(n^3) time complexity.
*/
float ** standardMultiplication_ijk(float ** matrixA,float ** matrixB,int n)
{
    struct timespec start, finish;
    float ** result;
    int i,j,k;

    clock_gettime(CLOCK_MONOTONIC, &start);

    result = (float**)malloc(n*sizeof(float *));
    for(i=0;i<n;i++){
        result[i]=(float*)malloc(n*sizeof(float));
        memset(result[i],0,n*sizeof(float));
        for(j=0;j<n;j++){
            for(k=0;k<n;k++) {
                result[i][j]=result[i][j]+(matrixA[i][k]*matrixB[k][j]);
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
float ** standardMultiplication_ikj(float ** matrixA,float ** matrixB,int n)
{
    struct timespec start, finish;
    float ** result;
    int i,j,k;

    clock_gettime(CLOCK_MONOTONIC, &start);

    result = (float**)malloc(n*sizeof(float *));
    for(i=0;i<n;i++){
        result[i]=(float*)malloc(n*sizeof(float));
        memset(result[i],0,n*sizeof(float));
        for(k=0;k<n;k++) {
            for(j=0;j<n;j++){
                result[i][j]=result[i][j]+(matrixA[i][k]*matrixB[k][j]);
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed_std = (finish.tv_sec - start.tv_sec);
    elapsed_std += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    return result;
}
