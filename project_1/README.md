# Instructions

To run this code, please first load the Slurm module into the HPC cluster of your choice.
Next, open either the p1_builtin.cxx or p1_custom.cxx C++ source code files in order to adjust N.
The N variable from the project instructions is called vector_size in the code.
Once the value of N (aka vector_size) is set the value you want, use Slurm to schedule a job to run this code.
Additionally, it is recommended to alter the corresponding run_custom.slurm or run_builtin.slurm file to create an output file with a name corresponding to the value of N (aka vector_size) that you chose.
Finally, use the sbatch command with either the run_custom.slurm or run_builtin.slurm to schedule a job to run this code and generate the output results.