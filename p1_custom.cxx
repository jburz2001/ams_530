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

    int index[size] = {3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48};
    int edges[48]   = {1,2,3, 0,6,10, 0,8,12, 0,4,14, 3,5,9, 4,6,12, 1,5,7, 6,8,13, 2,7,9, 4,8,15, 1,12,15, 5,10,12, 2,11,13, 7,12,14, 3,13,15, 9,10,14};
    MPI_Comm graph_comm;
    MPI_Graph_create(MPI_COMM_WORLD, 16, index, edges, false, &graph_comm);

    switch (rank) {
        case 0: {     
            std::vector<int> data;
            data.resize(vector_size, 42);

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_INT, 1, rank, graph_comm, &request_s0);
            MPI_Wait(&request_s0, MPI_STATUS_IGNORE);
            MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_INT, 2, rank, graph_comm, &request_s1);
            MPI_Wait(&request_s1, MPI_STATUS_IGNORE);
            MPI_Request request_s2;
            MPI_Isend(data.data(), vector_size, MPI_INT, 3, rank, graph_comm, &request_s2);
            MPI_Wait(&request_s2, MPI_STATUS_IGNORE);
            
            std::cout << "Process " << rank << " sent data to Processes 1, 2, and 3" << std::endl;
            break;
        }
        case 1: {
            std::vector<int> recv_data(vector_size);
            int src = 0;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;
	    
	    MPI_Request request_s0;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 6,  rank, graph_comm, &request_s0);
            MPI_Wait(&request_s0, MPI_STATUS_IGNORE);
	    MPI_Request request_s1;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 10, rank, graph_comm, &request_s1);
            MPI_Wait(&request_s1, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " sent data to Processes 6 and 10" << std::endl;
            break;
        }
        case 2: {
            std::vector<int> recv_data(vector_size);
            int src = 0;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 8,  rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 12, rank, graph_comm, &request_s1);

            std::cout << "Process " << rank << " sent data to Processes 8 and 12" << std::endl;
            break;
        }
        case 3: {
            std::vector<int> recv_data(vector_size);
            int src = 0;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 4,  rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 14, rank, graph_comm, &request_s1);

            std::cout << "Process " << rank << " sent data to Processes 4 and 14" << std::endl;
            break;
        }
        case 4: {
            std::vector<int> recv_data(vector_size);
            int src = 3;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 9, rank, graph_comm, &request_s0);

            std::cout << "Process " << rank << " sent data to Process 9" << std::endl;
            break;
        }
        case 5: {
            std::vector<int> recv_data(vector_size);
            int src = 6;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;
            break;
        }
        case 6: {
            std::vector<int> recv_data(vector_size);
            int src = 1;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 5, rank, graph_comm, &request_s0);

            std::cout << "Process " << rank << " sent data to Process 5" << std::endl;
            break;
        }
        case 7: {
            std::vector<int> recv_data(vector_size);
            int src = 8;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;
            break;
        }
        case 8: {
            std::vector<int> recv_data(vector_size);
            int src = 2;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 7, rank, graph_comm, &request_r0);

            std::cout << "Process " << rank << " sent data to Process 7" << std::endl;
            break;
        }
        case 9: {
            std::vector<int> recv_data(vector_size);
            int src = 4;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;
            break;
        }
        case 10: {
            std::vector<int> recv_data(vector_size);
            int src = 1;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 11, rank, graph_comm, &request_s0);

            std::cout << "Process " << rank << " sent data to Process 11" << std::endl;
            break;
        }
        case 11: {
            std::vector<int> recv_data(vector_size);
            int src = 10;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;
            break;
        }
        case 12: {
            std::vector<int> recv_data(vector_size);
            int src = 2;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 13, rank, graph_comm, &request_s0);
            MPI_Wait(&request_s0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " sent data to Process 13" << std::endl;
            break;
        }
        case 13: {
            std::vector<int> recv_data(vector_size);
            int src = 12;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;
            break;
        }
        case 14: {
            std::vector<int> recv_data(vector_size);
            int src = 3;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(recv_data.data(), vector_size, MPI_INT, 15, rank, graph_comm, &request_s0);
            MPI_Wait(&request_s0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " sent data to Process 15" << std::endl;
            break;
        }
        case 15: {
            std::vector<int> recv_data(vector_size);
            int src = 14;

            MPI_Request request_r0;
            MPI_Irecv(recv_data.data(), vector_size, MPI_INT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            std::cout << "Process " << rank << " received data from Process " << src << std::endl;
            break;
        }
        default: {
            std::cerr << "Unhandled rank: " << rank << std::endl;
            break;
        }
    }

    double elapsed_time = MPI_Wtime() - tbeg;
    double totalTime;
    MPI_Reduce(&elapsed_time, &totalTime, 1, MPI_DOUBLE, MPI_SUM, 0, graph_comm);
    if (rank == 0) {
        std::cout << "Total time spent broadcasting a vector with " << vector_size << " elements: " << totalTime << "s" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
