

#include "codegen/IR.hpp"


std::vector<std::string> Sub::GetUsedVars() const {
  return {src1_, src2_};
}

Branch::Branch(const std::shared_ptr<IRValue>& condition,
               const std::shared_ptr<IRBasicBlock>& true_target,
               const std::shared_ptr<IRBasicBlock>& false_target)
    : condition_(condition), true_target_(true_target), false_target_(false_target) {}
Branch::Branch(const Branch& other)
    : condition_(other.condition_), true_target_(other.true_target_), false_target_(other.false_target_) {}

Branch::Branch(Branch&& other) noexcept
    : condition_(std::move(other.condition_)), true_target_(std::move(other.true_target_)), false_target_(std::move(other.false_target_)) {}

const std::shared_ptr<IRBasicBlock>& Branch::GetTrueTarget() const {
  return true_target_;
}

const std::shared_ptr<IRBasicBlock>& Branch::GetFalseTarget() const {
  return false_target_;
}

// Phi class constructors and destructor
Phi::Phi(const std::string& dest,
         const std::shared_ptr<IRValue>& true_value,
         const std::shared_ptr<IRValue>& false_value)
    : dest_(dest), true_value_(true_value), false_value_(false_value) {}

Phi::Phi(const Phi& other)
    : dest_(other.dest_), true_value_(other.true_value_), false_value_(other.false_value_) {}

Phi::Phi(Phi&& other) noexcept
    : dest_(std::move(other.dest_)), true_value_(std::move(other.true_value_)), false_value_(std::move(other.false_value_)) {}


Jump::Jump(const std::shared_ptr<IRBasicBlock>& target) : target_(target) {}

const std::shared_ptr<IRBasicBlock>& Jump::GetTarget() const {
  return target_;
}

std::string IRProgram::GetNextTempVar() {
  return "temp_" + std::to_string(temp_var_counter_++);
}

void IRProgram::add_function(const std::shared_ptr<IRFunction>& function) {
  functions_.push_back(function);
};

const std::vector<std::shared_ptr<IRFunction>>& IRProgram::get_functions() const {
  return functions_;
};