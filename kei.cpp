#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <getopt.h>


#include "frontend/parser.hpp"
#include "frontend/tokenizer.hpp"
#include "codegen/ir_visitor.hpp"


void print_help() {
  std::cout << "Welcome to Kei, Scheme compiler. Work in progress. Only frontend is considered to be fully working now.\n";
  std::cout << "Usage: kei [-p] [-e] [-h] -i <input_file> [-o <output_file>] [-c <code_input>]\n";
  std::cout << "Options:\n";
  std::cout << "  -p, --emit-ir\t\tEmit IR and print it to console\n";
  std::cout << "  -e, --emit-ast\tEmit AST instead of IR\n";
  std::cout << "  -h, --help\t\tShow this help text\n";
  std::cout << "  -i, --input <file>\tInput file containing Scheme code\n";
  std::cout << "  -o, --output <file>\tOutput file for the result. If not provided, the output will be printed to console\n";
  std::cout << "  -c, --code <code>\tCode input as a string. If both -c and -i options are provided, -c will be used\n";
}

void PrintToConsole(const std::string& string) {
  std::cout << string;
  return;
}

void PrintToFile(const std::string& string, const std::string& outputFileName) {
  // Open file
  std::ofstream outputFile(outputFileName);

  // Check if file opened correctly
  if (!outputFile) {
    std::cerr << "Unable to open file for writing\n";
    return;
  }

  // Write to file
  outputFile << string;

  // Close file
  outputFile.close();
}


int main(int argc, char* argv[]) {
  std::ifstream inputFile;
  std::ofstream outputFile;
  std::string inputFileName;
  std::string outputFileName = "out";
  bool printToConsole = false;
  std::string codeInput;
  bool emitAST = false;

  static struct option long_options[] = {
    {"emit-ir", no_argument, 0, 'p'},
    {"emit-ast", no_argument, 0, 'e'},
    {"help", no_argument, 0, 'h'},
    {"input", required_argument, 0, 'i'},
    {"output", required_argument, 0, 'o'},
    {"code", required_argument, 0, 'c'},
    {0, 0, 0, 0}
  };

  int opt, long_index = 0;
  while ((opt = getopt_long(argc, argv, "ipc:o:eh", long_options, &long_index)) != -1) {
  switch (opt) {
    case 'i':
      if (optarg != NULL) inputFileName = optarg;
      break;
    case 'o':
      if (optarg != NULL) outputFileName = optarg;
      break;
    case 'p':
      printToConsole = true;
      break;
    case 'c':
      if (optarg != NULL) codeInput = optarg;
      break;
    case 'h':
      print_help();
      return 0;
    case 'e':
      emitAST = true;
      break;
    default:
      print_help();
      return 1;
    }
  }

std::unique_ptr<Tokenizer> tokenizer;

if (!codeInput.empty()) {
  std::istringstream codeStream(codeInput);
  tokenizer = std::make_unique<Tokenizer>(&codeStream);
} else if (!inputFileName.empty()) {
  inputFile.open(inputFileName);
  if (!inputFile) {
    std::cerr << "Unable to open input file: " << inputFileName << "\n";
    return 1;
  }
  tokenizer = std::make_unique<Tokenizer>(&inputFile);
}

if (!tokenizer) {
  std::cerr << "Tokenizer is not initialized. Please provide either -i or -c option.\n";
  return 1;
}

  auto ast = Parse(*tokenizer);

  if (emitAST) {
    std::string output = ASTtoString(ast);
    if (printToConsole) {
      std::cout << output;
    } else {
      PrintToFile(output, outputFileName);
    }
  } else {
    auto visitor = std::make_shared<AstToIrVisitor>();
    auto a = visitor->GenerateIR(ast);
    if (printToConsole) {
      auto b = a->Print();
      PrintToConsole(b);
    } else if (outputFile.is_open()) {
      std::string out = a->Print();
      // Open file
    } 
  }
  return 0;
}
