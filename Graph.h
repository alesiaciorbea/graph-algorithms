#pragma once
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <utility>

 // hash for std::pair (source, target) keys
 
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

class DirectedGraph {
private:
    // the 3 dictionaries required by the lab, for O(1) operations

     // dict_out: vertex -> outbound neighbors
    std::unordered_map<int, std::vector<int>> dict_out;

    // dict_in: vertex -> inbound neighbors
    std::unordered_map<int, std::vector<int>> dict_in;

    // dict_costs: maps an edge (source, target) to its cost
    std::unordered_map<std::pair<int, int>, int, pair_hash> dict_costs;

public:
    DirectedGraph();
    explicit DirectedGraph(int n); // Creates graph with n vertices, 0 edges


     // const iterators: the graph can be parsed but not modified
    typedef std::unordered_map<int, std::vector<int>>::const_iterator VertexIterator;
    typedef std::vector<int>::const_iterator NeighborIterator;

    VertexIterator begin_vertices() const;
    VertexIterator end_vertices() const;
    NeighborIterator begin_outbound(int v) const;
    NeighborIterator end_outbound(int v) const;
    NeighborIterator begin_inbound(int v) const;
    NeighborIterator end_inbound(int v) const;

    int get_number_of_vertices() const;
    int get_number_of_edges() const;

    // Complexity: O(deg(source))
    bool is_edge(int source, int target) const;

    // Complexity: O(1)
    int get_in_degree(int v) const;
    int get_out_degree(int v) const;

    // Complexity: O(deg(source)), because of the is_edge check
    int get_edge_cost(int source, int target) const;
    void set_edge_cost(int source, int target, int new_cost);

    void add_vertex(int v);
    void remove_vertex(int v);
    void add_edge(int source, int target, int cost = 0);
    void remove_edge(int source, int target);

    DirectedGraph copy() const;
};