#include <memory>
#include <string>
#include <vector>

#include "codegen/IR.hpp"


class MachineCodeGeneratorInterface {
 public:
  virtual ~MachineCodeGeneratorInterface() = default;

  virtual std::vector<uint8_t> GenerateMachineCode(const std::shared_ptr<IRProgram>& ir_program) = 0;
};