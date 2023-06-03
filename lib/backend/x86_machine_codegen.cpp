#include "backend/x86_machine_codegen.hpp"
#include "codegen/IR.hpp"
#include <vector>


std::vector<uint8_t> X86CodeGenerator::GenerateMachineCode(const std::shared_ptr<IRProgram>& ir_program) {
  std::vector<uint8_t> machine_code;
  // Iterate over each function in the IR program
  for (const auto& function : ir_program->get_functions()) {
    // Process each basic block in the function
    for (const auto& basic_block : function->get_basic_blocks()) {
      // Process each instruction in the basic block
      for (const auto& instruction : basic_block->get_instructions()) {
        std::vector<uint8_t> instructionCode = GenerateInstructionCode(instruction);
        machine_code.insert(machine_code.end(), instructionCode.begin(), instructionCode.end());
      }
    }
  }
  return machine_code;
}

std::vector<uint8_t> X86CodeGenerator::GenerateInstructionCode(const std::shared_ptr<IRInstruction>& instruction) {
  std::vector<uint8_t> instructionCode;
  if (auto addInstruction = std::dynamic_pointer_cast<Add>(instruction)) {
    // Generate machine code for Add instruction
    // Example: mov eax, [operand1]
    //          add eax, [operand2]
    //          mov [result], eax

    // Generate the machine code for mov eax, [operand1]
    instructionCode.push_back(0x8B);  // mov opcode
    instructionCode.push_back(0x08);  // displacement 8 bytes
    // ...

    // Generate the machine code for add eax, [operand2]
    instructionCode.push_back(0x03);  // add opcode
    instructionCode.push_back(0x10);  // displacement 16 bytes
    // ...

    // Generate the machine code for mov [result], eax
    instructionCode.push_back(0x89);  // mov opcode
    instructionCode.push_back(0x18);  // displacement 24 bytes
    // ...
  }
  return instructionCode;
};
