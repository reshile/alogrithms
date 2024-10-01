#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

enum { EmptyChar = '+', DeletedChar = '-', AlphabetSize = 52 };

using Pair = std::pair<bool, int>;

std::vector<char> alphabet(AlphabetSize);

const std::string kEmpty = " ";

void InitAlphabet() {
  char begin = 'a';
  for (int i = 0; i < AlphabetSize / 2; ++i) {
    alphabet[i] = begin;
    ++begin;
  }
  begin = 'A';
  for (int i = AlphabetSize / 2; i < AlphabetSize; ++i) {
    alphabet[i] = begin;
    ++begin;
  }
}

struct Hash {
  std::vector<std::string> table;
  const uint64_t kPrime = 15101111;
  const uint64_t kSeed = 101;
  uint64_t size;

  Hash(int req) : size(2 * req) { table.resize(size, kEmpty); }

  void Set(const std::string& str) {
    bool exists = CheckIfExists(str);
    if (exists) {
      return;
    }
    uint64_t pos = StringHash(str);
    while (table[pos] != kEmpty) {
      ++pos %= size;
    }
    table[pos] = str;
  }

  bool CheckIfExists(const std::string& str) {
    uint64_t pos = StringHash(str);
    while (table[pos] != kEmpty) {
      if (table[pos] == str) {
        return true;
      }
      ++pos %= size;
    }
    return false;
  }

  uint64_t StringHash(const std::string& str) const {
    uint64_t hash = 0;
    uint64_t mult = 1;
    for (auto ch : str) {
      mult *= AlphabetSize / 2;
      hash += (ch * mult);
      hash %= kPrime;
    }
    return hash % size;
  }
};

struct Map {
  std::vector<std::pair<char, char>> map;
  const int64_t kPrime = 15101111;
  int64_t coeff;
  int64_t size;

  Map(int req) : size(2 * req) {
    map.resize(size, {EmptyChar, EmptyChar});
    SetCoeff();
  }

  void Set(char key, char value) {
    int64_t pos = HashFunc(key);
    while (map[pos].first != EmptyChar) {
      ++pos %= size;
    }
    map[pos].first = key;
    map[pos].second = value;
  }

  Pair CheckIfExists(char key) {
    int64_t pos = HashFunc(key);
    while (map[pos].first != EmptyChar) {
      if (map[pos].first == key) {
        return {true, map[pos].second};
      }
      ++pos %= size;
    }
    return {false, '-'};
  }

  void SetCoeff() {
    static std::random_device rand;
    static std::mt19937 random_gen(rand());
    std::uniform_int_distribution<int> distro(1, kPrime - 1);
    coeff = distro(rand);
  }

  int64_t HashFunc(char key) const {
    int64_t res = ((key * coeff) % kPrime) % size;
    return res;
  }
};

void ModifyString(std::string& str) {
  Map table(str.size());
  int last_used_char = 0;
  for (size_t i = 0; i < str.size(); ++i) {
    Pair exists = table.CheckIfExists(str[i]);
    if (!exists.first) {
      table.Set(str[i], alphabet[last_used_char]);
      str[i] = alphabet[last_used_char];
      ++last_used_char;
    } else {
      str[i] = exists.second;
    }
  }
}

void Answer(int req) {
  InitAlphabet();
  Hash table(req);
  char request;
  std::string str;
  for (int i = 0; i < req; ++i) {
    std::cin >> request >> str;
    ModifyString(str);
    if (request == '+') {
      table.Set(str);
      continue;
    }
    if (request == '?') {
      bool exists = table.CheckIfExists(str);
      if (exists) {
        std::cout << "YES\n";
      } else {
        std::cout << "NO\n";
      }
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);

  int req;
  std::cin >> req;
  Answer(req);
}
