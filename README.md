# SCP-Practica4

## Description

This project, part of the *Sistemes Concurrents i Paral·lels* (SCP[^1]) subject, explores concurrent Matrix 
Multiplication using two different methods: [standard](#standard-matrix-multiplication) and [strassen](#strassen-matrix-multiplication).
The objective is to demonstrate and compare the efficiency of these methods in a concurrent computing context.
This project is developed for the Computer Science degree at the University of Lleida ([UDL](https://udl.cat)), Spain.

> [^1]: **SCP** Stands for *Sistemes Concurrents i Paral·lels* (Concurrent and Parallel Systems) which is the Catalan name for the subject.

## Standard Matrix Multiplication

Standard matrix multiplication is a straightforward approach where each element of the resulting matrix is computed as 
the dot product of the corresponding row and column from the two matrices. These methods use an $O(n^3)$ time complexity, 
since is necessary to nest three loops to iterate over the three dimensions of the resulting matrix.

A visual example of the standard matrix multiplication is shown below:



There are to approaches to implement this method, ijk and ikj. The first one is the most common and the second one is
the most efficient one. The reason is that the first one is not cache friendly, since it iterates over the rows of the

**ijk** matrix multiplication should look like this:

```c
int A[n][n], B[n][n], C[n][n];
int i, j, k;

for (i = 0; i < n; i++) {
    for (k = 0; k < n; k++) {
        for (j = 0; j < n; j++) {
            C[i][k] += A[i][j] * B[j][k];
        }
    }
}
```

> [!TIP]
> Check [geeksforgeeks](https://www.geeksforgeeks.org/c-program-multiply-two-matrices/) for more information about standard matrix multiplication


## Strassen Matrix Multiplication

The Strassen method is an advanced algorithm that divides the matrices into smaller ones, reducing the number of multiplicative operations, thus potentially improving performance on large matrices.


> [!TIP]
> Check [geeksforgeeks](https://www.geeksforgeeks.org/strassens-matrix-multiplication/) for more information about strassen matrix multiplication

## Documentation

- To check the assignment of this file check [assignment](./Practica4_SCP.pdf) file.
- To check the documentation of this project check [report](./report.pdf) file.

## Installation
Clone the repo:

```shell
git clone https://github.com/Aniol0012/SCP-Practica4.git
```

## Usage

### Automatic
Just open the project with [CLion](https://www.jetbrains.com/clion/) and run it. You can change parameters as shown below:


### Manual

To run programs manually you first need to compile them:

```shell
cd MultMat_Conc
```

```shell
make
```

Then, you can choose between run **[main](./MultMat_Conc/MultMat_Conc.c)** or **[tests](./MultMat_Conc/MultMat_Test.c)**:

#### Main
There are three ways to run the main program:

  1. First option:
     ````shell
     ./MultMat_Conc <MatrixA_File> <MatrixB_File> <ResultMatrix_File>
     ````
  1. Second option:
     ````shell
     ./MultMat_Conc <Matrix_size> <Matrix_RootFolder>
     ````
  1. Third option:
     ````shell
     ./MultMat_Conc <Matrix_size> <Matrix_RootFolder> <Threads>
     ````

#### Tests

```shell
./MultMat_Test <Trheads>
```


## Authors

- [@Aniol0012](https://github.com/Aniol0012)
- [@aleixsegura](https://github.com/aleixsegura)
