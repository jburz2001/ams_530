#!/bin/bash

#SBATCH --job-name=p3_j
#SBATCH --ntasks-per-node=28
#SBATCH --nodes=1
#SBATCH --time=02:00
#SBATCH -p short-28core

# load g++ compiler
module load mvapich2/gcc12.1/2.3.7

# compile C++ code
mpicxx -o mpi_p3_j /gpfs/home/jburzachiell/code/ams530/project3/jacobi/laplacian_128x128/p3_j.cxx

# execute C++ code
mpirun -np 1  ./mpi_p3_j > p3_j_1.out
mpirun -np 2  ./mpi_p3_j > p3_j_2.out
mpirun -np 4  ./mpi_p3_j > p3_j_4.out
mpirun -np 16  ./mpi_p3_j > p3_j_16.out
