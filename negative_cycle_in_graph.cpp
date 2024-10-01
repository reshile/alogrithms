#include <algorithm>
#include <iostream>
#include <vector>

struct Edge {
  int fromv;
  int tov;
  int cost;
};

struct Task {
  std::vector<Edge> edges;
  std::vector<int> dist;
  std::vector<int> parents;
  int vertices;
  const int cKNoedge = 100000;

  Task(int vertices)
      : dist(vertices, 0), parents(vertices, -1), vertices(vertices) {
    int cost;
    for (int i = 0; i < vertices; ++i) {
      for (int j = 0; j < vertices; ++j) {
        std::cin >> cost;
        if (cost != cKNoedge) {
          edges.push_back(Edge{i, j, cost});
        }
      }
    }
  }

  void SearchNegativeCycle() {
    int potential_cycle_vertex = -1;
    for (int i = 0; i < vertices; ++i) {
      potential_cycle_vertex = -1;
      for (auto& edge : edges) {
        if (dist[edge.tov] > dist[edge.fromv] + edge.cost) {
          dist[edge.tov] = dist[edge.fromv] + edge.cost;
          parents[edge.tov] = edge.fromv;
          potential_cycle_vertex = edge.tov;
        }
      }
    }

    if (potential_cycle_vertex == -1) {
      std::cout << "NO\n";
    } else {
      std::cout << "YES\n";
      int vertex_on_cycle = potential_cycle_vertex;
      for (int i = 0; i < vertices - 1; ++i) {
        vertex_on_cycle = parents[vertex_on_cycle];
      }

      std::vector<int> cycle;
      for (int current = vertex_on_cycle;; current = parents[current]) {
        cycle.push_back(current);
        if (current == vertex_on_cycle && cycle.size() > 1) {
          break;
        }
      }
      std::cout << cycle.size() << '\n';
      std::reverse(cycle.begin(), cycle.end());
      for (auto& cycle_elem : cycle) {
        std::cout << cycle_elem + 1 << " ";
      }
    }
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int vertices;
  std::cin >> vertices;
  Task task(vertices);
  task.SearchNegativeCycle();
}
