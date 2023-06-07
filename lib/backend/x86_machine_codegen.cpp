#include <vector>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <optional>
#include <list>


#include "backend/machine_description.hpp"
#include "backend/x86_machine_codegen.hpp"
#include "backend/x86_target.hpp"


Register RegistersPool::GetRegister() {
  if (allocations_ == registers_.size()) {
    //Spilling has to be implemented!
      throw;
  }
  for (auto it = registers_.begin(); it != registers_.end(); ++it) {
  if (it->second == 0) {
    // We found a first free Register {Register, 0}.
    allocations_++;
    it->second = 1;
    return it->first;
    }
  }
}

bool RegistersPool::FreeRegister() {
  for (auto it = registers_.begin(); it != registers_.end(); ++it) {
  if (it->second == 1) {
    // We found a first free Register {Register, 0}.
    allocations_--;
    it->second = 0;
    return true;
  }
}

void X86CodeGenerator::CalculateLiveRanges(const std::shared_ptr<IRProgram>& ir_program) {
  int opIndex = 0;  // This will be our timestamp
  for (const auto& function : ir_program->get_functions()) {
    for (const auto& basicBlock : function->get_basic_blocks()) {
      for (const auto& instruction : basicBlock->get_instructions()) {
        std::vector<std::string> usedVars = instruction->GetUsedVars();
        for (const auto& var : usedVars) {
          if (liveRanges_.count(var) > 0) {
            liveRanges_[var].end = opIndex;
          } else {
            liveRanges_[var] = {opIndex, opIndex};
          }
        }
        opIndex++;
      }
    }
  }
}

std::map<std::string, Register> X86CodeGenerator::PerformRegisterAllocation(const std::shared_ptr<IRProgram>& ir_program) {
  std::map<std::string, Register> register_allocations;
  // We'll use this set to keep track of what variables are currently in registers.
  std::set<std::string> active;
  RegistersPool pool;

  // Create a vector of live ranges and sort it.
  std::vector<std::pair<std::string, LiveRange>> liveRangesVec(liveRanges_.begin(), liveRanges_.end());
  std::sort(liveRangesVec.begin(), liveRangesVec.end(),
            [](const auto& a, const auto& b) { return a.second.start < b.second.start; });

  for (auto& [var, liveRange] : liveRangesVec) { 
    // If a variable's live range has ended, it can be removed from active,
    // and its register can be freed.
    for (auto it = active.begin(); it != active.end(); ) {
      if (liveRanges_[*it].end < liveRange.start) {
        pool.FreeRegister();
        it = active.erase(it);  // Note the erase() call. This correctly removes the item and advances the iterator.
      } else {
        ++it;  // If we didn't erase an item, just advance the iterator normally.
      }
    }

    Register reg = pool.GetRegister(); 
    register_allocations.insert({var, reg});
    active.insert(var);  // Add the variable to the active set.
  }

  return register_allocations;
}


std::vector<uint8_t> X86CodeGenerator::GenerateMachineCode(const std::shared_ptr<IRProgram>& ir_program) {
  std::vector<uint8_t> machine_code;
  auto register_allocations = PerformRegisterAllocation(ir_program);
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
