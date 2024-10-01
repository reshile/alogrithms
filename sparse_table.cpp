#include <cmath>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

const int kMax = std::numeric_limits<int>::max();

using Pair = std::pair<int, int>;
using Vector = std::vector<std::vector<Pair>>;

struct SparseTable {
  int size;
  int height;
  std::vector<int> len;
  Vector table;

  SparseTable(int size, std::vector<int>& vec)
      : size(size),
        height(std::log2(size) + 1),
        len(size + 1),
        table(size, std::vector<Pair>(height, {0, 0})) {
    Init(vec);
    for (int i = 1; i <= size; ++i) {
      len[i] = Floor(i);
    }
  }

  int Floor(int len) {
    if (len == 1) {
      return 0;
    }
    return Floor(len / 2) + 1;
  }

  void Init(std::vector<int>& vec) {
    int exp;
    for (int i = 0; i < size; ++i) {
      table[i][0] = {vec[i], i};
    }
    for (int j = 1; j < height; ++j) {
      exp = std::pow(2, j) - 1;
      for (int i = 0; i + exp < size; ++i) {
        table[i][j] =
            std::min(table[i][j - 1], table[i + std::pow(2, j - 1)][j - 1]);
      }
    }
  }

  Pair GetPair(int lx, int rx) {
    int exp = len[rx - lx + 1];
    return std::min(table[lx - 1][exp], table[rx - std::pow(2, exp)][exp]);
  }

  int GetAns(int lx, int rx) {
    Pair minp = GetPair(lx, rx);
    Pair minp_left = {kMax, kMax};
    Pair minp_right = {kMax, kMax};
    if (lx < minp.second + 1) {
      minp_left = GetPair(lx, minp.second);
    }
    if (rx > minp.second + 1) {
      minp_right = GetPair(minp.second + 2, rx);
    }
    return std::min(minp_left, minp_right).first;
  }
};

void Answer(int size, int req) {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int lx;
  int rx;
  std::vector<int> vec(size);
  for (int i = 0; i < size; ++i) {
    std::cin >> vec[i];
  }

  SparseTable sp(size, vec);
  for (int i = 0; i < req; ++i) {
    std::cin >> lx >> rx;
    std::cout << sp.GetAns(lx, rx) << '\n';
  }
}

int main() {
  int size;
  int req;
  std::cin >> size >> req;
  Answer(size, req);
}
