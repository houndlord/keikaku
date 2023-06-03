#ifndef KEI_BACKEND_X86_MACHINE_CODEGEN_H
#define KEI_BACKEND_X86_MACHINE_CODEGEN_H


#include "machine_codegen_interface.hpp"


class X86CodeGenerator : public MachineCodeGeneratorInterface {
 public:
  X86CodeGenerator() = default;
  ~X86CodeGenerator() override = default;

  std::vector<uint8_t> GenerateMachineCode(const std::shared_ptr<IRProgram>& ir_program) override;
  std::vector<uint8_t> GenerateInstructionCode(const std::shared_ptr<IRInstruction>& instruction);
};
#endif