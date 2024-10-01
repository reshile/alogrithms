#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Node {
  std::unordered_map<char, size_t> to;
  bool is_term = false;
  bool is_required = false;
  size_t word_id = 0;

  Node() {}
};

struct Bor {
  std::vector<Node> nodes;
  std::vector<size_t> suf_ref;
  std::vector<std::unordered_map<char, int>> step;

  std::vector<size_t> word_cnt;
  std::vector<size_t> word_ids;
  std::vector<size_t> word_size;

  std::vector<std::vector<size_t>> positions;

  std::string main_string;

  std::unordered_set<char> alphabet;

  Bor() {
    size_t words_cnt;
    std::cin >> main_string;
    std::cin >> words_cnt;

    positions.resize(words_cnt);
    word_cnt.resize(words_cnt);

    for (auto ch : main_string) {
      alphabet.insert(ch);
    }

    nodes.push_back(Node());

    std::string input;
    for (size_t i = 0; i < words_cnt; ++i) {
      std::cin >> input;
      size_t current_node = 0;
      for (auto ch : input) {
        current_node = HandleNode(current_node, ch);
      }
      nodes[current_node].is_term = true;
      nodes[current_node].is_required = true;
      nodes[current_node].word_id = i;
      word_ids.push_back(current_node);
      word_size.push_back(input.size());
    }
  }

  void SetUpReferences() {
    suf_ref.resize(nodes.size());
    step.resize(nodes.size());

    for (auto ch : alphabet) {
      step[0][ch] = NodeHasTransitionBy(0, ch) ? nodes[0].to[ch] : 0;
    }
  }

  bool NodeHasTransitionBy(const size_t& node_index, char ch) {
    return (nodes[node_index].to.find(ch) != nodes[node_index].to.end());
  }

  size_t HandleNode(size_t& node_index, char ch) {
    if (!NodeHasTransitionBy(node_index, ch)) {
      nodes[node_index].to[ch] = nodes.size();
      nodes.push_back(Node());
    }
    return nodes[node_index].to[ch];
  }

  void BFS() {
    std::queue<size_t> bfs_order;
    bfs_order.push(0);

    while (!bfs_order.empty()) {
      size_t cur_node = bfs_order.front();
      bfs_order.pop();

      for (const auto& [ch, node_id] : nodes[cur_node].to) {
        suf_ref[node_id] = cur_node == 0 ? 0 : step[suf_ref[cur_node]][ch];
        if (cur_node != 0 && nodes[suf_ref[node_id]].is_term) {
          nodes[node_id].is_term = true;
        }

        for (auto ch : alphabet) {
          step[node_id][ch] = NodeHasTransitionBy(node_id, ch)
                                  ? nodes[node_id].to[ch]
                                  : step[suf_ref[node_id]][ch];
        }
        bfs_order.push(node_id);
      }
    }
  }

  void HandleTerm(size_t cur_pos, size_t word_pos) {
    while (nodes[cur_pos].is_term) {
      if (nodes[cur_pos].is_required) {
        ++word_cnt[nodes[cur_pos].word_id];
        positions[nodes[cur_pos].word_id].push_back(word_pos);
      }
      cur_pos = suf_ref[cur_pos];
    }
  }

  void MainLoop() {
    size_t pos = 0;
    for (size_t ch = 0; ch < main_string.size(); ++ch) {
      pos = step[pos][main_string[ch]];

      if (!nodes[pos].is_term) {
        continue;
      }
      HandleTerm(pos, ch);
    }
  }

  void GetAnswer() {
    for (size_t word = 0; word < word_ids.size(); ++word) {
      std::cout << word_cnt[word] << " ";
      for (auto pos : positions[word]) {
        std::cout << pos + 2 - word_size[word] << " ";
      }
      std::cout << '\n';
    }
  }
};

int main() {
  Bor bor;
  bor.SetUpReferences();
  bor.BFS();
  bor.MainLoop();
  bor.GetAnswer();
}
