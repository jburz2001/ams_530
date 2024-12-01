#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int root = 31;

    if (size != 32) {
        if (rank == root) {
            std::cerr << "This program requires exactly 32 processes.\n";
        }
        MPI_Finalize();
        return -1;
    }

    int vector_size = 100000000;
    std::vector<double> data;

    if (rank == root) {
        data.resize(vector_size, 42.0);
    } else {
        data.resize(vector_size);
    }

    int index[size] = {3, 5, 8, 11, 13, 16, 19, 21, 24, 27, 29, 32, 35, 37, 40, 43, 45, 48, 51, 53, 56, 59, 61, 64, 67, 70, 73, 76, 79, 82, 85, 88};
    int edges[88]   = {1, 23, 24, 0, 2, 1, 3, 24, 2, 4, 25, 3, 5, 4, 6, 25, 5, 7, 26, 6, 8, 7, 9, 26, 8, 10, 27, 9, 11, 10, 12, 27, 11, 13, 28, 12, 14, 13, 15, 28, 14, 16, 29, 15, 17, 16, 18, 29, 17, 19, 30, 18, 20, 19, 21, 30, 20, 22, 31, 21, 23, 0, 22, 31, 0, 2, 27, 3, 5, 30, 6, 8, 29, 9, 11, 24, 12, 14, 31, 15, 17, 26, 18, 20, 25, 21, 23, 28};
    
    MPI_Comm graph_comm;
    MPI_Graph_create(MPI_COMM_WORLD, size, index, edges, false, &graph_comm);

    double tbeg;
    if (rank == root) {
        tbeg = MPI_Wtime();
    }

    /* ******************** */
    MPI_Barrier(graph_comm);

    MPI_Bcast(data.data(), vector_size, MPI_FLOAT, root, graph_comm);

    MPI_Barrier(graph_comm);
    /* ******************** */

    if (rank == root) {
        double elapsed_time = MPI_Wtime() - tbeg;
        std::cout << "BUILTIN: Total time spent broadcasting a vector with " << vector_size << " elements:\n" << elapsed_time << "s" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
