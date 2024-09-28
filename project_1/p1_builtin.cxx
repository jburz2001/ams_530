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
    std::vector<int> data;

    if (rank == 0) {
        data.resize(vector_size, 42);
    } else {
        data.resize(vector_size);
    }

    MPI_Bcast(data.data(), vector_size, MPI_INT, 0, MPI_COMM_WORLD);

    double totalTime = MPI_Wtime() - tbeg;
    if (rank == 0) {
        std::cout << "Total time spent broadcasting a vector with " << vector_size << " elements: " << totalTime << "s" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
