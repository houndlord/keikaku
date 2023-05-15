#include <vector>
#include <memory>

#include "IR.hpp"

class Pass {
 public:
  virtual ~Pass() = default;

  // This function will be implemented by each pass to perform its specific task.
  virtual void Run(std::shared_ptr<IRProgram> ir_program) = 0;
};



class PassManager {
 public:
  void AddPass(std::shared_ptr<Pass> pass);

  void RunPasses(std::shared_ptr<IRProgram> ir_program);

 private:
  std::vector<std::shared_ptr<Pass>> passes_;
};
