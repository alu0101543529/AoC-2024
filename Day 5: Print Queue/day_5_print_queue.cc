/**
 * AoC 2024: Day 5: Print Queue

 * @file   day_5_print_queue.cc
 * @author Raúl Gonzalez Acosta
 * @date   05/12/2024
 * @brief  Order print jobs based on dependencies and calculate the sum of middle pages.
 * @see    https://adventofcode.com/2024/day/5
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <queue>

using namespace std;

// Function to parse ordering rules from the input
unordered_map<int, unordered_set<int>> parseOrderingRules(const vector<string>& rules) {
  unordered_map<int, unordered_set<int>> ordering;
  for (const string& rule : rules) {
    int x, y;
    sscanf(rule.c_str(), "%d|%d", &x, &y);
    ordering[x].insert(y);
  }
  return ordering;
}

// Function to check if a given update is in the correct order
bool isUpdateCorrect(const vector<int>& update, const unordered_map<int, unordered_set<int>>& ordering) {
  unordered_map<int, int> index;
  for (size_t i = 0; i < update.size(); ++i) { index[update[i]] = i; }
  for (const auto& [x, ys] : ordering) {
    if (index.find(x) == index.end()) continue; // Skip if x is not in the update
    for (int y : ys) {
      if (index.find(y) == index.end()) { continue; } // Skip if y is not in the update
      if (index[x] > index[y]) { return false; }      // x must appear before y
    }
  }
  return true;
}

vector<int> reorderUpdate(const vector<int>& update, const unordered_map<int, unordered_set<int>>& ordering) {
  // Crear un grafo dirigido y un mapa de in-degrees
  unordered_map<int, int> inDegree;
  unordered_map<int, vector<int>> graph;
  
  // Inicializar nodos del grafo para las páginas del update
  for (int page : update) {
    inDegree[page] = 0;
    graph[page] = {};
  }

  // Construir el grafo solo con las páginas en el update
  for (int page : update) {
    if (ordering.count(page)) {
      for (int dependent : ordering.at(page)) {
        if (find(update.begin(), update.end(), dependent) != update.end()) {
          graph[page].push_back(dependent);
          inDegree[dependent]++;
        }
      }
    }
  }

  // Ordenamiento topológico usando Kahn's Algorithm
  queue<int> q;
  vector<int> sortedUpdate;

  // Agregar nodos con in-degree 0 a la cola
  for (const auto& [page, degree] : inDegree) {
    if (degree == 0) { q.push(page); }
  }

  while (!q.empty()) {
    int current = q.front();
    q.pop();
    sortedUpdate.push_back(current);

    for (int neighbor : graph[current]) {
      inDegree[neighbor]--;
      if (inDegree[neighbor] == 0) { q.push(neighbor); }
    }
  }

  // Si no se pudieron ordenar todas las páginas, hay un ciclo o inconsistencia
  if (sortedUpdate.size() != update.size()) { return {}; }

  return sortedUpdate;
}

// Main program logic
int main(int argc, char* argv[]) {
  // Check if the input file is provided
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file> [-trace]" << std::endl;
    return EXIT_FAILURE;
  }

  // Check if the -trace option is provided
  bool trace = false;
  if (argc > 2 && std::string(argv[2]) == "-trace") { trace = true; }

  ifstream inputFile(argv[1]);
  if (!inputFile) {
    cerr << "Error: Could not open file " << argv[1] << endl;
    return EXIT_FAILURE;
  }

  vector<string> rules;
  vector<vector<int>> updates;
  string line;

  // Read ordering rules
  while (getline(inputFile, line) && !line.empty()) { rules.push_back(line); }

  // Read updates
  while (getline(inputFile, line)) {
    stringstream ss(line);
    vector<int> update;
    int page;
    while (ss >> page) {
      update.push_back(page);
      if (ss.peek() == ',') { ss.ignore(); }
    }
    updates.push_back(update);
  }

  inputFile.close();

  // Parse ordering rules
  auto ordering = parseOrderingRules(rules);

  // Phase 1: Process updates and calculate the sum of middle pages
  int sumOfMiddlePages = 0;
  for (const auto& update : updates) {
      if (isUpdateCorrect(update, ordering)) {
        if (trace) {
          cout << "Correct update: ";
          for (int page : update) cout << page << " ";
          cout << endl;
        }

        // Find the middle page
        int middlePage = update[update.size() / 2];
        sumOfMiddlePages += middlePage;
      }
  }

  cout << "Sum of middle pages: " << sumOfMiddlePages << endl;

  if (trace) {
    cout << endl << "--------------------------------" << endl << endl;
  }

  // Phase 2: Process updates and calculate the sum of middle pages for reordered updates
  int sumOfReorderedMiddlePages = 0;
  for (const auto& update : updates) {
      if (!isUpdateCorrect(update, ordering)) {
        // Reordenar la actualización incorrecta
        auto reorderedUpdate = reorderUpdate(update, ordering);
        if (reorderedUpdate.empty()) { continue; }

        if (trace) {
          if (reorderedUpdate.empty()) {
            cerr << "Skipping reorder due to inconsistency.\n";
          }

          // Imprimir la actualización reordenada
          cout << "Reordered update: ";
          for (int page : reorderedUpdate) cout << page << " ";
          cout << endl;
        }

        // Encontrar la página central
        int middlePage = reorderedUpdate[reorderedUpdate.size() / 2];
        sumOfReorderedMiddlePages += middlePage;
      }
  }

  cout << "Sum of middle pages (reordered updates): " << sumOfReorderedMiddlePages << endl;

  return EXIT_SUCCESS;
}
