#include "Graph.h"
#include <algorithm>

DirectedGraph::DirectedGraph() {}

DirectedGraph::DirectedGraph(int n) {
    for (int i = 0; i < n; ++i) {
        dict_out[i] = std::vector<int>();
        dict_in[i] = std::vector<int>();
    }
}

DirectedGraph::VertexIterator DirectedGraph::begin_vertices() const {
    return dict_out.begin();
}
DirectedGraph::VertexIterator DirectedGraph::end_vertices() const {
    return dict_out.end();
}

DirectedGraph::NeighborIterator DirectedGraph::begin_outbound(int v) const {
    if (dict_out.find(v) == dict_out.end()) throw std::invalid_argument("Vertex missing.");
    return dict_out.at(v).begin();
}
DirectedGraph::NeighborIterator DirectedGraph::end_outbound(int v) const {
    if (dict_out.find(v) == dict_out.end()) throw std::invalid_argument("Vertex missing.");
    return dict_out.at(v).end();
}

DirectedGraph::NeighborIterator DirectedGraph::begin_inbound(int v) const {
    if (dict_in.find(v) == dict_in.end()) throw std::invalid_argument("Vertex missing.");
    return dict_in.at(v).begin();
}
DirectedGraph::NeighborIterator DirectedGraph::end_inbound(int v) const {
    if (dict_in.find(v) == dict_in.end()) throw std::invalid_argument("Vertex missing.");
    return dict_in.at(v).end();
}

int DirectedGraph::get_number_of_vertices() const {
    return dict_out.size();
}

int DirectedGraph::get_number_of_edges() const {
    return dict_costs.size();
}

bool DirectedGraph::is_edge(int source, int target) const {
    if (dict_out.find(source) == dict_out.end() || dict_out.find(target) == dict_out.end()) {
        return false;
    }
    // Search target in the outbound list of source ( O(deg(source)) )
    const auto& out_edges = dict_out.at(source);
    return std::find(out_edges.begin(), out_edges.end(), target) != out_edges.end();
}

int DirectedGraph::get_in_degree(int v) const {
    if (dict_in.find(v) == dict_in.end()) throw std::invalid_argument("Vertex missing.");
    return dict_in.at(v).size();
}

int DirectedGraph::get_out_degree(int v) const {
    if (dict_out.find(v) == dict_out.end()) throw std::invalid_argument("Vertex missing.");
    return dict_out.at(v).size();
}

int DirectedGraph::get_edge_cost(int source, int target) const {
    if (!is_edge(source, target)) throw std::invalid_argument("Edge missing.");
    return dict_costs.at({ source, target });
}

void DirectedGraph::set_edge_cost(int source, int target, int new_cost) {
    if (!is_edge(source, target)) throw std::invalid_argument("Edge missing.");
    dict_costs[{source, target}] = new_cost;
}

void DirectedGraph::add_vertex(int v) {
    if (dict_out.find(v) != dict_out.end()) throw std::invalid_argument("Vertex already exists.");
    dict_out[v] = std::vector<int>();
    dict_in[v] = std::vector<int>();
}

void DirectedGraph::remove_vertex(int v) {
    if (dict_out.find(v) == dict_out.end()) throw std::invalid_argument("Vertex missing.");

    // We copy the lists of neighbors first, because we cannot modify a dictionary while iterating over it.
    std::vector<int> out_copy = dict_out[v];
    for (int target : out_copy) {
        remove_edge(v, target);
    }

    std::vector<int> in_copy = dict_in[v];
    for (int source : in_copy) {
        remove_edge(source, v);
    }

    dict_out.erase(v);
    dict_in.erase(v);
}

void DirectedGraph::add_edge(int source, int target, int cost) {
    if (dict_out.find(source) == dict_out.end() || dict_out.find(target) == dict_out.end()) {
        throw std::invalid_argument("Source or target missing.");
    }
    if (is_edge(source, target)) throw std::invalid_argument("Edge already exists.");

    dict_out[source].push_back(target);
    dict_in[target].push_back(source);
    dict_costs[{source, target}] = cost;
}

void DirectedGraph::remove_edge(int source, int target) {
    if (!is_edge(source, target)) throw std::invalid_argument("Edge missing.");

    auto& out_vec = dict_out[source];
    out_vec.erase(std::remove(out_vec.begin(), out_vec.end(), target), out_vec.end());

    auto& in_vec = dict_in[target];
    in_vec.erase(std::remove(in_vec.begin(), in_vec.end(), source), in_vec.end());

    dict_costs.erase({ source, target });
}

DirectedGraph DirectedGraph::copy() const {
    DirectedGraph new_graph = *this;
    return new_graph;
}