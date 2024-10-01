#include <algorithm>
#include <iostream>
#include <vector>

const int64_t kMax = 1000000000;

struct Node {
  Node* left = nullptr;
  Node* right = nullptr;
  int64_t diff = 0;
  int64_t min = 0;
  int64_t min_cnt = 0;

  Node() {}

  Node(int64_t min_cnt) : min_cnt(min_cnt) {}

  ~Node() {
    delete left;
    delete right;
  }
};

void Propagate(Node* node) {
  node->left->diff += node->diff;
  node->right->diff += node->diff;
  node->left->min += node->diff;
  node->right->min += node->diff;
  node->diff = 0;
}

void ReCountMin(Node* node) {
  if (node->left->min == node->right->min) {
    node->min = node->left->min;
    node->min_cnt = node->left->min_cnt + node->right->min_cnt;
  } else if (node->left->min < node->right->min) {
    node->min = node->left->min;
    node->min_cnt = node->left->min_cnt;
  } else {
    node->min = node->right->min;
    node->min_cnt = node->right->min_cnt;
  }
}

void AddInRange(Node* node, int64_t left, int64_t right, int64_t lx,
                int64_t rx) {
  if (left == lx && right == rx) {
    ++node->diff;
    ++node->min;
    return;
  }
  int64_t md = (lx + rx) / 2;
  if (node->left == nullptr) {
    node->left = new Node(md - lx + 1);
  }
  if (node->right == nullptr) {
    node->right = new Node(rx - md);
  }
  Propagate(node);
  if (left <= md) {
    AddInRange(node->left, left, std::min(right, md), lx, md);
  }
  if (right > md) {
    AddInRange(node->right, std::max(left, md + 1), right, md + 1, rx);
  }
  ReCountMin(node);
}

void SubInRange(Node* node, int64_t left, int64_t right, int64_t lx,
                int64_t rx) {
  if (left == lx && right == rx) {
    --node->diff;
    --node->min;
    return;
  }
  int64_t md = (lx + rx) / 2;
  if (node->left == nullptr) {
    node->left = new Node(md - lx + 1);
  }
  if (node->right == nullptr) {
    node->right = new Node(rx - md);
  }
  Propagate(node);
  if (left <= md) {
    SubInRange(node->left, left, std::min(right, md), lx, md);
  }
  if (right > md) {
    SubInRange(node->right, std::max(left, md + 1), right, md + 1, rx);
  }
  ReCountMin(node);
}

int64_t GetZero(Node* node) {
  if (node->min == 0) {
    return node->min_cnt;
  }
  return 0;
}

struct Segment {
  int64_t xpos = 0;
  int64_t y1 = 0;
  int64_t y2 = 0;
  int64_t type = 1;
  // type = 1 <-> open , type = -1 <-> close

  Segment() {}

  Segment(int64_t xpos, int64_t y1, int64_t y2, int64_t type)
      : xpos(xpos), y1(y1), y2(y2), type(type) {}

  bool operator<(const Segment& seg) const {
    return (xpos < seg.xpos || (xpos == seg.xpos && type > seg.type));
  }
};

void Answer(int cnt) {
  Node* root = new Node(kMax);
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int64_t x1;
  int64_t y1;
  int64_t x2;
  int64_t y2;
  std::vector<Segment> vec;
  for (int i = 0; i < cnt; ++i) {
    std::cin >> x1 >> y1 >> x2 >> y2;
    if (x1 == x2 || y1 == y2) {
      continue;
    }
    vec.push_back({x1, y1, y2 - 1, 1});
    vec.push_back({x2, y1, y2 - 1, -1});
  }
  std::sort(vec.begin(), vec.end());

  int64_t width = 0;
  int64_t lenght = 0;

  int64_t open_cnt = 0;
  int64_t answer = 0;
  if (vec.empty()) {
    std::cout << 0;
    delete root;
    return;
  }
  for (size_t i = 0; i < vec.size() - 1; ++i) {
    if (vec[i].type == 1) {
      AddInRange(root, vec[i].y1, vec[i].y2, 0, kMax);
    } else {
      SubInRange(root, vec[i].y1, vec[i].y2, 0, kMax);
    }
    open_cnt += vec[i].type;
    if (open_cnt > 0) {
      width = kMax - GetZero(root) + 1;
      lenght = vec[i + 1].xpos - vec[i].xpos;
      answer += width * lenght;
    } else {
      continue;
    }
  }

  std::cout << answer;
  delete root;
}

int main() {
  int cnt;
  std::cin >> cnt;
  Answer(cnt);
}
