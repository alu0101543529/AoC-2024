/**
 * AoC 2024: Day 23: LAN Party

 * @file   day_23_lan_party.cc
 * @author Raúl Gonzalez Acosta
 * @date   23/12/2024
 * @brief  Determine the total number of sets and the password for the LAN party.
 * @see    https://adventofcode.com/2024/day/23
 */

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>

using Connections = std::unordered_map<std::string, std::unordered_set<std::string>>;
using Computers = std::unordered_set<std::string>;

void ParseInput(const std::string &filename, Connections &connections, Computers &computers) {
  std::ifstream inputFile(filename);

  if (!inputFile) {
    std::cerr << "Could not open file: " << filename << std::endl;
    return;
  }

  std::string line;
  while (inputFile >> line) {
    std::string c1{line.substr(0, 2)};
    std::string c2{line.substr(3)};
    computers.insert(c1);
    computers.insert(c2);
    connections[c1].insert(c2);
    connections[c2].insert(c1);
  }
}

template <typename T>
std::unordered_set<T> findIntersection(const std::unordered_set<T> &set1, const std::unordered_set<T> &set2) {
  std::unordered_set<T> intersection;

  const std::unordered_set<T> &smaller = (set1.size() < set2.size()) ? set1 : set2;
  const std::unordered_set<T> &larger = (set1.size() < set2.size()) ? set2 : set1;

  for (const T &element : smaller) {
    if (larger.find(element) != larger.end()) {
      intersection.insert(element);
    }
  }

  return intersection;
}

int numStartingWith(const Computers &computers, const char c = 't') {
  int n{0};
  for (const auto &com : computers) {
    n += com[0] == c;
  }

  return n;
}

int CalculateTotalSets(const Connections &connections, Computers computers) {
  long totalSets{0};
  auto it1 = computers.begin();
  while (it1 != computers.end()) {
    auto set1{findIntersection(connections.at(*it1), computers)};
    auto it2{set1.begin()};
    while (it2 != set1.end()) {
      auto set2{findIntersection(set1, connections.at(*it2))};
      if ((*it1)[0] == 't' || (*it2)[0] == 't') {
        totalSets += set2.size();
      } else {
        totalSets += numStartingWith(set2);
      }

      it2 = set1.erase(it2);
    }

    it1 = computers.erase(it1);
  }

  return totalSets;
}

Computers largestNetworkSize(Computers &computers, const Connections &connections, Computers &connectedSet) {
  if (computers.empty()) { return connectedSet; }

  Computers largestSet{connectedSet};
  auto it{computers.begin()};

  while (it != computers.end()) {
    connectedSet.insert(*it);
    auto set{findIntersection(computers, connections.at(*it))};
    auto resSet{largestNetworkSize(set, connections, connectedSet)};

    if (resSet.size() > largestSet.size()) { largestSet = resSet; }

    connectedSet.erase(*it);
    it = computers.erase(it);
  }

  return largestSet;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: solution.out <filename>\n";
    return 1;
  }

  std::string filename = argv[1];

  Connections connections;
  Computers computers;

  ParseInput(filename, connections, computers);
  int totalSets = CalculateTotalSets(connections, computers);
  std::cout << "Total sets: " << totalSets << std::endl;

  ParseInput(filename, connections, computers);
  Computers connectedSet;
  Computers largestSet{largestNetworkSize(computers, connections, connectedSet)};
  std::vector<std::string> password{largestSet.begin(), largestSet.end()};
  std::sort(password.begin(), password.end());

  std::cout << "Password: ";
  for (size_t i = 0; i < password.size(); i++) {
    std::cout << password[i];
    if (i == password.size() - 1) { std::cout << "."; } 
    else { std::cout << ","; }
  }

  std::cout << std::endl;

  return EXIT_SUCCESS;
}