#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

using namespace std;

vector<string> parseFile(string fileName) {
  ifstream file(fileName);
  string line;
  vector<string> codes;

  while(getline(file, line)) { codes.push_back(line); }

  file.close();
  return codes;
}

class Pather {
 public:
  Pather(int dirpadLayers) {
    this->dirpadLayers = dirpadLayers + 1;
    for (int i = 0; i < dirpad.size(); i++) {
      for (int j = 0; j < dirpad[i].size(); j++) {
          if (dirpad[i][j] != ' ') { directions[dirpad[i][j]] = {i, j}; }
      }
    }

    for (int i = 0; i < numpad.size(); i++) {
      for (int j = 0; j < numpad[i].size(); j++) {
          if (numpad[i][j] != ' ') { digits[numpad[i][j]] = {i, j}; }
      }
    }
  }

  uint64_t shortestPathForCodes(vector<string> codes) {
    uint64_t sum = 0;
    for (auto code : codes) { sum += shortestPathForCode(code) * stoi(code.substr(0, 3)); }
    return sum;
  }

 private:
  int dirpadLayers;
  vector<string> numpad = {"789", "456", "123", " 0A"}, dirpad = {" ^A", "<v>"};
  map<char, pair<int, int>> digits, directions;
  map<pair<pair<char, char>, int>, uint64_t> cache; // Memoizes the shortest path between two points at a given layer

  vector<string> pathsBetweenTwoKeys(vector<string> grid, pair<int, int> a, pair<int, int> b) {
    if (!grid[a.first][a.second] || grid[a.first][a.second] == ' ')
        return {};
    if (a == b)
        return {"A"};
    vector<string> paths;
    if (a.first > b.first)
        for (auto path : pathsBetweenTwoKeys(grid, {a.first - 1, a.second}, b))
            paths.push_back("^" + path);
    if (a.first < b.first)
        for (auto path : pathsBetweenTwoKeys(grid, {a.first + 1, a.second}, b))
            paths.push_back("v" + path);
    if (a.second > b.second)
        for (auto path : pathsBetweenTwoKeys(grid, {a.first, a.second - 1}, b))
            paths.push_back("<" + path);
    if (a.second < b.second)
        for (auto path : pathsBetweenTwoKeys(grid, {a.first, a.second + 1}, b))
            paths.push_back(">" + path);
    return paths;
  }

  uint64_t shortestPathAtLayer(char a, char b, int layer) {
    if (layer == dirpadLayers)
      return 1; // Press upper layer button
    pair<pair<char, char>, int> key = pair<pair<char, char>, int>{{a, b}, layer};
    if (cache.find(key) != cache.end())
      return cache[key];
    uint64_t shortestPath = UINT64_MAX;
    auto paths = (layer == 0 ? pathsBetweenTwoKeys(numpad, digits[a], digits[b]) : pathsBetweenTwoKeys(dirpad, directions[a], directions[b]));
    for (string path : paths) {
      uint64_t sum = shortestPathAtLayer('A', path[0], layer + 1);
      for (int i=0; i<path.size()-1; i++)
        sum += shortestPathAtLayer(path[i], path[i+1], layer + 1);
      shortestPath = min(shortestPath, sum);
    }
    cache[key] = shortestPath;
    return shortestPath;
  }

  uint64_t shortestPathForCode(string code) {
    uint64_t sum = shortestPathAtLayer('A', code[0], 0);
    for (int i=0; i<code.size()-1; i++)
      sum += shortestPathAtLayer(code[i], code[i+1], 0);
    return sum;
  }
};

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <input_file>" << endl;
    return EXIT_FAILURE;
  }

  string filename = argv[1];

  auto codes = parseFile(filename);
  cout << "Part 1: " << Pather(2).shortestPathForCodes(codes) << endl;
  cout << "Part 2: " << Pather(25).shortestPathForCodes(codes) << endl;

  return EXIT_SUCCESS;
}
