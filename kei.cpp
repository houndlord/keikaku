#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>


#include "frontend/parser.hpp"
#include "frontend/tokenizer.hpp"
#include "codegen/ir_visitor.hpp"

void print_help() {
    std::cout << "Usage: kei [-p] -i <input_file> -o [<output_file>]\n";
    std::cout << "Options:\n";
     std::cout << "  -p\t\tPrint IR to console\n";
    std::cout << "  -i <file>\tInput file containing Scheme code\n";
    std::cout << "  -o <file>\tOutput file for IR\n";
    std::cout << "  -h\t\tShow this help text\n";
}

int main(int argc, char* argv[]) {
  std::ifstream inputFile;
  std::ofstream outputFile;
  std::string inputFileName;
  std::string outputFileName;
  bool printToConsole = false;

  int opt;
  while ((opt = getopt(argc, argv, "i:o:pvh")) != -1) {
    switch (opt) {
      case 'i':
        inputFileName = optarg;
        break;
      case 'o':
        outputFileName = optarg;
        break;
      case 'p':
        printToConsole = true;
        break;
      case 'h':
        print_help();
        return 0;
      default:
        print_help();
        return 1;
      }
    }

  if (inputFileName.empty()) {
    std::cerr << "Input file is required\n";
    print_help();
    return 1;
  }

  inputFile.open(inputFileName);
  if (!inputFile) {
    std::cerr << "Unable to open input file: " << inputFileName << "\n";
    return 1;
  }

  if (!outputFileName.empty()) {
    outputFile.open(outputFileName);
    if (!outputFile) {
      std::cerr << "Unable to open output file: " << outputFileName << "\n";
      return 1;
    }
  }

  Tokenizer tokenizer(&inputFile);
  auto ast = Parse(tokenizer);
  auto visitor = std::make_shared<AstToIrVisitor>();
  auto a = visitor->GenerateIR(ast);
  if (printToConsole) {
    std::cout << a->Print();
  }
  if (outputFile.is_open()) {
    std::string out = a->Print();
    // Open file
    std::ofstream outputFile("output_filename.txt");
    // Check if file opened correctly
    if (!outputFile) {
        std::cerr << "Unable to open file for writing\n";
        return 1; // Or another appropriate error handling
    }
    // Write to file
    outputFile << out;
    // Close file
    outputFile.close();
  }
  return 0;
}