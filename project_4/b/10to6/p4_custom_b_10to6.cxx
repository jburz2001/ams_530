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

    int vector_size = 1000000;
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

    switch (rank) {
        case 24: {
            data.resize(vector_size, 42.0);

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 23, rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 1, rank, graph_comm, &request_s1);
            MPI_Request request_s2;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 28, rank, graph_comm, &request_s2);
            
            //std::cout << "Process " << rank << " sent data to Processes 1, 23, and 28" << std::endl;
            break;
        }
        case 23: {
            //std::vector<int> recv_data(vector_size);
            int src = 24;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;
	    
	        MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 0,  rank, graph_comm, &request_s0);
	        MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 22, rank, graph_comm, &request_s1);

            //std::cout << "Process " << rank << " sent data to Processes 0 and 22" << std::endl;
            break;
        }
        case 0: {
            //std::vector<int> recv_data(vector_size);
            int src = 23;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;
            break;
        }
        case 22: {
            //std::vector<int> recv_data(vector_size);
            int src = 23;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 21,  rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 31, rank, graph_comm, &request_s1);

            //std::cout << "Process " << rank << " sent data to Processes 21 and 31" << std::endl;
            break;
        }
        case 31: {
            //std::vector<int> recv_data(vector_size);
            int src = 22;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;
            break;
        }
        case 21: {
            //std::vector<int> recv_data(vector_size);
            int src = 22;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 20,  rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 20" << std::endl;

            break;
        }
        case 20: {
            //std::vector<int> recv_data(vector_size);
            int src = 21;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 19, rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 19" << std::endl;
            break;
        }
        case 19: {
            //std::vector<int> recv_data(vector_size);
            int src = 20;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 18, rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 30, rank, graph_comm, &request_s1);

            //std::cout << "Process " << rank << " sent data to Processes 18 and 30" << std::endl;

            break;
        }
        case 18: {
            //std::vector<int> recv_data(vector_size);
            int src = 19;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            break;
        }
        case 30: {
            //std::vector<int> recv_data(vector_size);
            int src = 19;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;
            break;
        }
        case 1: {
            //std::vector<int> recv_data(vector_size);
            int src = 24;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 2, rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 2" << std::endl;
            break;
        }
        case 2: {
            //std::vector<int> recv_data(vector_size);
            int src = 1;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 3, rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 25, rank, graph_comm, &request_s1);

            //std::cout << "Process " << rank << " sent data to Processes 3 and 25" << std::endl;

            break;
        }
        case 25: {
            //std::vector<int> recv_data(vector_size);
            int src = 2;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            break;
        }
        case 3: {
            //std::vector<int> recv_data(vector_size);
            int src = 2;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 4, rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 4" << std::endl;

            break;
        }
        case 4: {
            //std::vector<int> recv_data(vector_size);
            int src = 3;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 5, rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 5" << std::endl;
            break;
        }
        case 5: {
            //std::vector<int> recv_data(vector_size);
            int src = 4;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 6, rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 26, rank, graph_comm, &request_s1);

            //std::cout << "Process " << rank << " sent data to Processes 6 and 26" << std::endl;

            break;
        }
        case 6: {
            //std::vector<int> recv_data(vector_size);
            int src = 5;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            break;
        }
        case 26: {
            //std::vector<int> recv_data(vector_size);
            int src = 5;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            break;
        }
        case 28: {
            //std::vector<int> recv_data(vector_size);
            int src = 24;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 11, rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 13, rank, graph_comm, &request_s1);

            //std::cout << "Process " << rank << " sent data to Processes 11 and 13" << std::endl;

            break;
        }
        case 11: {
            //std::vector<int> recv_data(vector_size);
            int src = 28;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 10, rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 10" << std::endl;

            break;
        }
        case 10: {
            //std::vector<int> recv_data(vector_size);
            int src = 11;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 9, rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 27, rank, graph_comm, &request_s1);

            //std::cout << "Process " << rank << " sent data to Process 9 and 27" << std::endl;

            break;
        }
        case 9: {
            //std::vector<int> recv_data(vector_size);
            int src = 10;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 8, rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 8" << std::endl;

            break;
        }
        case 8: {
            //std::vector<int> recv_data(vector_size);
            int src = 9;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 7, rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 7" << std::endl;

            break;
        }
        case 7: {
            //std::vector<int> recv_data(vector_size);
            int src = 8;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            break;
        }
        case 27: {
            //std::vector<int> recv_data(vector_size);
            int src = 10;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 17, rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 17" << std::endl;

            break;
        }
        case 13: {
            //std::vector<int> recv_data(vector_size);
            int src = 28;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 14, rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 12, rank, graph_comm, &request_s1);

            //std::cout << "Process " << rank << " sent data to Processes 12 and 14" << std::endl;

            break;
        }
        case 12: {
            //std::vector<int> recv_data(vector_size);
            int src = 13;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            break;
        }
        case 14: {
            //std::vector<int> recv_data(vector_size);
            int src = 13;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 15, rank, graph_comm, &request_s0);
            MPI_Request request_s1;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 29, rank, graph_comm, &request_s1);

            //std::cout << "Process " << rank << " sent data to Processes 15 and 29" << std::endl;

            break;
        }
        case 29: {
            //std::vector<int> recv_data(vector_size);
            int src = 14;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            break;
        }
        case 15: {
            //std::vector<int> recv_data(vector_size);
            int src = 14;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 16, rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 16" << std::endl;

            break;
        }
        case 16: {
            //std::vector<int> recv_data(vector_size);
            int src = 15;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            MPI_Request request_s0;
            MPI_Isend(data.data(), vector_size, MPI_FLOAT, 17, rank, graph_comm, &request_s0);

            //std::cout << "Process " << rank << " sent data to Process 17" << std::endl;

            break;
        }
        case 17: {
            //std::vector<int> recv_data(vector_size);
            int src = 16;

            MPI_Request request_r0;
            MPI_Irecv(data.data(), vector_size, MPI_FLOAT, src, src, graph_comm, &request_r0);
            MPI_Wait(&request_r0, MPI_STATUS_IGNORE);

            //std::cout << "Process " << rank << " received data from Process " << src << std::endl;

            break;
        }
        default: {
            std::cerr << "Unhandled rank: " << rank << std::endl;
            break;
        }
    }

    MPI_Barrier(graph_comm);
    /* ******************** */


    if (rank == 24) {
        double elapsed_time = MPI_Wtime() - tbeg;
        std::cout << "CUSTOM: Total time spent broadcasting a vector with " << vector_size << " elements:\n" << elapsed_time << "s" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
