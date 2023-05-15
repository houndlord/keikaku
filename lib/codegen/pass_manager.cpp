#include "codegen/pass_manager.hpp"


void PassManager::AddPass(std::shared_ptr<Pass> pass) {
  passes_.push_back(pass);
}

void PassManager::RunPasses(std::shared_ptr<IRProgram> ir_program) {
  for (const auto& pass : passes_) {
    pass->Run(ir_program);
  }
}