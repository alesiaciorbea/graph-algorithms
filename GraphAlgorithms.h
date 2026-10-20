#pragma once

#include "Graph.h"
#include <vector>
#include <utility>
#include <stdexcept>

const int GRAPH_INF = 1000000000;

// LAB 5: HAMILTONIAN CYCLE 

// LAB 5 - Problem 4: Find a Hamiltonian Cycle using Backtracking
// Returns: A vector containing the sequence of vertices in the cycle.
// If no cycle exists, returns an empty vector.
std::vector<int> find_hamiltonian_cycle(const DirectedGraph& graph);

// LAB 4

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

inline void delete_tree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    delete_tree(root->left); 
    delete_tree(root->right); 
    delete root;           
}

// LAB 4 - Problem 4: Highest cost path in a DAG using Kahn's Algorithm
// Returns: The maximum cost path from start to target.
// Throws: std::runtime_error if the graph is not a DAG.
int highest_cost_path(const DirectedGraph& graph, int start, int target);

// LAB 4 - Bonus 1B: Reconstruct a tree from Pre-order and In-order traversals
// Returns: The root of the reconstructed tree.
TreeNode* reconstruct_tree(const std::vector<int>& preorder, const std::vector<int>& inorder);

// LAB 4 - Bonus 2B: Distinct paths in a DAG
// Returns: The number of distinct paths from start to target.
int count_distinct_paths(const DirectedGraph& graph, int start, int target);

// LAB 4 - Bonus 3B: Distinct lowest cost paths in a DAG
// Returns: The number of distinct minimum cost paths from start to target.
int count_lowest_cost_paths(const DirectedGraph& graph, int start, int target);


// (LAB 2 & LAB 3)

// Problem 5: Shortest path using Bellman-Ford
// Returns: A pair containing <minimum_cost, path_vector>
// Throws: std::runtime_error if a negative cycle is accessible from start
std::pair<int, std::vector<int>> bellman_ford_shortest_path(const DirectedGraph& graph, int start, int target);

// Bonus 1B: Number of distinct minimum cost walks
// Returns: A pair containing <minimum_cost, number_of_distinct_walks>
// Throws: std::runtime_error if a negative cycle is accessible from start
std::pair<int, int> count_min_cost_walks(const DirectedGraph& graph, int start, int target);

// Bonus 2B: Number of distinct walks in a DAG (Directed Acyclic Graph)
// Returns: The total number of distinct walks from start to target
// Throws: std::runtime_error if the graph is not a DAG (contains cycles)
int count_paths_in_dag(const DirectedGraph& graph, int start, int target);

// Bonus 3B: Bridge and Torch Problem
// Returns: The minimum time required for everyone to cross
int solve_bridge_and_torch(const std::vector<int>& times);

// Finds the lowest length path using a forward Breadth-First Search
void lowest_length_path_forward(const DirectedGraph& graph, int start, int target);

// Bonus 1B: Strongly Connected Components (Kosaraju's Algorithm)
void strongly_connected_components(const DirectedGraph& graph);

// Bonus 2B: Biconnected Components
// Note: To use this, the DirectedGraph must have bidirectional edges (if u->v exists, v->u must exist).
void find_biconnected_components(const DirectedGraph& graph);

// Bonus 3B: Wolf, Goat, Cabbage Problem
// Uses the lowest length path logic on an implicitly generated state graph.
void solve_wolf_goat_cabbage();