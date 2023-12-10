//
// Created by Fernando Cores Prado on 4/12/23.
//
#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"
#include "Errors.h"


/*
* Create the matrix with random floating point numbers betweeen -5 to 5
*/
float ** create(int n,int pow)
{
    if (DEBUG) printMessage("[create] Begin.\n",COLOR_MAGENTA);
    float ** array = createZeroMatrix(pow);
    if (array==NULL)
        Error("[create] Error allocating memory for matrix!");
    if (DEBUG) printMessage("[create] Reserved memory.\n",COLOR_MAGENTA);
    int i,j;
    for(i = 0;i < n; i++) {
        for(j = 0; j < n; j++) {
            array[i][j] = ((((float)rand())/((float)RAND_MAX) *10.0) - 5.0);
        }
    }
    if (DEBUG) printMessage("[create] End.\n",COLOR_MAGENTA);
    return array;
}

/*
* This function creates the matrix and initialize all elements to zero
*/
float ** createZeroMatrix(int n)
{
    int i,j;
    float ** array = (float**)malloc(n*sizeof(float *));
    if (array==NULL)
        return NULL;

    for(i = 0;i < n; i++)
    {
        array[i] = (float*)malloc(n*sizeof(float));
        if (array[i]==NULL)
            return NULL;
        for(j = 0; j < n; j++) {
            array[i][j] = 0.0;
        }
    }
    return array;
}

/*
* Function to Print Matrix
*/
void printMatrix(float ** matrix,int n)
{
    printMatrixC(matrix, n, COLOR_BLUE);
}
/*
* Function to Print Matrix
*/
void printMatrixC(float ** matrix,int n, char *color)
{
    char value[25];

    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            sprintf(value,"% 8.2f \t",matrix[i][j]);
            printMessage(value,color);
        }
        printf("\n");
    }
}


/*
* Function to Save Matrix
*/
void openMatrix(char *inputFile, float *** matrix,int *n)
{
    int i,j;
    FILE *fileMatrix;

    // Create output file
    fileMatrix = fopen(inputFile, "r");
    if (fileMatrix==NULL)
        Error("[openMatrix] Error opening input matrix!");

    fscanf(fileMatrix,"%d\n",n);

    *matrix = createZeroMatrix(*n);
    if (matrix==NULL)
        Error("[openMatrix] Error creating matrix.");

    for(i=0;i<*n;i++){
        for(j=0;j<*n;j++){
            fscanf(fileMatrix,"\t%f",&((*matrix)[i][j]));
        }
    }
    fclose(fileMatrix);
}

/*
* Function to Save Matrix
*/
void saveMatrix(char *outputFile, float ** matrix,int n)
{
    int i,j;
    FILE *fileMatrix;

    // Create output file
    fileMatrix = fopen(outputFile, "w");
    if (fileMatrix==NULL)
        Error("[openMatrix] Error opening output matrix!");

    fprintf(fileMatrix,"%d\n",n);

    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            fprintf(fileMatrix,"% 8.2f \t",matrix[i][j]);
        }
        if (i<(n-1))
            fprintf(fileMatrix,"\n");
    }
    fclose(fileMatrix);
}
