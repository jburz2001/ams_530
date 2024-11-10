The code in this directory implements parallel Jacobi iteration or Gauss-Seidel iteration in the respective directories. 
Both the Jacobi and the Gauss-Seidel code directories contain a subdirectory called /laplacian_128x128. This subdirectory stores the code and results for solving the problem assigned in the homework file.
However, the directory for Jacobi also include additional subdirectories that were used for testing the validity of the Jacobi iteration code.
Gauss-Seidel does not contain test code for verifying the output of its code since its results for solving the homework problem align with those obained by Jacobi, and Jacobi yielded correct results.
To run code in the respective directories please run the run.sh script. This will call sbatch on the respective slurm script.
Results are outputted for each number of processors considered in the slurm script, which is usually 1, 2, 4, and 16.
