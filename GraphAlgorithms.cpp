#include "GraphAlgorithms.h"
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>

// LAB 5: HAMILTONIAN CYCLE

// Recursive helper function for Backtracking
bool hamiltonian_backtracking(const DirectedGraph& graph, int current_node,
    std::vector<int>& path, std::vector<bool>& visited) {

    if (path.size() == graph.get_number_of_vertices()) {
        int start_node = path[0];

        if (graph.is_edge(current_node, start_node)) {
            path.push_back(start_node);
            return true;
        }
        return false;
    }

    for (auto it = graph.begin_outbound(current_node); it != graph.end_outbound(current_node); ++it) {
        int neighbor = *it;

        if (!visited[neighbor]) {
            visited[neighbor] = true;
            path.push_back(neighbor);

            if (hamiltonian_backtracking(graph, neighbor, path, visited)) {
                return true;
            }

            // backtrack
            visited[neighbor] = false;
            path.pop_back();
        }
    }

    return false;
}

std::vector<int> find_hamiltonian_cycle(const DirectedGraph& graph) {
    if (graph.get_number_of_vertices() == 0) return {};

    std::vector<int> path;
    std::vector<bool> visited(graph.get_number_of_vertices(), false);

    int start_node = graph.begin_vertices()->first;

    path.push_back(start_node);
    visited[start_node] = true;

    if (hamiltonian_backtracking(graph, start_node, path, visited)) {
        return path;
    }

    return {};
}


//PRACTICAL WORK 4
// Problem 4: Highest cost path using Kahn's Algorithm
int highest_cost_path(const DirectedGraph& graph, int start, int target) {
    std::unordered_map<int, int> in_degree;
    std::unordered_map<int, int> dp_cost; // max cost to reach each vertex

    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        in_degree[it->first] = graph.get_in_degree(it->first);
        dp_cost[it->first] = -999999; // -INF (unreachable)
    }

    std::queue<int> q;
    for (auto const& pair : in_degree) {
        if (pair.second == 0) q.push(pair.first);
    }

    dp_cost[start] = 0;
    int visited_count = 0;

    // Kahn's Algorithm (Topological Sort)
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        visited_count++;

        for (auto nit = graph.begin_outbound(u); nit != graph.end_outbound(u); ++nit) {
            int v = *nit;
            int cost = graph.get_edge_cost(u, v);

            if (dp_cost[u] != -999999) {
                dp_cost[v] = std::max(dp_cost[v], dp_cost[u] + cost);
            }

            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    // Verify DAG condition
    if (visited_count != graph.get_number_of_vertices()) {
        throw std::runtime_error("Graph is not a DAG (contains cycles)!");
    }

    return dp_cost[target];
}

// Bonus 2B: Number of distinct paths using Kahn's Algorithm
int count_distinct_paths(const DirectedGraph& graph, int start, int target) {
    std::unordered_map<int, int> in_degree;
    std::unordered_map<int, int> dp_paths; // number of paths to each vertex

    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        in_degree[it->first] = graph.get_in_degree(it->first);
        dp_paths[it->first] = 0;
    }

    std::queue<int> q;
    for (auto const& pair : in_degree) {
        if (pair.second == 0) q.push(pair.first);
    }

    dp_paths[start] = 1;
    int visited_count = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        visited_count++;

        for (auto nit = graph.begin_outbound(u); nit != graph.end_outbound(u); ++nit) {
            int v = *nit;

            if (dp_paths[u] > 0) {
                dp_paths[v] += dp_paths[u];
            }

            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    if (visited_count != graph.get_number_of_vertices()) {
        throw std::runtime_error("Graph is not a DAG (contains cycles)!");
    }

    return dp_paths[target];
}

// Bonus 3B: Number of distinct lowest cost paths
int count_lowest_cost_paths(const DirectedGraph& graph, int start, int target) {
    std::unordered_map<int, int> in_degree;
    std::unordered_map<int, int> dp_cost;  // min cost to each vertex
    std::unordered_map<int, int> dp_paths; // number of paths with the min cost

    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        in_degree[it->first] = graph.get_in_degree(it->first);
        dp_cost[it->first] = 999999; // +INF (unreachable)
        dp_paths[it->first] = 0;
    }

    std::queue<int> q;
    for (auto const& pair : in_degree) {
        if (pair.second == 0) q.push(pair.first);
    }

    dp_cost[start] = 0;
    dp_paths[start] = 1;
    int visited_count = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        visited_count++;

        for (auto nit = graph.begin_outbound(u); nit != graph.end_outbound(u); ++nit) {
            int v = *nit;
            int cost = graph.get_edge_cost(u, v);

            if (dp_cost[u] != 999999) {
                if (dp_cost[u] + cost < dp_cost[v]) {
                    // cheaper path: replace the cost and the count
                    dp_cost[v] = dp_cost[u] + cost;
                    dp_paths[v] = dp_paths[u];
                }
                else if (dp_cost[u] + cost == dp_cost[v]) {
                    // same min cost: add to the count
                    dp_paths[v] += dp_paths[u];
                }
            }

            in_degree[v]--;
            if (in_degree[v] == 0) q.push(v);
        }
    }

    if (visited_count != graph.get_number_of_vertices()) {
        throw std::runtime_error("Graph is not a DAG!");
    }

    return dp_paths[target];
}

TreeNode* build_tree_helper(const std::vector<int>& preorder, const std::vector<int>& inorder,
    int& pre_idx, int in_start, int in_end) {
    if (in_start > in_end) {
        return nullptr;
    }

    // The current root is always the next element in preorder traversal
    int root_val = preorder[pre_idx++];
    TreeNode* root = new TreeNode(root_val);

    int in_idx = in_start;
    while (in_idx <= in_end && inorder[in_idx] != root_val) {
        in_idx++;
    }

    // Elements left of in_idx in 'inorder' form the left subtree
    root->left = build_tree_helper(preorder, inorder, pre_idx, in_start, in_idx - 1);

    // Elements right of in_idx in 'inorder' form the right subtree
    root->right = build_tree_helper(preorder, inorder, pre_idx, in_idx + 1, in_end);

    return root;
}

// Bonus 1B: Reconstruct tree from Pre-order and In-order traversals
TreeNode* reconstruct_tree(const std::vector<int>& preorder, const std::vector<int>& inorder) {
    int pre_idx = 0;
    return build_tree_helper(preorder, inorder, pre_idx, 0, inorder.size() - 1);
}

// PRACTICAL WORK 3
// PROBLEM 5: Bellman-Ford
std::pair<int, std::vector<int>> bellman_ford_shortest_path(const DirectedGraph& graph, int start, int target) {
    std::unordered_map<int, int> dist;
    std::unordered_map<int, int> prev;
    int V = graph.get_number_of_vertices();

    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        dist[it->first] = GRAPH_INF;
    }
    dist[start] = 0;

    // Relax all edges V-1 times
    for (int i = 1; i < V; ++i) {
        bool changed = false;
        for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
            int u = it->first;
            if (dist[u] == GRAPH_INF) continue;

            for (auto nit = graph.begin_outbound(u); nit != graph.end_outbound(u); ++nit) {
                int v = *nit;
                int cost = graph.get_edge_cost(u, v);
                if (dist[u] + cost < dist[v]) {
                    dist[v] = dist[u] + cost;
                    prev[v] = u;
                    changed = true;
                }
            }
        }
        if (!changed) break; // stop early if distances stabilized
    }

    // Check for negative-weight cycles
    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        int u = it->first;
        if (dist[u] == GRAPH_INF) continue;
        for (auto nit = graph.begin_outbound(u); nit != graph.end_outbound(u); ++nit) {
            int v = *nit;
            if (dist[u] + graph.get_edge_cost(u, v) < dist[v]) {
                throw std::runtime_error("Graph contains a negative-cost cycle accessible from the start vertex!");
            }
        }
    }

    std::vector<int> path;
    if (dist[target] == GRAPH_INF) {
        return { GRAPH_INF, path }; // No path exists
    }

    for (int curr = target; curr != start; curr = prev[curr]) {
        path.push_back(curr);
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());

    return { dist[target], path };
}

int dfs_count_paths(const DirectedGraph& graph, int u, int target, const std::unordered_map<int, int>& dist, std::unordered_map<int, int>& memo) {
    if (u == target) return 1;

    // memoization
    if (memo.find(u) != memo.end()) return memo[u];

    int ways = 0;
    for (auto nit = graph.begin_outbound(u); nit != graph.end_outbound(u); ++nit) {
        int v = *nit;
        int cost = graph.get_edge_cost(u, v);

        // We only traverse the edge if it belongs to a minimum cost path
        if (dist.at(u) + cost == dist.at(v)) {
            ways += dfs_count_paths(graph, v, target, dist, memo);
        }
    }

    return memo[u] = ways;
}

// BONUS 1B: Distinct min-cost walks
std::pair<int, int> count_min_cost_walks(const DirectedGraph& graph, int start, int target) {
    std::unordered_map<int, int> dist;
    int V = graph.get_number_of_vertices();

    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        dist[it->first] = GRAPH_INF;
    }
    dist[start] = 0;

    for (int i = 1; i < V; ++i) {
        bool changed = false;
        for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
            int u = it->first;
            if (dist[u] == GRAPH_INF) continue;

            for (auto nit = graph.begin_outbound(u); nit != graph.end_outbound(u); ++nit) {
                int v = *nit;
                int cost = graph.get_edge_cost(u, v);

                if (dist[u] + cost < dist[v]) {
                    dist[v] = dist[u] + cost;
                    changed = true;
                }
            }
        }
        if (!changed) break;
    }

    // Check for negative-weight cycles
    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        int u = it->first;
        if (dist[u] == GRAPH_INF) continue;
        for (auto nit = graph.begin_outbound(u); nit != graph.end_outbound(u); ++nit) {
            int v = *nit;
            if (dist[u] + graph.get_edge_cost(u, v) < dist[v]) {
                throw std::runtime_error("Graph contains a negative-cost cycle accessible from the start vertex!");
            }
        }
    }

    if (dist[target] == GRAPH_INF) {
        return { GRAPH_INF, 0 };
    }

    std::unordered_map<int, int> memo;
    int total_paths = dfs_count_paths(graph, start, target, dist, memo);

    return { dist[target], total_paths };
}

// BONUS 2B: Distinct walks in a DAG (Topological Sort + DP)
int count_paths_in_dag(const DirectedGraph& graph, int start, int target) {
    std::unordered_map<int, int> in_degree;
    std::unordered_map<int, int> dp;

    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        in_degree[it->first] = graph.get_in_degree(it->first);
        dp[it->first] = 0;
    }

    std::queue<int> q;
    for (auto const& pair : in_degree) {
        if (pair.second == 0) q.push(pair.first);
    }

    dp[start] = 1;
    int visited_count = 0;

    // Kahn's Algorithm for Topological Sorting
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        visited_count++;

        for (auto nit = graph.begin_outbound(u); nit != graph.end_outbound(u); ++nit) {
            int v = *nit;
            if (dp[u] > 0) {
                dp[v] += dp[u];
            }

            in_degree[v]--;
            if (in_degree[v] == 0) {
                q.push(v);
            }
        }
    }

    if (visited_count != graph.get_number_of_vertices()) {
        throw std::runtime_error("Graph is not a DAG (contains cycles)!");
    }

    return dp[target];
}


// BONUS 3B: Bridge and Torch (State Space Graph)
int solve_bridge_and_torch(const std::vector<int>& times) {
    int N = times.size();
    if (N == 0) return 0;
    
    // Bits 0 to N-1: 1 if person is on the right, 0 if on the left.
    // Bit N: 1 if the torch is on the right, 0 if on the left.
    int target_state = (1 << (N + 1)) - 1;

    DirectedGraph state_graph;
    state_graph.add_vertex(0); // Initial state

    std::queue<int> q;
    std::unordered_map<int, bool> visited;

    q.push(0);
    visited[0] = true;

    // Build the state space graph
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        bool torch_on_right = (u & (1 << N)) != 0;
        int people_mask = u & ((1 << N) - 1);

        if (!torch_on_right) {
            // torch on the left: 1 or 2 people cross to the right
            for (int i = 0; i < N; ++i) {
                if (!(people_mask & (1 << i))) {
                    for (int j = i; j < N; ++j) {
                        if (!(people_mask & (1 << j))) {
                            int next_people = people_mask | (1 << i) | (1 << j);
                            int next_state = next_people | (1 << N); // Move torch to right
                            int cost = std::max(times[i], times[j]); // Cost is the slowest person

                            try { state_graph.add_vertex(next_state); }
                            catch (...) {}
                            try { state_graph.add_edge(u, next_state, cost); }
                            catch (...) {}

                            if (!visited[next_state]) {
                                visited[next_state] = true;
                                q.push(next_state);
                            }
                        }
                    }
                }
            }
        }
        else {
            // torch on the right: 1 person goes back
            for (int i = 0; i < N; ++i) {
                if (people_mask & (1 << i)) {
                    int next_people = people_mask & ~(1 << i);
                    int next_state = next_people; // Move torch to left (Bit N becomes 0)
                    int cost = times[i];

                    try { state_graph.add_vertex(next_state); }
                    catch (...) {}
                    try { state_graph.add_edge(u, next_state, cost); }
                    catch (...) {}

                    if (!visited[next_state]) {
                        visited[next_state] = true;
                        q.push(next_state);
                    }
                }
            }
        }
    }

    // Run Bellman-Ford on the generated state graph
    auto result = bellman_ford_shortest_path(state_graph, 0, target_state);
    return result.first;
}





/** * PROBLEM 1: Lowest Length Path (BFS)
 * Finds the shortest path between two nodes in an unweighted graph using
 * Breadth-First Search with O(1) average lookup time via unordered_map.
 */
void lowest_length_path_forward(const DirectedGraph& graph, int start, int target) {
    std::queue<int> q;
    std::unordered_map<int, int> dist;
    std::unordered_map<int, int> prev; // Stores predecessors for path reconstruction

    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == target) break;

        for (auto it = graph.begin_outbound(current); it != graph.end_outbound(current); ++it) {
            int neighbor = *it;
            if (dist.find(neighbor) == dist.end()) {
                dist[neighbor] = dist[current] + 1;
                prev[neighbor] = current;
                q.push(neighbor);
            }
        }
    }

    if (dist.find(target) == dist.end()) {
        std::cout << "No path exists between " << start << " and " << target << ".\n";
        return;
    }

    std::vector<int> path;
    int current_node = target;

    while (current_node != start) {
        path.push_back(current_node);
        current_node = prev[current_node];
    }
    path.push_back(start);

    std::reverse(path.begin(), path.end());

    std::cout << "Forward BFS Path: ";
    for (int v : path) {
        std::cout << v << " ";
    }
    std::cout << "\nLength: " << dist[target] << "\n";
}

/** * BONUS 1B: Strongly Connected Components (Kosaraju's Algorithm)
 * Identifies all SCCs in a directed graph using two DFS passes.
 */
void dfs1(const DirectedGraph& g, int v, std::unordered_map<int, bool>& vis, std::stack<int>& s) {
    vis[v] = true;
    for (auto it = g.begin_outbound(v); it != g.end_outbound(v); ++it)
        if (!vis[*it]) dfs1(g, *it, vis, s);
    s.push(v); // Push to stack after visiting all neighbors (finishing time)
}

void dfs2(const DirectedGraph& g, int v, std::unordered_map<int, bool>& vis) {
    vis[v] = true;
    std::cout << v << " ";
    // Using inbound edges simulates the transposed graph
    for (auto it = g.begin_inbound(v); it != g.end_inbound(v); ++it)
        if (!vis[*it]) dfs2(g, *it, vis);
}

void strongly_connected_components(const DirectedGraph& graph) {
    std::unordered_map<int, bool> vis;
    std::stack<int> s;

    // Pass 1: Fill stack with nodes based on finishing times
    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it)
        if (!vis[it->first]) dfs1(graph, it->first, vis, s);

    vis.clear();
    // Pass 2: Process nodes in decreasing order of finishing time
    int scc_count = 0;
    while (!s.empty()) {
        int v = s.top(); s.pop();
        if (!vis[v]) {
            scc_count++;
            std::cout << "SCC " << scc_count << ": ";
            dfs2(graph, v, vis);
            std::cout << "\n";
        }
    }
}

/** * BONUS 2B: Biconnected Components
 * Finds the biconnected components of an undirected graph in O(n+m) time.
 */
void bcc_dfs(int u, std::unordered_map<int, int>& disc, std::unordered_map<int, int>& low, std::unordered_map<int, int>& parent,
    std::stack<std::pair<int, int>>& st, const DirectedGraph& graph, int& time_counter, int& components_count) {

    disc[u] = low[u] = ++time_counter;

    for (auto it = graph.begin_outbound(u); it != graph.end_outbound(u); ++it) {
        int v = *it;

        // tree edge
        if (disc[v] == -1) {
            parent[v] = u;

            st.push({ u, v });

            bcc_dfs(v, disc, low, parent, st, graph, time_counter, components_count);

            low[u] = std::min(low[u], low[v]);

            // articulation point 
            if (low[v] >= disc[u]) {
                components_count++;
                std::cout << "BCC " << components_count << ": ";

                // pop edges until (u, v)
                while (st.top().first != u || st.top().second != v) {
                    std::cout << "(" << st.top().first << ", " << st.top().second << ") ";
                    st.pop();
                }
                std::cout << "(" << st.top().first << ", " << st.top().second << ")\n";
                st.pop();
            }
        }
        // back edge
        else if (v != parent[u] && disc[v] < disc[u]) {
            low[u] = std::min(low[u], disc[v]);

            st.push({ u, v });
        }
    }
}

/** * Main function to initialize data structures and trigger the BCC algorithm.
 * @param graph: The input graph (simulating an undirected graph via bidirectional edges).
 */
void find_biconnected_components(const DirectedGraph& graph) {
    int n = graph.get_number_of_vertices();
    if (n == 0) return;

    // Using unordered_map to avoid segfaults with non-consecutive IDs
    std::unordered_map<int, int> disc;
    std::unordered_map<int, int> low;
    std::unordered_map<int, int> parent;
    std::stack<std::pair<int, int>> st;

    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        int vertex = it->first;
        disc[vertex] = -1;
        low[vertex] = -1;
        parent[vertex] = -1;
    }

    int time_counter = 0;
    int components_count = 0;

    for (auto it = graph.begin_vertices(); it != graph.end_vertices(); ++it) {
        int i = it->first;
        if (disc[i] == -1) {
            bcc_dfs(i, disc, low, parent, st, graph, time_counter, components_count);

            // the remaining edges form the last biconnected component
            bool has_edges = false;
            while (!st.empty()) {
                if (!has_edges) {
                    components_count++;
                    std::cout << "BCC " << components_count << " (Isolated): ";
                    has_edges = true;
                }
                std::cout << "(" << st.top().first << ", " << st.top().second << ") ";
                st.pop();
            }
            if (has_edges) std::cout << "\n";
        }
    }
}

// Bonus 3B: Wolf, Goat, Cabbage Problem
// Helper to check if a string state is safe
bool is_safe_string(std::string s) {
    char man = s[0], wolf = s[1], goat = s[2], cabbage = s[3];
    // If Wolf and Goat are together but Man is away
    if (wolf == goat && man != wolf) return false;
    // If Goat and Cabbage are together but Man is away
    if (goat == cabbage && man != goat) return false;
    return true;
}

// Helper to flip the side (L -> R or R -> L)
char flip(char side) {
    if (side == 'L') {
        return 'R';
    }
    else {
        return 'L';
    }
}

void solve_wolf_goat_cabbage() {
    std::queue<std::string> q;
    std::unordered_map<std::string, std::string> prev;
    std::unordered_map<std::string, int> dist;

    std::string start = "LLLL";
    std::string target = "RRRR";

    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        if (current == target) break;

        // Try all possible moves (0: Man alone, 1: Man+Wolf, 2: Man+Goat, 3: Man+Cabbage)
        for (int i = 0; i < 4; i++) {
            std::string next = current;

            // Man always moves
            next[0] = flip(current[0]);

            // If i > 0, Man tries to take item i with him
            if (i > 0) {
                if (current[i] == current[0]) {
                    next[i] = flip(current[i]);
                }
                else {
                    continue;
                }
            }

            if (is_safe_string(next) && dist.find(next) == dist.end()) {
                dist[next] = dist[current] + 1;
                prev[next] = current;
                q.push(next);
            }
        }
    }

    if (dist.find(target) == dist.end()) {
        std::cout << "No solution found!\n";
    }
    else {
        std::vector<std::string> path;
        for (std::string curr = target; curr != ""; curr = prev[curr]) {
            path.push_back(curr);
            if (curr == start) break;
        }
        std::reverse(path.begin(), path.end());

        std::cout << "Solution Found in " << dist[target] << " steps:\n\n";
        int step = 0;
        for (std::string s : path) {
            std::string left = "", right = "";
            if (s[1] == 'L') left += "W"; else right += "W";
            if (s[3] == 'L') left += "C"; else right += "C";
            if (s[2] == 'L') left += "G"; else right += "G";
            if (s[0] == 'L') left += "B"; else right += "B";

            std::cout << "Step " << step++ << ": " << left << "-" << right << "\n";
        }
    }
}