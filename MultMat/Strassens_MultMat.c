//
// Created by Fernando Cores Prado on 4/12/23.
//

#include "Strassens_MultMat.h"
#include <time.h>
#include <stdlib.h>
#include "Matrix.h"

double elapsed_str;
int Dim2StopRecursivity = 10;


/*
* Wrapper function over strassensMultRec.
*/
float ** strassensMultiplication(float ** matrixA, float** matrixB,int n)
{
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    if (n>32)
        Dim2StopRecursivity = n/16;

    float ** result = strassensMultRec(matrixA,matrixB,n);

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed_str = (finish.tv_sec - start.tv_sec);
    elapsed_str += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    return result;
}

/*
* Strassen's Multiplication algorithm using Divide and Conquer technique.
*/
float** strassensMultRec(float ** matrixA, float** matrixB,int n){
    float ** result = createZeroMatrix(n);
    if(n>Dim2StopRecursivity) {
        //Divide the matrix
        float ** a11 = divide(matrixA, n, 0, 0);
        float ** a12 = divide(matrixA, n, 0, (n/2));
        float ** a21 = divide(matrixA, n, (n/2), 0);
        float ** a22 = divide(matrixA, n, (n/2), (n/2));
        float ** b11 = divide(matrixB, n, 0, 0);
        float ** b12 = divide(matrixB, n, 0, n/2);
        float ** b21 = divide(matrixB, n, n/2, 0);
        float ** b22 = divide(matrixB, n, n/2, n/2);

        //Recursive call for Divide and Conquer
        float** m1= strassensMultRec(addMatrix(a11,a22,n/2),addMatrix(b11,b22,n/2),n/2);
        float** m2= strassensMultRec(addMatrix(a21,a22,n/2),b11,n/2);
        float** m3= strassensMultRec(a11,subMatrix(b12,b22,n/2),n/2);
        float** m4= strassensMultRec(a22,subMatrix(b21,b11,n/2),n/2);
        float** m5= strassensMultRec(addMatrix(a11,a12,n/2),b22,n/2);
        float** m6= strassensMultRec(subMatrix(a21,a11,n/2),addMatrix(b11,b12,n/2),n/2);
        float** m7= strassensMultRec(subMatrix(a12,a22,n/2),addMatrix(b21,b22,n/2),n/2);
        free(a11); free(a12); free(a21); free(a22);
        free(b11); free(b12); free(b21); free(b22);

        float** c11 = addMatrix(subMatrix(addMatrix(m1,m4,n/2),m5,n/2),m7,n/2);
        float** c12 = addMatrix(m3,m5,n/2);
        float** c21 = addMatrix(m2,m4,n/2);
        float** c22 = addMatrix(subMatrix(addMatrix(m1,m3,n/2),m2,n/2),m6,n/2);
        free(m1); free(m2); free(m3); free(m4);
        free(m5); free(m6); free(m7);

        //Compose the matrix
        compose(c11,result,0,0,n/2);
        compose(c12,result,0,n/2,n/2);
        compose(c21,result,n/2,0,n/2);
        compose(c22,result,n/2,n/2,n/2);

        free(c11); free(c12); free(c21); free(c22);
    }
    else {
        //This is the terminating condition for recurssion.
        //result[0][0]=matrixA[0][0]*matrixB[0][0];
        result = standardMultiplication(matrixA,matrixB, n);
    }
    return result;
}

/*
* This method combines the matrix in the result matrix
*/
void compose(float** matrix,float** result,int row,int col,int n){
    int i,j,r=row,c=col;
    for(i=0;i<n;i++){
        c=col;
        for(j=0;j<n;j++){
            result[r][c]=matrix[i][j];
            c++;
        }
        r++;
    }
}

/*
* Sub-divide the matrix according to row and col specified
*/
float** divide(float ** matrix,int n, int row,int col) {
    int n_new=n/2;

    float ** array = createZeroMatrix(n_new);
    int i,j,r=row,c=col;
    for(i = 0;i < n_new; i++) {
        c=col;
        for(j = 0; j < n_new; j++) {
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
float** addMatrix(float** matrixA,float** matrixB,int n){
    float ** res = createZeroMatrix(n);
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            res[i][j]=matrixA[i][j]+matrixB[i][j];

    return res;
}

/*
* Substract the two matrix
*/
float** subMatrix(float** matrixA,float** matrixB,int n){
    float ** res = createZeroMatrix(n);
    int i,j;
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            res[i][j]=matrixA[i][j]-matrixB[i][j];

    return res;
}

