# Graph Algorithms

C++ implementations of graph algorithms from the Graph Algorithms course at Babeș-Bolyai University (year 1, spring 2026). Everything runs from a console menu.

## The graph

`DirectedGraph` (`Graph.h`, `Graph.cpp`) stores a directed graph with edge costs in three hash maps:

- the outbound neighbours of each vertex
- the inbound neighbours of each vertex
- the cost of each edge, keyed by (source, target)

Vertices and edges can be added and removed, and the graph can be copied. In/out degree lookups are O(1); checking whether an edge exists is O(deg(source)).

## Algorithms

| Lab | Problem | Method |
|---|---|---|
| 2 | Lowest-length path between two vertices | forward BFS |
| 2 (bonus) | Strongly connected components | Kosaraju |
| 2 (bonus) | Biconnected components | DFS with discovery and low times (edges must exist in both directions) |
| 2 (bonus) | Wolf, goat and cabbage puzzle | BFS on a state graph |
| 3 | Lowest-cost walk | Bellman-Ford, with negative-cycle detection |
| 3 (bonus) | Number of distinct minimum-cost walks | Bellman-Ford + memoized DFS |
| 3 (bonus) | Number of distinct walks in a DAG | topological sort + DP |
| 3 (bonus) | Bridge and torch puzzle | Bellman-Ford on a state graph |
| 4 | Highest-cost path in a DAG | Kahn's topological sort + DP |
| 4 (bonus) | Rebuild a binary tree from its preorder and inorder traversals | recursion |
| 4 (bonus) | Number of distinct paths in a DAG | Kahn's topological sort + DP |
| 4 (bonus) | Number of distinct lowest-cost paths in a DAG | Kahn's topological sort + DP |
| 5 | Hamiltonian cycle | backtracking |

## Input format

The first line holds the number of vertices `n` and the number of edges `m`, followed by `m` lines with one edge each: `source target cost`. Vertices are numbered from 0 to n-1.

```
8 12
0 1 10
0 2 15
...
```

Example files: `example1_DAG.txt`, `example2_DAG.txt`, `example_hamiltonian.txt`, and random graphs with 1,000 and 10,000 vertices (`graph1k.txt`, `graph10k.txt`). The 100,000 and 1,000,000 vertex test graphs are not in the repository because of their size.

## Build and run

Visual Studio: open `Graph_p1.slnx` and run.

g++:

```
g++ -std=c++17 -O2 Graph.cpp GraphAlgorithms.cpp main.cpp -o graph
./graph
```

Start with option 10 to read a graph from a file.

## Performance

On a random graph with 1,000,000 vertices and 4,000,000 edges (g++ -O2), reading the file and building the graph takes about 7 seconds, and BFS takes about 2 seconds.

## What I would improve

- BFS keeps distances and predecessors in `unordered_map`s, which allocate a node for every vertex it reaches. Vectors indexed by vertex would make it much faster.
- Kosaraju and the biconnected components use a recursive DFS, which can run out of stack on very large graphs. An iterative DFS would fix this.
- The random graph generator computes `n * (n - 1)` as an `int`, which overflows for very large `n`.
- `is_edge` could be O(1) by looking up the cost map instead of scanning the adjacency list.
