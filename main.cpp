#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <algorithm>
#include "Graph.h"
#include "GraphAlgorithms.h"

// helpers from lab 1
DirectedGraph read_graph_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open file.");

    int n, m;
    if (!(file >> n >> m)) throw std::runtime_error("File format error.");

    DirectedGraph graph(n);

    int source, target, cost;
    for (int i = 0; i < m; ++i) {
        if (file >> source >> target >> cost) {
            graph.add_edge(source, target, cost);
        }
    }
    file.close();
    return graph;
}

DirectedGraph generate_random_graph(int n, int m) {
    if (m > n * (n - 1)) throw std::invalid_argument("Too many edges for a simple graph.");

    DirectedGraph graph(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> vertex_dist(0, n - 1);
    std::uniform_int_distribution<> cost_dist(1, 100);

    int edges_added = 0;
    while (edges_added < m) {
        int source = vertex_dist(gen);
        int target = vertex_dist(gen);
        if (source != target && !graph.is_edge(source, target)) {
            graph.add_edge(source, target, cost_dist(gen));
            edges_added++;
        }
    }
    return graph;
}

void write_graph_to_file(const DirectedGraph& graph, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Cannot open file for writing.");

    int total_vertices = graph.get_number_of_vertices();
    int total_edges = graph.get_number_of_edges();

    file << total_vertices << " " << total_edges << "\n";

    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        int source = it->first;
        for (auto n_it = graph.begin_outbound(source); n_it != graph.end_outbound(source); ++n_it) {
            int target = *n_it;
            file << source << " " << target << " " << graph.get_edge_cost(source, target) << "\n";
        }
    }
    file.close();
}

void display_graph_contents(const DirectedGraph& graph) {
    std::cout << "\n--- GRAPH CONTENTS ---\n";
    std::cout << "Vertices: ";
    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        std::cout << it->first << " ";
    }
    std::cout << "\nEdges:\n";
    bool has_edges = false;
    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        int source = it->first;
        for (auto n_it = graph.begin_outbound(source); n_it != graph.end_outbound(source); ++n_it) {
            int target = *n_it;
            std::cout << "    " << source << " -> " << target << " (cost: " << graph.get_edge_cost(source, target) << ")\n";
            has_edges = true;
        }
    }
    if (!has_edges) std::cout << "    No edges in this graph.\n";
    std::cout << "----------------------\n";
}

// Funcție helper pentru LAB 4 (Bonus 1B - Arbore)
void print_postorder(TreeNode* node) {
    if (node == nullptr) return;
    print_postorder(node->left);
    print_postorder(node->right);
    std::cout << node->val << " ";
}

void print_menu() {
    std::cout << "\n=================================================================\n";
    std::cout << "1.  Get the number of vertices\n";
    std::cout << "2.  Parse the set of vertices\n";
    std::cout << "3.  Check if there is an edge between two vertices\n";
    std::cout << "4.  Get the in-degree and out-degree of a vertex\n";
    std::cout << "5.  Parse the set of outbound edges\n";
    std::cout << "6.  Parse the set of inbound edges\n";
    std::cout << "7.  Retrieve or modify edge cost\n";
    std::cout << "8.  Modify graph (Add/Remove V or E)\n";
    std::cout << "9.  Make an exact copy of the graph\n";
    std::cout << "10. Read graph from file\n";
    std::cout << "11. Write graph to file\n";
    std::cout << "12. Create a random graph\n";
    std::cout << "13. Display entire graph contents\n";
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << "14. [LAB 2] Lowest length path (Forward BFS)\n";
    std::cout << "15. [LAB 2 - BONUS 1B] Strongly Connected Components (Kosaraju)\n";
    std::cout << "16. [LAB 2 - BONUS 2B] Biconnected Components\n";
    std::cout << "17. [LAB 2 - BONUS 3B] Wolf, Goat, Cabbage Problem\n";
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << "18. [LAB 3 - Prob 5] Lowest cost walk (Bellman-Ford)\n";
    std::cout << "19. [LAB 3 - BONUS 1B] Count distinct min-cost walks\n";
    std::cout << "20. [LAB 3 - BONUS 2B] Distinct walks in a DAG\n";
    std::cout << "21. [LAB 3 - BONUS 3B] Bridge and Torch problem\n";
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << "22. [LAB 4 - Prob 4] Highest cost path in DAG (Kahn)\n";
    std::cout << "23. [LAB 4 - BONUS 1B] Reconstruct Tree (Pre/In-order)\n";
    std::cout << "24. [LAB 4 - BONUS 2B] Distinct paths in DAG (Kahn)\n";
    std::cout << "25. [LAB 4 - BONUS 3B] Distinct lowest cost paths in DAG\n";
    std::cout << "-----------------------------------------------------------------\n";
    std::cout << "26. [LAB 5 - Prob 4] Find a Hamiltonian Cycle using Backtracking\n";
    std::cout << "0.  Exit\n";
    std::cout << "=================================================================\n";
    std::cout << "Choose: ";
}

int main() {
    DirectedGraph graph;
    int choice;

    while (true) {
        print_menu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        try {
            switch (choice) {
            case 1: std::cout << "Vertices: " << graph.get_number_of_vertices() << "\n"; break;
            case 2:
                for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) std::cout << it->first << " ";
                std::cout << "\n"; break;
            case 3: {
                int s, t; std::cout << "Source Target: "; std::cin >> s >> t;
                std::cout << (graph.is_edge(s, t) ? "Edge exists.\n" : "No edge.\n");
                break;
            }
            case 4: {
                int v; std::cout << "Vertex: "; std::cin >> v;
                std::cout << "In: " << graph.get_in_degree(v) << " | Out: " << graph.get_out_degree(v) << "\n";
                break;
            }
            case 5: {
                int v; std::cout << "Vertex: "; std::cin >> v;
                for (auto it = graph.begin_outbound(v); it != graph.end_outbound(v); ++it) std::cout << *it << " ";
                std::cout << "\n"; break;
            }
            case 6: {
                int v; std::cout << "Vertex: "; std::cin >> v;
                for (auto it = graph.begin_inbound(v); it != graph.end_inbound(v); ++it) std::cout << *it << " ";
                std::cout << "\n"; break;
            }
            case 7: {
                int s, t; char action; std::cout << "Source Target: "; std::cin >> s >> t;
                std::cout << "(R)etrieve or (M)odify: "; std::cin >> action;
                if (action == 'R' || action == 'r') std::cout << "Cost: " << graph.get_edge_cost(s, t) << "\n";
                else { int c; std::cin >> c; graph.set_edge_cost(s, t, c); }
                break;
            }
            case 8: {
                char mod; std::cout << "a. Add V | b. Rem V | c. Add E | d. Rem E: "; std::cin >> mod;
                if (mod == 'a') { int v; std::cin >> v; graph.add_vertex(v); }
                else if (mod == 'b') { int v; std::cin >> v; graph.remove_vertex(v); }
                else if (mod == 'c') { int s, t, c; std::cin >> s >> t >> c; graph.add_edge(s, t, c); }
                else if (mod == 'd') { int s, t; std::cin >> s >> t; graph.remove_edge(s, t); }
                break;
            }
            case 9: {
                DirectedGraph copy = graph.copy();
                std::cout << "Copy created successfully.\n";
                break;
            }
            case 10: {
                std::string f; std::cout << "Filename: "; std::cin >> f;
                graph = read_graph_from_file(f);
                break;
            }
            case 11: {
                std::string f; std::cout << "Filename: "; std::cin >> f;
                write_graph_to_file(graph, f);
                break;
            }
            case 12: {
                int n, m; std::cout << "N M: "; std::cin >> n >> m;
                graph = generate_random_graph(n, m);
                break;
            }
            case 13: display_graph_contents(graph); break;

                // --- LAB 2 ---
            case 14: {
                int s, t;
                std::cout << "Start vertex: "; std::cin >> s;
                std::cout << "Target vertex: "; std::cin >> t;
                lowest_length_path_forward(graph, s, t);
                break;
            }
            case 15: strongly_connected_components(graph); break;
            case 16: find_biconnected_components(graph); break;
            case 17: solve_wolf_goat_cabbage(); break;

                // --- LAB 3 ---
            case 18: {
                int s, t;
                std::cout << "Start vertex: "; std::cin >> s;
                std::cout << "Target vertex: "; std::cin >> t;
                auto result = bellman_ford_shortest_path(graph, s, t);
                if (result.first == GRAPH_INF) {
                    std::cout << "No path found between " << s << " and " << t << ".\n";
                }
                else {
                    std::cout << "Minimum cost: " << result.first << "\nPath: ";
                    for (int v : result.second) std::cout << v << " ";
                    std::cout << "\n";
                }
                break;
            }
            case 19: {
                int s, t;
                std::cout << "Start vertex: "; std::cin >> s;
                std::cout << "Target vertex: "; std::cin >> t;
                auto result = count_min_cost_walks(graph, s, t);
                if (result.first == GRAPH_INF) {
                    std::cout << "No path found between " << s << " and " << t << ".\n";
                }
                else {
                    std::cout << "Minimum cost: " << result.first << "\n";
                    std::cout << "Number of distinct minimum cost walks: " << result.second << "\n";
                }
                break;
            }
            case 20: {
                int s, t;
                std::cout << "Start vertex: "; std::cin >> s;
                std::cout << "Target vertex: "; std::cin >> t;
                int num_paths = count_paths_in_dag(graph, s, t);
                std::cout << "Total distinct walks in DAG from " << s << " to " << t << " is: " << num_paths << "\n";
                break;
            }
            case 21: {
                int n;
                std::cout << "How many people need to cross the bridge? (e.g., 4): ";
                std::cin >> n;
                if (n <= 0) {
                    std::cout << "Invalid number of people.\n";
                    break;
                }
                std::vector<int> times(n);
                std::cout << "Enter the times for each person (e.g., 1 2 5 10): ";
                for (int i = 0; i < n; ++i) {
                    std::cin >> times[i];
                }
                int min_time = solve_bridge_and_torch(times);
                std::cout << "Minimum time required for everyone to cross is: " << min_time << "\n";
                break;
            }

                   // --- LAB 4 ---
            case 22: {
                int s, t;
                std::cout << "Start vertex: "; std::cin >> s;
                std::cout << "Target vertex: "; std::cin >> t;
                int cost = highest_cost_path(graph, s, t);
                if (cost == -999999) {
                    std::cout << "Target " << t << " is unreachable from " << s << ".\n";
                }
                else {
                    std::cout << "Highest cost path from " << s << " to " << t << " is: " << cost << "\n";
                }
                break;
            }
            case 23: {
                int n;
                std::cout << "How many nodes in the tree? "; std::cin >> n;
                if (n <= 0) break;

                std::vector<int> preorder(n), inorder(n);
                std::cout << "Enter the " << n << " numbers of the Pre-Order sequence (separated by spaces):\n";
                for (int i = 0; i < n; ++i) std::cin >> preorder[i];

                std::cout << "Enter the " << n << " numbers of the In-Order sequence (separated by spaces):\n";
                for (int i = 0; i < n; ++i) std::cin >> inorder[i];

                TreeNode* root = reconstruct_tree(preorder, inorder);
                std::cout << "Tree successfully reconstructed!\n";
                std::cout << "Proof (Post-Order Traversal): ";
                print_postorder(root);
                std::cout << "\n";
                delete_tree(root);
                break;
            }
            case 24: {
                int s, t;
                std::cout << "Start vertex: "; std::cin >> s;
                std::cout << "Target vertex: "; std::cin >> t;
                int paths = count_distinct_paths(graph, s, t);
                std::cout << "Total distinct paths from " << s << " to " << t << " is: " << paths << "\n";
                break;
            }
            case 25: {
                int s, t;
                std::cout << "Start vertex: "; std::cin >> s;
                std::cout << "Target vertex: "; std::cin >> t;
                int min_paths = count_lowest_cost_paths(graph, s, t);
                std::cout << "Total distinct lowest cost paths from " << s << " to " << t << " is: " << min_paths << "\n";
                break;
            }
            case 26: {
                std::cout << "Searching for a Hamiltonian Cycle...\n";
                try {
                    std::vector<int> cycle = find_hamiltonian_cycle(graph);
                    if (cycle.empty()) {
                        std::cout << "No Hamiltonian Cycle exists in this graph.\n";
                    }
                    else {
                        std::cout << "Hamiltonian Cycle found!\nPath: ";
                        for (size_t i = 0; i < cycle.size(); ++i) {
                            std::cout << cycle[i] << (i < cycle.size() - 1 ? " -> " : "");
                        }
                        std::cout << "\n";
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Error: " << e.what() << "\n";
                }
                break;
            }
            case 0: return 0;
            default: std::cout << "Invalid choice.\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
