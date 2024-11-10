#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

const int n = 128;  // Size of the matrix
const double tolerance = 1e-6;
const int max_iterations = 1000;

void create_laplacian_matrix(std::vector<std::vector<double>>& A) {
    //
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
	    if (i == j) {
 		A[i][j] = 1000;
	    } else {
		A[i][j] = 1;
	    }
        }
    }
    //

    /*
    int grid_size = std::sqrt(n);  // Assumes n = 128 corresponds to a 2D grid 16x8

    for (int i = 0; i < n; ++i) {
        A[i][i] = 4;  // Set diagonal elements to 4

        // Left neighbor
        if (i % grid_size != 0) {  // Not in the first column
            A[i][i - 1] = -1;
        }
        // Right neighbor
        if (i % grid_size != grid_size - 1) {  // Not in the last column
            A[i][i + 1] = -1;
        }
        // Top neighbor
        if (i - grid_size >= 0) {  // Not in the first row
            A[i][i - grid_size] = -1;
        }
        // Bottom neighbor
        if (i + grid_size < n) {  // Not in the last row
            A[i][i + grid_size] = -1;
        }
    }
    */
}

void create_b_vector(std::vector<double>& b) {
    //
    for (int i = 0; i < n; i++) {
	b[i] = 1;
    }
    //

    /*
    for (int i = 0; i < n; i += 4) {
        b[i] = 1;     // Pattern: 1, 1, 0, 0
        b[i + 1] = 1;
        b[i + 2] = 0;
        b[i + 3] = 0;
    }
    */
}

// Function to compute the infinity norm of the residual (error)
double compute_infinity_norm(const std::vector<std::vector<double>>& A, const std::vector<double>& x, const std::vector<double>& b) {
    std::vector<double> residual(n, 0.0);
    for (int i = 0; i < n; ++i) {
        residual[i] = -b[i];
        for (int j = 0; j < n; ++j) {
            residual[i] += A[i][j] * x[j];
        }
    }
    return *std::max_element(residual.begin(), residual.end(), [](double a, double b) {
        return std::fabs(a) < std::fabs(b);
    });
}

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure n is divisible by the number of processors
    if (n % size != 0) {
        if (rank == 0) {
            std::cerr << "Number of processors must evenly divide matrix size." << std::endl;
        }
        MPI_Finalize();
        return -1;
    }

    int rows_per_proc = n / size;
    int start_row = rank * rows_per_proc;
    int end_row = start_row + rows_per_proc;

    // Initialize Laplacian matrix A
    std::vector<std::vector<double>> local_A(rows_per_proc, std::vector<double>(n, 0.0));
    std::vector<std::vector<double>> A(n, std::vector<double>(n, 0.0));
    if (rank == 0) {
        create_laplacian_matrix(A);
    }

    // Broadcast A to all processes
    for (int i = 0; i < n; ++i) {
        MPI_Bcast(A[i].data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    // Copy the relevant rows of A to local_A
    for (int i = 0; i < rows_per_proc; ++i) {
        local_A[i] = A[start_row + i];
    }

    // Initialize the vector b with the pattern 1, 1, 0, 0, 1, 1, ...
    std::vector<double> b(n, 0.0);
    if (rank == 0) {
        create_b_vector(b);
    }


    // Broadcast b to all processes
    MPI_Bcast(b.data(), n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Initialize solution vectors with a small non-zero initial guess
    std::vector<double> x_global(n, 0.0);  // Start with a small initial guess instead of zeros
    std::vector<double> x_local(rows_per_proc, 0.0);
    std::vector<double> new_x_local(rows_per_proc, 0.0);

    std::vector<double> errors;

    double start_time = MPI_Wtime();

    // Jacobi iteration
    for (int iteration = 0; iteration < max_iterations; ++iteration) {
        // Calculate the new values for the local part of the solution
        for (int i = 0; i < rows_per_proc; ++i) {
            int row_global_index = start_row + i;
            double row_sum = 0.0;

            // Compute the sum of A[i, j] * x_global[j] for j != row_global_index
            for (int j = 0; j < n; ++j) {
                if (j != row_global_index) {
                    row_sum += local_A[i][j] * x_global[j];
                }
            }

            // Calculate the new value using the Jacobi formula
            if (local_A[i][row_global_index] != 0) {  // Check to avoid division by zero
                new_x_local[i] = (b[row_global_index] - row_sum) / local_A[i][row_global_index];
            } else {
                std::cerr << "Division by zero detected at processor " << rank 
                          << " for row " << row_global_index << std::endl;
                MPI_Abort(MPI_COMM_WORLD, -1);
            }
        }

        // Gather all parts of the solution vector to form the global solution vector
        MPI_Allgather(new_x_local.data(), rows_per_proc, MPI_DOUBLE,
                      x_global.data(), rows_per_proc, MPI_DOUBLE, MPI_COMM_WORLD);

        // Calculate the maximum local error for convergence checking
        double local_error = 0.0;
        for (int i = 0; i < rows_per_proc; ++i) {
            local_error = std::max(local_error, std::fabs(new_x_local[i] - x_local[i]));
        }

        // Reduce the local errors to a global error (maximum norm across all processors)
        double global_error = 0.0;
        MPI_Allreduce(&local_error, &global_error, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

        // Update x_local to the new values for the next iteration
        x_local = new_x_local;

        // Check for convergence
        // if (global_error < tolerance) {
        //     if (rank == 0) {
        //         std::cout << "Converged in " << iteration + 1 << " iterations" << std::endl;
        //     }
        //     break;
        // }

        errors.push_back(compute_infinity_norm(A, x_global, b));

        if (iteration == 9) {
            break;
        }
    }

    double end_time = MPI_Wtime();
    double local_elapsed_time = end_time - start_time;
    double total_elapsed_time;
    double max_elapsed_time;
    MPI_Allreduce(&local_elapsed_time, &total_elapsed_time, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allreduce(&local_elapsed_time, &max_elapsed_time, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    // Final output from each processor
    for (int i = 0; i < rows_per_proc; ++i) {
        //std::cout << "Processor " << rank << ": x[" << start_row + i << "] = " << x_local[i] << std::endl;
    }

    if (rank == 0) {
	std::cout << "JACOBI\n";

	std::cout << "Total elapsed time (global) with " << size << " processors:\n";
	std::cout << total_elapsed_time;
	std::cout << std::endl;

	std::cout << "Max elapsed time (global) with " << size << " processors:\n";
	std::cout << max_elapsed_time;
	std::cout << std::endl << std::endl;
	
	/*
        std::cout << "Approximate final solution vector:\n";
        for (const auto& val : x_global) {
            std::cout << std::setprecision(4) << val << " "; }
        std::cout << std::endl << std::endl;

        std::cout << "Residual infinity norm for iterations 0..9:\n";
        for (const auto& error : errors) {
            std::cout << std::setprecision(4) << error << " ";
        }
        std::cout << std::endl << std::endl;
	*/
    }

    // Finalize MPI
    MPI_Finalize();
    return 0;
}
