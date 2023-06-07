#ifndef KEI_BACKEND_X86_MACHINE_CODEGEN_H
#define KEI_BACKEND_X86_MACHINE_CODEGEN_H


#include <map>


#include "machine_codegen_interface.hpp"
#include "backend/machine_description.hpp"
#include "backend/x86_target.hpp"
#include "codegen/IR.hpp"


struct LiveRange {
  int start;
  int end;
};

struct RegistersPool {
 public:
  explicit RegistersPool() : allocations_(0) {
    for (const auto& reg : x86MachineDesc.GetRegisters()) {
      if (reg.GetType() == RegisterType::GENERAL_PURPOSE) {
        registers_.insert({reg, 0});
      }
   }
  };

  bool is_empty_;
  Register GetRegister();
  bool FreeRegister();
 private:
  std::map<Register, int> registers_;
  size_t allocations_;
}

class X86CodeGenerator : public MachineCodeGeneratorInterface {
 public:
  X86CodeGenerator() = default;
  ~X86CodeGenerator() override = default;

  std::vector<uint8_t> GenerateMachineCode(const std::shared_ptr<IRProgram>& ir_program) override;
  std::vector<uint8_t> GenerateInstructionCode(const std::shared_ptr<IRInstruction>& instruction);
 private:
  void CalculateLiveRanges(const std::shared_ptr<IRProgram>& ir_program);
  std::map<std::string, Register> PerformRegisterAllocation(const std::shared_ptr<IRProgram>& ir_program);

  std::map<std::string, LiveRange> liveRanges_;
};
#endif