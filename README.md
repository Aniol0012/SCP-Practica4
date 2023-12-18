# SCP-Practica4

---
## Table of Contents
1. 📝 [Description](#description)
2. 🧮 [Standard Matrix Multiplication](#standard-matrix-multiplication)
3. 🌀 [Strassen Matrix Multiplication](#strassen-matrix-multiplication)
4. ⚙️ [Concurrent Implementation](#concurrent-implementation)
    - 📊 [Concurrent Standard Matrix Multiplication](#concurrent-standard-matrix-multiplication)
    - 🌐 [Concurrent Strassen Matrix Multiplication](#concurrent-strassen-matrix-multiplication)
5. 📚 [Documentation](#documentation)
6. 📦 [Installation](#installation)
7. 🚀 [Usage](#usage)
    - 🧑‍💻 [Automatic](#automatic)
    - ✍️ [Manual](#manual)
        - 🌟 [Main](#main)
        - 🧪 [Tests](#tests)
8. 👥 [Authors](#authors)
9. 📄 [License](#license)

---

## 📝 Description

This project, part of the *Sistemes Concurrents i Paral·lels* (SCP[^1]) subject, explores concurrent Matrix 
Multiplication using two different methods: [standard](#standard-matrix-multiplication) and [Strassen](#strassen-matrix-multiplication).
The objective is to demonstrate and compare the efficiency of these methods in a concurrent computing context.
This project is developed for the Computer Science degree at the University of Lleida ([UDL](https://udl.cat)), Spain.

> [^1]: **SCP** Stands for *Sistemes Concurrents i Paral·lels* (Concurrent and Parallel Systems) which is the Catalan name for the subject.

## 🧮 Standard Matrix Multiplication

Standard matrix multiplication is a straightforward approach where each element of the resulting matrix is computed as 
the dot product of the corresponding row and column from the two matrices. These methods use an $O(n^3)$ time complexity, 
since is necessary to nest three loops to iterate over the three dimensions of the resulting matrix.

A visual example of the standard matrix multiplication is shown below:

![bad3a8b38db64a918543146979adcea0](https://github.com/Aniol0012/SCP-Practica4/assets/53788631/6077d953-70de-4888-86b1-2180ddf95774)

There are two approaches to implement this method, **ijk** and **ikj**. The first one is the most **common** and the second one is
the most **efficient** one. The reason is that the first one is not cache friendly, since it iterates over the rows of the
first matrix, then over the columns of the second matrix and finally over the columns of the first matrix. This means
that the elements of the first matrix are not stored in the cache, since the iteration is not sequential. The second
approach is more cache friendly, since it iterates over the rows of the first matrix, then over the columns of the first
matrix and finally over the columns of the second matrix. This means that the elements of the first matrix are stored in
the cache, since the iteration is sequential.

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


## 🌀 Strassen Matrix Multiplication

The Strassen method is an advanced algorithm that divides the matrices into smaller ones, reducing the number of multiplicative operations, thus potentially improving performance on large matrices.
A visual example of this is shown below:

![strassen_new](https://github.com/Aniol0012/SCP-Practica4/assets/53788631/5abfa375-036d-4312-8a06-5eec0c11c30f)


> [!TIP]
> Check [geeksforgeeks](https://www.geeksforgeeks.org/strassens-matrix-multiplication/) for more information about Strassen matrix multiplication

## ⚙️ Concurrent implementation
These implementations dive us to the concurrent implementation of the algorithms.

### 📊 Concurrent Standard Matrix Multiplication

This implementation is based on the [standard](#standard-matrix-multiplication) one, but with the difference that it uses
threads to compute the result. The idea is to divide the matrix into blocks and compute each block in a different thread.

For this concurrent implementation it has been made from the sequential version of matrix multiplication ijk. Comparisons 
between sequential and concurrent times have been made between these two versions of matrix multiplication ijk.

You can check this implementation at [Standard_MultMat.c](./MultMat_Conc/Standard_MultMat.c) file.

### 🌐 Concurrent Strassen Matrix Multiplication

A conceptual function calling can be shown like this:

![strassens_1](https://github.com/Aniol0012/SCP-Practica4/assets/53788631/80bf5883-a392-4c50-bda1-5ecf2ea8fd07)

> [!NOTE]
> Take in account that by each mX call (*calculate_mx()*) it generates 7 new threads.

## 📚 Documentation

- To check the assignment of this file check [assignment](./Practica4_SCP.pdf) file.
- To check the documentation of this project check [report](./report.pdf) file.

## 📦 Installation
Clone the repo:

```shell
git clone https://github.com/Aniol0012/SCP-Practica4.git
```

## 🚀 Usage

### 🧑‍💻 Automatic
Just open the project with [CLion](https://www.jetbrains.com/clion/) and run it. You can change parameters as shown below:

![Screenshot_1](https://github.com/Aniol0012/SCP-Practica4/assets/53788631/ded2b73b-9161-45bd-9baf-72593cb3dc42)

### ✍️ Manual

To run programs manually you first need to compile them:

```shell
cd MultMat_Conc
```

```shell
make
```

Then, you can choose between run **[main](./MultMat_Conc/MultMat_Conc.c)** or **[tests](./MultMat_Conc/MultMat_Test.c)**:

#### 🌟 Main
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

#### 🧪 Tests

```shell
./MultMat_Test <Trheads>
```


## 👥 Authors

- [@Aniol0012](https://github.com/Aniol0012)
- [@aleixsegura](https://github.com/aleixsegura)

## 📄 License

[MIT](https://choosealicense.com/licenses/mit/)