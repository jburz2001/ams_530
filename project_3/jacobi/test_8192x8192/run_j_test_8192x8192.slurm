#!/bin/bash

#SBATCH --job-name=p3_j
#SBATCH --ntasks-per-node=28
#SBATCH --nodes=1
#SBATCH --time=02:00
#SBATCH -p short-28core

# load g++ compiler
module load mvapich2/gcc12.1/2.3.7

# compile C++ code
mpicxx -o mpi_p3_j_test_8192x8192 /gpfs/home/jburzachiell/code/ams530/project3/jacobi/test_8192x8192/p3_j_test_8192x8192.cxx

# execute C++ code
mpirun -np 1  ./mpi_p3_j_test_8192x8192 > p3_j_test_8192x8192_1.out
mpirun -np 2  ./mpi_p3_j_test_8192x8192 > p3_j_test_8192x8192_2.out
mpirun -np 4  ./mpi_p3_j_test_8192x8192 > p3_j_test_8192x8192_4.out
mpirun -np 8  ./mpi_p3_j_test_8192x8192 > p3_j_test_8192x8192_8.out
mpirun -np 16  ./mpi_p3_j_test_8192x8192 > p3_j_test_8192x8192_16.out
