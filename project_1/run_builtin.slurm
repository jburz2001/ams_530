#!/bin/bash

#SBATCH --job-name=p1
#SBATCH --output=p1_builtin_10to8.out
#SBATCH --ntasks-per-node=16
#SBATCH --nodes=1
#SBATCH --time=02:00
#SBATCH -p short-28core
#SBATCH --mail-type=END
#SBATCH --mail-user=justin.burzachiello@stonybrook.edu

# load g++ compiler
module load mvapich2/gcc12.1/2.3.7

# compile C++ code
mpicxx -o mpi_p1 /gpfs/home/jburzachiell/code/ams361/project1/p1_builtin.cxx

# execute C++ code
mpirun -np 16 ./mpi_p1

