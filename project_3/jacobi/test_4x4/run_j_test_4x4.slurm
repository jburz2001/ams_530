#!/bin/bash

#SBATCH --job-name=p3_j
#SBATCH --ntasks-per-node=28
#SBATCH --nodes=1
#SBATCH --time=02:00
#SBATCH -p short-28core

# load g++ compiler
module load mvapich2/gcc12.1/2.3.7

# compile C++ code
mpicxx -o mpi_p3_j_test_4x4 /gpfs/home/jburzachiell/code/ams530/project3/jacobi/test_4x4/p3_j_test_4x4.cxx

# execute C++ code
mpirun -np 1  ./mpi_p3_j_test_4x4 > p3_j_test_4x4_1.out
mpirun -np 2  ./mpi_p3_j_test_4x4 > p3_j_test_4x4_2.out
mpirun -np 4  ./mpi_p3_j_test_4x4 > p3_j_test_4x4_4.out
