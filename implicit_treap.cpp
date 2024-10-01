#include <iostream>
#include <random>
#include <string>
#include <vector>

const int kMax = 1000000001;
const int kMin = -1000000001;

struct Node;

using Pair = std::pair<Node*, Node*>;

struct Node {
  Node* left = nullptr;
  Node* right = nullptr;
  int value;
  int prior;
  int subtree_size = 1;

  Node(int value, int prior) : value(value), prior(prior) {}

  ~Node() {
    delete left;
    delete right;
  }
};

struct Tree {
  Node* root = nullptr;

  static size_t GetSubtreeSize(Node* node) {
    if (node == nullptr) {
      return 0;
    }
    return node->subtree_size;
  }

  static void Recalc(Node* node) {
    if (node == nullptr) {
      return;
    }
    node->subtree_size = 1;
    if (node->left != nullptr) {
      node->subtree_size += node->left->subtree_size;
    }
    if (node->right != nullptr) {
      node->subtree_size += node->right->subtree_size;
    }
  }

  static int GetRand() {
    static std::random_device rand;
    static std::mt19937 random_gen(rand());
    std::uniform_int_distribution<int> distro(1, kMax);
    return distro(rand);
  }

  Node* Merge(Node* atree, Node* btree) {
    if (atree == nullptr) {
      return btree;
    }
    if (btree == nullptr) {
      return atree;
    }
    if (atree->prior > btree->prior) {
      atree->right = Merge(atree->right, btree);
      Recalc(atree);
      return atree;
    }
    btree->left = Merge(atree, btree->left);
    Recalc(btree);
    return btree;
  }

  Pair Split(Node* node, int value) {
    if (node == nullptr) {
      return {nullptr, nullptr};
    }
    if (node->value < value) {
      Pair pair = Split(node->right, value);
      node->right = pair.first;
      Recalc(node);
      return {node, pair.second};
    }
    Pair pair = Split(node->left, value);
    node->left = pair.second;
    Recalc(node);
    return {pair.first, node};
  }

  Node* Insert(Node* node, Node* new_node) {
    if (node == nullptr) {
      return new_node;
    }
    if (new_node->prior > node->prior) {
      Pair pair = Split(node, new_node->value);
      new_node->left = pair.first;
      new_node->right = pair.second;
      Recalc(new_node);
      return new_node;
    }
    if (new_node->value < node->value) {
      node->left = Insert(node->left, new_node);
    } else {
      node->right = Insert(node->right, new_node);
    }
    Recalc(node);
    return node;
  }

  Node* Remove(Node* node, int value) {
    if (node->value == value) {
      Node* new_node = Merge(node->left, node->right);
      node->left = nullptr;
      node->right = nullptr;
      delete (node);
      Recalc(new_node);
      return new_node;
    }
    if (value < node->value) {
      node->left = Remove(node->left, value);
    } else {
      node->right = Remove(node->right, value);
    }
    Recalc(node);
    return node;
  }

  void Exists(int value) {
    if (Find(root, value)) {
      std::cout << "true\n";
    } else {
      std::cout << "false\n";
    }
  }

  void Next(int value) {
    int result = FindNext(root, value, kMax);
    if (result == kMax) {
      std::cout << "none\n";
    } else {
      std::cout << result << '\n';
    }
  }

  void Previous(int value) {
    int result = FindPrev(root, value, kMin);
    if (result == kMin) {
      std::cout << "none\n";
    } else {
      std::cout << result << '\n';
    }
  }

  void GetKth(size_t index) {
    if (index >= GetSubtreeSize(root)) {
      std::cout << "none\n";
    } else {
      std::cout << FindKth(root, index) << '\n';
    }
  }

  static Node* CreateNode(int value) {
    Node* new_node = new Node(value, GetRand());
    return new_node;
  }

  void TreeInsert(int value) {
    if (Find(root, value)) {
      return;
    }
    Node* new_node = CreateNode(value);
    root = Insert(root, new_node);
  }

  void TreeRemove(int value) {
    if (!Find(root, value)) {
      return;
    }
    root = Remove(root, value);
  }

  bool Find(Node* node, int value) {
    if (node == nullptr) {
      return false;
    }
    if (node->value == value) {
      return true;
    }
    if (node->value > value) {
      return Find(node->left, value);
    }
    return Find(node->right, value);
  }

  int FindNext(Node* node, int value, int prev_value) {
    if (node == nullptr) {
      return prev_value;
    }
    if (node->value > value) {
      return FindNext(node->left, value, std::min(prev_value, node->value));
    }
    if (node->value <= value) {
      return FindNext(node->right, value, prev_value);
    }
    return prev_value;
  }

  int FindPrev(Node* node, int value, int prev_value) {
    if (node == nullptr) {
      return prev_value;
    }
    if (node->value < value) {
      return FindPrev(node->right, value, std::max(prev_value, node->value));
    }
    if (node->value >= value) {
      return FindPrev(node->left, value, prev_value);
    }
    return prev_value;
  }

  int FindKth(Node* node, size_t index) {
    if (GetSubtreeSize(node->left) == index) {
      return node->value;
    }
    if (index < GetSubtreeSize(node->left)) {
      return FindKth(node->left, index);
    }
    return FindKth(node->right, index - GetSubtreeSize(node->left) - 1);
  }

  ~Tree() { delete root; }
};

void Answer() {
  std::string insert = "insert";
  std::string del = "delete";
  std::string exists = "exists";
  std::string next = "next";
  std::string prev = "prev";
  std::string kth = "kth";
  std::string input;
  int value;
  Tree tree;

  while (std::cin >> input >> value) {
    if (input == insert) {
      tree.TreeInsert(value);
      continue;
    }
    if (input == del) {
      tree.TreeRemove(value);
      continue;
    }
    if (input == exists) {
      tree.Exists(value);
      continue;
    }
    if (input == next) {
      tree.Next(value);
      continue;
    }
    if (input == prev) {
      tree.Previous(value);
      continue;
    }
    if (input == kth) {
      tree.GetKth(value);
      continue;
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  Answer();
}
