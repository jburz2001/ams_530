#!/bin/bash

#SBATCH --job-name=p4_custom_b_10to6
#SBATCH --output=p4_custom_b_10to6.out
#SBATCH --ntasks-per-node=16
#SBATCH --nodes=1
#SBATCH --time=02:00
#SBATCH -p short-40core

# Load g++ compiler
module load gcc-stack

# Compile C++ code
mpicxx -o mpi_p4_custom_b_10to6 /gpfs/home/jburzachiell/code/ams530/project4/b/10to6/p4_custom_b_10to6.cxx

# Execute C++ code
mpirun -np 32 /gpfs/home/jburzachiell/code/ams530/project4/b/10to6/mpi_p4_custom_b_10to6
