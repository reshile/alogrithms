#include <iostream>
#include <vector>

const int kMax = 1000000000;

struct Node {
  Node* left = nullptr;
  Node* right = nullptr;
  int value = 0;
  int hight = 0;

  Node() {}

  Node(int value) : value(value) {}

  ~Node() {
    delete left;
    delete right;
  }
};

struct Tree {
  Node* root = nullptr;

  static int GetHight(Node* node) {
    if (node != nullptr) {
      return node->hight;
    }
    return 0;
  }

  static int GetValue(Node* node) {
    if (node != nullptr) {
      return node->value;
    }
    return -1;
  }

  static void Recount(Node* node) {
    node->hight = std::max(GetHight(node->left), GetHight(node->right)) + 1;
  }

  static void Rotate(Node* first, Node* second) {
    if (first->right == second) {
      first->right = second->left;
      second->left = first;
    } else {
      first->left = second->right;
      second->right = first;
    }
    Recount(first);
    Recount(second);
  }

  static Node* RightRotate(Node* node) {
    Node* left = node->left;
    node->left = left->right;
    left->right = node;
    Recount(node);
    Recount(left);
    return left;
  }

  static Node* LeftRotate(Node* node) {
    Node* right = node->right;
    node->right = right->left;
    right->left = node;
    Recount(node);
    Recount(right);
    return right;
  }

  Node* Add(Node* cur_node, Node* new_node) {
    if (cur_node == nullptr) {
      Recount(new_node);
      return new_node;
    }
    if (new_node->value == cur_node->value) {
      delete (new_node);
      return cur_node;
    }
    if (new_node->value > cur_node->value) {
      cur_node->right = Add(cur_node->right, new_node);
    } else if (new_node->value < cur_node->value) {
      cur_node->left = Add(cur_node->left, new_node);
    }
    Recount(cur_node);
    if ((GetHight(cur_node->left) - GetHight(cur_node->right) == 2)) {
      Node* left_son = cur_node->left;
      if ((GetHight(left_son->right) - GetHight(left_son->left) > 0)) {
        cur_node->left = LeftRotate(cur_node->left);
      }
      return RightRotate(cur_node);
    }
    if ((GetHight(cur_node->left) - GetHight(cur_node->right) == -2)) {
      Node* right_son = cur_node;
      if ((GetHight(right_son->right) - GetHight(right_son->left) < 0)) {
        cur_node->right = RightRotate(cur_node->right);
      }
      return LeftRotate(cur_node);
    }
    return cur_node;
  }

  void AddToTree(Node* new_node) { root = Add(root, new_node); }

  int GetNext(Node* node, int value) {
    if (node == nullptr) {
      return -1;
    }
    if (GetValue(node) > value) {
      if (node->left == nullptr || node->left->value < value) {
        return node->value;
      }
      return GetNext(node->left, value);
    }
    if (GetValue(node) < value) {
      return GetNext(node->right, value);
    }
    return GetValue(node);
  }

  int Get(int value) { return GetNext(root, value); }

  int FindInTree(Node* node, int value, int prev_value) {
    if (node == nullptr) {
      return prev_value;
    }
    if (node->value == value) {
      return value;
    }
    if (node->value > value) {
      prev_value = node->value;
      return FindInTree(node->left, value, prev_value);
    }
    if (node->value < value) {
      return FindInTree(node->right, value, prev_value);
    }
    return prev_value;
  }

  int Find(int value) { return FindInTree(root, value, -1); }

  ~Tree() { delete root; }
};

void Answer(int req) {
  Tree tree;
  Node* new_node = nullptr;
  int64_t prev_value = 0;
  char prev_action = '\0';
  int64_t value = 0;
  char action = '\0';

  for (int i = 0; i < req; ++i) {
    std::cin >> action;
    if (action == '+') {
      std::cin >> value;
      if (prev_action == '?') {
        value += prev_value;
        value %= kMax;
      }
      prev_action = action;
      new_node = new Node(value);
      tree.AddToTree(new_node);
      continue;
    }
    if (action == '?') {
      prev_action = action;
      std::cin >> value;
      prev_value = tree.Find(value);
      std::cout << prev_value << '\n';
    }
  }
}

int main() {
  int req;
  std::cin >> req;
  Answer(req);
}
