import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

def convert_adjacency_list_to_matrices(adj):
    G = nx.from_dict_of_lists(adj)
    adj_matrix  = nx.adjacency_matrix(G).todense()
    dist_matrix = nx.floyd_warshall_numpy(G)

    return adj_matrix, dist_matrix

# Adjacency lists
adj_a = {
    0 : [1, 23, 24],
    1: [0, 2],
    2: [1, 3, 24],
    3: [2, 4, 25],
    4: [3, 5],
    5: [4, 6, 25],
    6: [5, 7, 26],
    7: [6, 8],
    8: [7, 9, 26],
    9: [8, 10, 27],
    10: [9, 11],
    11: [10, 12, 27],
    12: [11, 13, 28],
    13: [12, 14],
    14: [13, 15, 28],
    15: [14, 16, 29],
    16: [15, 17],
    17: [16, 18, 29],
    18: [17, 19, 30],
    19: [18, 20],
    20: [19, 21, 30],
    21: [20, 22, 31],
    22: [21, 23],
    23: [0, 22, 31],
    24: [0, 2, 27],
    25: [3, 5, 30],
    26: [6, 8, 29],
    27: [9, 11, 24],
    28: [12, 14, 31],
    29: [15, 17, 26],
    30: [18, 20, 25],
    31: [21, 23, 28],
}

adj_b = {
    0: [1, 23],
    1: [0, 2, 24],
    2: [1, 3, 25],
    3: [2, 4],
    4: [3, 5, 25],
    5: [4, 6, 26],
    6: [5, 7],
    7: [6, 8, 26],
    8: [7, 9, 27],
    9: [8, 10],
    10: [9, 11, 27],
    11: [10, 12, 28],
    12: [11, 13],
    13: [12, 14, 28],
    14: [13, 15, 29],
    15: [14, 16],
    16: [15, 17, 29],
    17: [16, 18, 30],
    18: [17, 19],
    19: [18, 20, 30],
    20: [19, 21, 31],
    21: [20, 22],
    22: [21, 23, 31],
    23: [0, 22, 24],
    24: [1, 23, 28],
    25: [2, 4, 29],
    26: [5, 7, 30],
    27: [8, 10, 31],
    28: [11, 13, 24],
    29: [14, 16, 25],
    30: [17, 19, 26],
    31: [20, 22, 27],
}

# Create the matrices
adj_matrix_a, dist_matrix_a = convert_adjacency_list_to_matrices(adj_a)
adj_matrix_b, dist_matrix_b = convert_adjacency_list_to_matrices(adj_b)

# Plot adjacency matrix
plt.figure(figsize=(8, 8))
plt.imshow(adj_matrix_a, cmap='viridis', interpolation='none')
plt.colorbar(label='Edge Presence')
plt.title('Adjacency Matrix: 4x8a')
plt.xlabel('Nodes')
plt.ylabel('Nodes')
plt.grid(False)
plt.show()

plt.figure(figsize=(8, 8))
plt.imshow(adj_matrix_b, cmap='viridis', interpolation='none')
plt.colorbar(label='Edge Presence')
plt.title('Adjacency Matrix: 4x8b')
plt.xlabel('Nodes')
plt.ylabel('Nodes')
plt.grid(False)
plt.show()

# Plot distance matrix
plt.figure(figsize=(8, 8))
plt.imshow(dist_matrix_a, cmap='plasma', interpolation='none')
plt.colorbar(label='Distance (Hops)')
plt.title('Distance Matrix: 4x8a')
plt.xlabel('Nodes')
plt.ylabel('Nodes')
plt.grid(False)
plt.show()

plt.figure(figsize=(8, 8))
plt.imshow(dist_matrix_b, cmap='plasma', interpolation='none')
plt.colorbar(label='Distance (Hops)')
plt.title('Distance Matrix: 4x8b')
plt.xlabel('Nodes')
plt.ylabel('Nodes')
plt.grid(False)
plt.show()
