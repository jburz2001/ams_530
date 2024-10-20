#!/bin/bash

#SBATCH --job-name=p2
#SBATCH --output=p2.out
#SBATCH --ntasks-per-node=32
#SBATCH --nodes=1
#SBATCH --time=02:00
#SBATCH -p short-40core

# load g++ compiler
module load mvapich2/gcc12.1/2.3.7

# compile C++ code
mpicxx -o mpi_p2 /gpfs/home/jburzachiell/code/ams530/project2/p2.cxx

# execute C++ code
mpirun -np 32 ./mpi_p2

