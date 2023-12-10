cmake_minimum_required(VERSION 3.23)
project(MultMat_Sec3 C)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES Errors.c Matrix.c Standard_MultMat.c Strassens_MultMat.c)

add_executable(MultMat_Sec MultMat_Sec.c  ${SOURCE_FILES} )
add_executable(MultMat_Test MultMat_Test.c ${SOURCE_FILES} )
