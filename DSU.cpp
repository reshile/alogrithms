#include <algorithm>
#include <iostream>
#include <vector>

#define Vertex int

struct DSU {
  std::vector<Vertex> tree;
  std::vector<size_t> rank;

  DSU(size_t max_size) : tree(max_size), rank(max_size) {}

  // Set 'parent' reference of vertex to itself
  void CreateSet(Vertex vertex) {
    tree[vertex] = vertex;
    rank[vertex] = 0;
  }

  // Go up the tree to find 'root' vertex
  size_t GetComponentId(Vertex vertex) {
    if (vertex == tree[vertex]) {  // 'root' has been found
      return vertex;
    }
    tree[vertex] = GetComponentId(tree[vertex]);
    return tree[vertex];
  }

  void Unite(Vertex lhs, Vertex rhs) {
    Vertex lhs_parent = GetComponentId(lhs);
    Vertex rhs_parent = GetComponentId(rhs);
    if (lhs_parent != rhs_parent) {
      if (rank[lhs_parent] < rank[rhs_parent]) {
        std::swap(lhs_parent, rhs_parent);
      }
      tree[rhs_parent] = lhs_parent;
      if (rank[lhs_parent] == rank[rhs_parent]) {
        ++rank[lhs_parent];
      }
    }
  }

  bool InSameComponent(Vertex lhs, Vertex rhs) {
    return (GetComponentId(lhs) == GetComponentId(rhs));
  }
};

struct Edge {
  Vertex from;
  Vertex to;
  size_t cost;

  Edge() = default;

  Edge(Vertex from, Vertex to, size_t cost) : from(from), to(to), cost(cost) {}
};

struct MSTEdge : public Edge {
  bool used = false;

  MSTEdge() = default;

  MSTEdge(Vertex from, Vertex to, size_t cost) : Edge(from, to, cost) {}
};

auto comp = [](const MSTEdge& lhs, const MSTEdge& rhs) {
  return lhs.cost < rhs.cost;
};

struct Task {
  DSU dsu;
  std::vector<MSTEdge> graph;
  size_t sum = 0;

  Task(size_t vertices, size_t edges) : dsu(vertices), graph(edges) {
    Vertex from;
    Vertex to;
    size_t cost;
    for (size_t i = 0; i < edges; ++i) {
      std::cin >> from >> to >> cost;
      graph[i] = MSTEdge(from - 1, to - 1, cost);
    }

    for (size_t i = 0; i < vertices; ++i) {
      dsu.CreateSet(i);
    }
    std::sort(graph.begin(), graph.end(), comp);
  }

  void Calculate() {
    for (auto& edge : graph) {
      if (!dsu.InSameComponent(edge.from, edge.to)) {
        sum += edge.cost;
        dsu.Unite(edge.from, edge.to);
      }
    }

    std::cout << sum << '\n';
  }
};

int main() {
  size_t vertices;
  size_t edges;
  std::cin >> vertices >> edges;
  Task(vertices, edges).Calculate();
}
