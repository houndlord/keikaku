#include "codegen/IR.hpp"
#include <iostream>


const std::string& Add::get_dest() const {
  return dest_;
}

const std::string& Add::get_src1() const {
  return src1_;
}

const std::string& Add::get_src2() const {
  return src2_;
}

std::vector<std::string> Add::GetUsedVars() const {
  return {src1_, src2_};
}

std::string  Add::Print() const {
  std::cerr << "In Add::Print" << std::endl;
  return "  Add " + get_src1() + ", " + get_src2() + "\n" + "  return " + get_dest();
}

const std::string& Sub::get_dest() const {
  return dest_;
}

const std::string& Sub::get_src1() const {
  return src1_;
}

const std::string& Sub::get_src2() const {
  return src2_;
}

std::vector<std::string> Sub::GetUsedVars() const {
  return {src1_, src2_};
}

std::string  Sub::Print() const {
  return "  Add " + get_src1() + ", " + get_src2() + "\n" + "  return " + get_dest();
}

const std::string& Mul::get_dest() const {
  return dest_;
}

const std::string& Mul::get_src1() const {
  return src1_;
}

const std::string& Mul::get_src2() const {
  return src2_;
}

std::vector<std::string> Mul::GetUsedVars() const {
  return {src1_, src2_};
}

std::string  Mul::Print() const {
  return"";
}

const std::string& Div::get_dest() const {
  return dest_;
}

const std::string& Div::get_src1() const {
  return src1_;
}

const std::string& Div::get_src2() const {
  return src2_;
}

std::vector<std::string> Div::GetUsedVars() const {
  return {src1_, src2_};
}

std::string  Div::Print() const {
  return"";
}

std::string  Phi::Print() const {
  return"";
}

std::vector<std::string> Phi::GetUsedVars() const {
  std::vector<std::string> vars;
  vars.push_back(true_value_->GetName());
  vars.push_back(false_value_->GetName());
  return vars;
}

void IRBasicBlock::add_instruction(const std::shared_ptr<IRInstruction>& instruction) {
  instructions_.push_back(instruction);
}

const std::vector<std::shared_ptr<IRInstruction>>& IRBasicBlock::get_instructions() const {
  return instructions_;
}

std::string  Jump::Print() const {
  return"";
}

std::vector<std::string> Jump::GetUsedVars() const {
  return {}; // Jump does not use any variables
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

std::vector<std::string> Branch::GetUsedVars() const {
  return {}; // Jump does not use any variables
}

std::string  Branch::Print() const {
  return"";
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

std::vector<std::string> IRFunctionCall::GetUsedVars() const {
  std::vector<std::string> vars;
  for (const auto& arg : args_) {
    vars.push_back(arg->GetName());
  }
  return vars;
}

std::string IRFunctionCall::Print() const {
  std::cerr << "Printing IRFCall" << std::endl;
  std::string result = "  call " + function_name_ + "(";
  for (size_t i = 0; i < args_.size(); ++i) {
    if (i != 0) {
      result += ", ";
    }
    std::cerr << "Printing VAL"<<args_[i] << std::endl;
    if (args_[i]->GetType() == IRValueType::Constant) {
      result += std::to_string(args_[i]->GetConstantValue());
    } else if (args_[i]->GetType() == IRValueType::Variable) {
      result += args_[i]->GetName();
    } else if (args_[i]->GetType() == IRValueType::Temporary) {
      // Handle temporary IRValues here
      // ...
    } else {
      // Handle other types here, or throw an error if an unexpected type is encountered
      // ...
    }
  }

  result += ")";
  return result;
}


IRFunction::IRFunction(const std::string& name, const std::vector<std::string>& parameters)
    : name_(name), parameters_(parameters) {
}

const std::shared_ptr<IRBasicBlock>& Jump::GetTarget() const {
  return target_;
}

const std::string& IRFunction::get_name() const {
  return name_;
}

const std::vector<std::string>& IRFunction::get_parameters() const {
  return parameters_;
}

void IRFunction::add_basic_block(const std::shared_ptr<IRBasicBlock>& basic_block) {
  basic_blocks_.push_back(basic_block);
}

const std::vector<std::shared_ptr<IRBasicBlock>>& IRFunction::get_basic_blocks() const {
  return basic_blocks_;
}

std::string IRFunction::Print() const {
  std::cerr << "Printing IR function" << std::endl;
  std::string output;
  for  (const auto& basic_block : get_basic_blocks()) {
    output += "Basic block: ";
    std::cerr << "Printing IR basic block. Number of instructions: " << basic_block->get_instructions().size() << std::endl;
    for (const auto& instruction : basic_block->get_instructions()) {
      output += instruction->Print();
    }
  }
  return output;
} 

std::string IRProgram::GetNextTempVar() {
  return "temp_" + std::to_string(temp_var_counter_++);
}

void IRProgram::add_function(const std::shared_ptr<IRFunction>& function) {
  std::cerr << "Adding a function to the program" << std::endl;
  functions_.push_back(function);
}

const std::vector<std::shared_ptr<IRFunction>>& IRProgram::get_functions() const {
  return functions_;
}

std::string IRProgram::Print() const {
  std::cerr << "In IRProgram::Print" << std::endl;
  std::cerr << "Number of functions: " << functions_.size() << std::endl;
  std::string output;
  for (const auto& function : functions_) {
    output += function->Print();
  }
  return output;
}