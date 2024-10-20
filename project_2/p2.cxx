#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int node_count, node_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &node_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &node_rank);

    const int M = 32;

    if (node_count != M) {
        if (node_rank == 0) {
            std::cerr << "This program must be run with " << M << " MPI processes." << std::endl;
        }
        MPI_Finalize();
        return -1;
    }

    std::vector<int> index_0 = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 90, 93, 96};
    std::vector<int> edges_0 = {
        1,  31, 15,
        0,  2,  14,
        1,  3,  13,
        2,  4,  12,
        3,  5,  27,
        4,  6,  26,
        5,  7,  25,
        6,  8,  24,
        7,  9,  23,
        8,  10, 22,
        9,  11, 21,
        10, 12, 20,
        11, 13, 3,
        12, 14, 2,
        13, 15, 1,
        14, 16, 0,
        15, 17, 31,
        16, 18, 30,
        17, 19, 29,
        18, 20, 28,
        19, 21, 11,
        20, 22, 10,
        21, 23, 9,
        22, 24, 8,
        23, 25, 7,
        24, 26, 6,
        25, 27, 5,
        26, 28, 4,
        27, 29, 19,
        28, 30, 18,
        29, 31, 17,
        0,  30, 16
    };

    std::vector<int> index_1 = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, 81, 84, 87, 90, 93, 96};
    std::vector<int> edges_1 = {
        1,  31, 16,
        0,  2,  23,
        1,  3,  28,
        2,  4,  19,
        3,  5,  10,
        4,  6,  14,
        5,  7,  29,
        6,  8,  17,
        7,  9,  24,
        8,  10, 31,
        9,  11, 4,
        10, 12, 27,
        11, 13, 18,
        12, 14, 22,
        13, 15, 5,
        14, 16, 25,
        15, 17, 0,
        16, 18, 7,
        17, 19, 12,
        18, 20, 3,
        19, 21, 26,
        20, 22, 30,
        21, 23, 13,
        22, 24, 1,
        23, 25, 8,
        24, 26, 15,
        25, 27, 20,
        26, 28, 11,
        27, 29, 2,
        28, 30, 6,
        29, 31, 21,
        0,  30, 9
    };

    srand(node_rank);
    
    for (std::string title : {"Bid32", "Opt32"}) {
        std::vector<int>* chosen_index = nullptr;
        std::vector<int>* chosen_edges = nullptr;
        if (title == "Bid32") {
            chosen_index = &index_0;
            chosen_edges = &edges_0;
        } else {
            chosen_index = &index_1;
            chosen_edges = &edges_1;
        }

        std::vector<int> Ns = {10, 20, 30, 40};
        for (size_t n = 0; n < Ns.size(); n++) {
	        int N = Ns[n];

            // Step 1: Each node generates N random floating-point numbers between node_rank and node_rank + 0.1
            std::vector<float> send_data(N);
            for (int i = 0; i < N; i++) {
                float random_fraction = static_cast<float>(rand()) / RAND_MAX * 0.1;
                send_data[i] = node_rank + random_fraction;
            }

            // Step 2: Prepare receive buffer to hold data from all nodes
            std::vector<float> recv_data(M * N);
	
	        // Step 3: Create network topology
            MPI_Comm graph_comm;
            MPI_Graph_create(MPI_COMM_WORLD, node_count, chosen_index->data(), chosen_edges->data(), 1, &graph_comm);

            // Step 4: Perform and time MPI_Allgather
            double start_time = MPI_Wtime();

            MPI_Allgather(send_data.data(), N, MPI_FLOAT,
                        recv_data.data(), N, MPI_FLOAT, graph_comm);

            double end_time = MPI_Wtime();
            double time_taken = end_time - start_time;

            // Step 5: Output the result (only from one node, say node 0, to avoid clutter)
            if (node_rank == 0) {
                std::cout << std::endl << "MPI_Allgather for network " << title << " with N = " << N << " completed in " << std::fixed << std::setprecision(6)
                        << time_taken << " seconds." << std::endl << std::endl;

                // Step 6: Print the sent data from all nodes (for debugging)
                for (int i = 0; i < M; ++i) {
                    //UNCOMMENT ME TO PRINT OUT DATA SENT FROM EACH NODE
                    //std::cout << "Data (node " << i << "): ";

                    for (int j = 0; j < N; ++j) {
                        //UNCOMMENT ME TOO!
                        //std::cout << std::setprecision(2) << recv_data[i * N + j] << " ";
                    }
                    //UNCOMMENT ME TOO!
                    //std::cout << std::endl;
                }
            }
        }
    }
	
    // Step 7: End MPI procedure
    MPI_Finalize();
    return 0;
}










