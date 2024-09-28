#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    double tbeg = MPI_Wtime();

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 16) {
        if (rank == 0) {
            std::cerr << "This program requires exactly 16 processes.\n";
        }
        MPI_Finalize();
        return -1;
    }

    int vector_size = 100000000;
    std::vector<double> data;

    if (rank == 0) {
        data.resize(vector_size, 42.0);
    } else {
        data.resize(vector_size);
    }

    int index[size] = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48};
    int edges[48]   = {1,2,3, 0,6,10, 0,8,12, 0,4,14, 3,5,9, 4,6,12, 1,5,7, 6,8,13, 2,7,9, 4,8,15, 1,12,15, 5,10,12, 2,11,13, 7,12,14, 3,13,15, 9,10,14};
    MPI_Comm graph_comm;
    MPI_Graph_create(MPI_COMM_WORLD, 16, index, edges, false, &graph_comm);


    MPI_Bcast(data.data(), vector_size, MPI_FLOAT, 0, graph_comm);

    double totalTime = MPI_Wtime() - tbeg;
    if (rank == 0) {
        std::cout << "Total time spent broadcasting a vector with " << vector_size << " elements: " << totalTime << "s" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
