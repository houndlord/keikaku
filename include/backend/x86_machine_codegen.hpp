#include "machine_codegen_interface.hpp"


class X86CodeGenerator : public MachineCodeGeneratorInterface {
 public:
  X86CodeGenerator() = default;
  ~X86CodeGenerator() override = default;

  std::vector<uint8_t> GenerateMachineCode(const std::shared_ptr<IRProgram>& ir_program) override;
};