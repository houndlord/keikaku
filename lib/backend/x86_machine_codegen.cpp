#include "backend/x86_machine_codegen.hpp"
#include <vector>


std::vector<uint8_t> X86CodeGenerator::GenerateMachineCode(const std::shared_ptr<IRProgram>& ir_program) {
  std::vector<uint8_t> machine_code;

  for (const auto& ir_function : ir_program)
}