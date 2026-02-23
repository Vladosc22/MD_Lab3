📖 Project Overview

This project implements two classical shortest path algorithms in C:

🔹 Bellman-Ford (Ford Algorithm) – Computes shortest paths from a single source node to all other nodes in a directed weighted graph (supports negative weights, no negative cycles).

🔹 Bellman-Calaba Algorithm – Computes the shortest paths to a specific destination node from all other nodes (reverse approach using adjacency matrix).

The program allows dynamic input of nodes and edges, builds the graph internally, and displays distances and paths.

🧠 Algorithms Implemented
1️⃣ Bellman-Ford Algorithm

Works with directed graphs

Supports negative edge weights

Time complexity: O(V × E)

Computes:

Minimum distances from a chosen start node

The actual shortest path using a predecessor vector

2️⃣ Bellman-Calaba Algorithm

Computes minimum cost towards a chosen destination node

Uses adjacency matrix representation

Iteratively relaxes nodes until no updates occur

Time complexity: O(V³) (matrix-based approach)

🏗️ Project Structure
.
├── main.c
└── README.md

Main components:

Edge structure for edge representation

Adjacency matrix M

Edge list edges[]

Functions:

initialize_matrix()

read_edges()

ford_algorithm()

bellman_calaba()

afisare_drum()
📊 Program Output
Shortest distance from start node to all other nodes
Full path reconstruction (Ford)
Minimum cost to reach destination node (Bellman-Calaba)
