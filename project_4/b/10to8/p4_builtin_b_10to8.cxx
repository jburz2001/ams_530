#include <mpi.h>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size != 32) {
        if (rank == 0) {
            std::cerr << "This program requires exactly 32 processes.\n";
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

    int index[size] = {2, 5, 8, 10, 13, 16, 18, 21, 24, 26, 29, 32, 34, 37, 40, 42, 45, 48, 50, 53, 56, 58, 61, 64, 67, 70, 73, 76, 79, 82, 85, 88};
    int edges[88]   = {
        1, 23,      //0
        0, 2, 24,   //1
        1, 3, 25,   //2
        2, 4,       //3
        3, 5, 25,   //4
        4, 6, 26,   //5
        5, 7,       //6
        6, 8, 26,   //7
        7, 9, 27,   //8
        8, 10,      //9
        9, 11, 27,  //10
        10, 12, 28, //11
        11, 13,     //12
        12, 14, 28, //13
        13, 15, 29, //14
        14, 16,     //15
        15, 17, 29, //16
        16, 18, 30, //17
        17, 19,     //18
        18, 20, 30, //19
        19, 21, 31, //20
        20, 22,     //21
        21, 23, 31, //22
        0, 22, 24,  //23
        1, 23, 28,  //24
        2, 4, 29,   //25
        5, 7, 30,   //26
        8, 10, 31,  //27
        11, 13, 24, //28
        14, 16, 25, //29
        17, 19, 26, //30
        20, 22, 27  //31
        };
    
    MPI_Comm graph_comm;
    MPI_Graph_create(MPI_COMM_WORLD, size, index, edges, false, &graph_comm);

    double tbeg;
    if (rank == 24) {
        tbeg = MPI_Wtime();
    }

    /* ******************** */
    MPI_Barrier(graph_comm);

    MPI_Bcast(data.data(), vector_size, MPI_FLOAT, 24, graph_comm);

    MPI_Barrier(graph_comm);
    /* ******************** */

    if (rank == 24) {
        double elapsed_time = MPI_Wtime() - tbeg;
        std::cout << "BUILTIN: Total time spent broadcasting a vector with " << vector_size << " elements:\n" << elapsed_time << "s" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
