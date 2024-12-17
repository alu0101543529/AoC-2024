/**
 * AoC 2024: Day 17: Chronospatial Computer

 * @file   day_17_chronospatial_computer.cc
 * @author Raúl Gonzalez Acosta
 * @date   17/12/2024
 * @brief  Determines given register values and program, the output of a Chronospatial Computer.
 * @see    https://adventofcode.com/2024/day/17
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

// Function to parse register values and program from file
std::vector<long long> readProgramAndRegisters(const std::string& filename, long long& regA, long long& regB, long long& regC) {
  std::vector<long long> program;
  std::ifstream file(filename);
  if (!file) {
      std::cerr << "Error: Cannot open file " << filename << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string line;

  // Read line containing registers
  while (std::getline(file, line)) {
    if (line.find("Register A") != std::string::npos) {
      std::istringstream iss(line);
      std::string dummy;
      char colon;
      iss >> dummy >> dummy >> colon >> regA; // Skip "Register A:"
      iss >> dummy >> colon >> regB;          // Skip "Register B:"
      iss >> dummy >> colon >> regC;          // Skip "Register C:"
    }

    // Read line containing the program
    if (line.find("Program:") != std::string::npos) {
      std::istringstream iss(line.substr(8)); // Skip "Program: "
      int value;
      char separator;
      while (iss >> value) {
        program.push_back(value);
        iss >> separator; // Skip commas
      }
    }
  }

  return program;
}

int getComboValue(int operand, long long regA, long long regB, long long regC) {
  if (operand >= 0 && operand <= 3) return operand; // Literal values 0-3
  else if (operand == 4) return regA; // Register A
  else if (operand == 5) return regB; // Register B
  else if (operand == 6) return regC; // Register C
  return -1; // Reserved operand 7
}

std::string runProgram(std::vector<long long> program, long long regA, long long regB, long long regC) {
  long long ip = 0; // Instruction pointer
  std::vector<long long> registers = {regA, regB, regC};
  std::ostringstream output;

  while (ip < program.size()) {
    int opcode = program[ip];
    long long operand = program[ip + 1];
    ip += 2;

    switch (opcode) {
      case 0: {
        long long divisor = pow(2, getComboValue(operand, registers[0], registers[1], registers[2]));
        if (divisor != 0) registers[0] /= divisor;
        break;
      }
      case 1: {
        registers[1] ^= operand;
        break;
      }
      case 2: {
        registers[1] = getComboValue(operand, registers[0], registers[1], registers[2]) % 8;
        break;
      }
      case 3: {
        if (registers[0] != 0) {
          ip = operand;
          continue;
        }
        break;
      }
      case 4: {
        registers[1] ^= registers[2];
        break;
      }
      case 5: {
        long long value = getComboValue(operand, registers[0], registers[1], registers[2]) % 8;
        if (output.str().empty()) output << value;
        else output << "," << value;
        break;
      }
      case 6: {
        long long divisor = pow(2, getComboValue(operand, registers[0], registers[1], registers[2]));
        if (divisor != 0) registers[1] = registers[0] / divisor;
        break;
      }
      case 7: {
        long long divisor = pow(2, getComboValue(operand, registers[0], registers[1], registers[2]));
        if (divisor != 0) registers[2] = registers[0] / divisor;
        break;
      }
      default: {
        std::cerr << "Invalid opcode encountered: " << opcode << std::endl;
        return "";
      }
    }
  }

  return output.str();
}

// New function to find the lowest positive A that outputs the program
long long findValidInitialA(const std::vector<long long>& program, long long regB, long long regC) {
  for (long long testA = 1;; ++testA) {
    std::string output = runProgram(program, testA, regB, regC);

    // Convert program to a comma-separated string
    std::ostringstream programStr;
    for (size_t i = 0; i < program.size(); ++i) {
      if (i > 0) programStr << ",";
      programStr << program[i];
    }

    if (output == programStr.str()) {
      std::cout << "Matching A found: " << testA << std::endl;
      return testA;
    }
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    return EXIT_FAILURE;
  }

  // Parse registers and program
  long long A = 0, B = 0, C = 0;
  std::vector<long long> program = readProgramAndRegisters(argv[1], A, B, C);

  // Phase 1: Run with given A
  std::string output = runProgram(program, A, B, C);
  std::cout << "Output: " << output << std::endl;

  // Phase 2: Find correct A
  long long validA = findValidInitialA(program, B, C);
  std::cout << "Lowest Valid Register A: " << validA << std::endl;

  return EXIT_SUCCESS;
}
