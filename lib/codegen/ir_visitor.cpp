#include "codegen/ir_visitor.hpp"
#include <iostream>


// Constructor for constant values
IRValue::IRValue(int constant_value) : type_(IRValueType::Constant), value_(constant_value) {
  std::cerr << "Created a Constant IRValue with value: " << constant_value << std::endl;
}

// Constructor for variable and temporary values
IRValue::IRValue(const std::string& name, IRValueType type) : type_(type), value_(name) {
  std::cerr << "Created a " << (type == IRValueType::Variable ? "Variable" : "Temporary") << " IRValue with name: " << name << std::endl;
}

IRValueType IRValue::GetType() const {
  return type_;
}

int IRValue::GetConstantValue() const {
  if (std::holds_alternative<int>(value_)) {
    return std::get<int>(value_);
  } else {
    throw std::runtime_error("Attempted to get constant value, but IRValue holds a string.");
  }
}

const std::string& IRValue::GetName() const {
  if (std::holds_alternative<std::string>(value_)) {
    return std::get<std::string>(value_);
  } else {
    throw std::runtime_error("Attempted to get name, but IRValue holds an int.");
  }
}


std::shared_ptr<IRProgram> AstToIrVisitor::GenerateIR(const AstNodePtr& ast) {
  ir_program_ = std::make_shared<IRProgram>();

  // Create a "main" function and set it as the current function
  current_function_ = std::make_shared<IRFunction>("main", std::vector<std::string>());
  ir_program_->add_function(current_function_);

  // Create an entry basic block for the "main" function
  current_basic_block_ = std::make_shared<IRBasicBlock>();
  current_function_->add_basic_block(current_basic_block_);

  // Visit the AST
  Visit(ast);

  return ir_program_;
}

void AstToIrVisitor::GenerateConditionalJump(
  const std::shared_ptr<IRValue>& condition,
  const std::shared_ptr<IRBasicBlock>& true_block,
  const std::shared_ptr<IRBasicBlock>& false_block) {
  std::string condition_name;


  
  // Create a Branch instruction with the condition name and target blocks
  auto branch = std::make_shared<Branch>(condition, true_block, false_block);
  
  // Add the Branch instruction to the current basic block
  current_basic_block_->add_instruction(branch);
}

std::shared_ptr<IRValue> AstToIrVisitor::GeneratePhiNode(
  const std::shared_ptr<IRValue>& true_value,
  const std::shared_ptr<IRValue>& false_value,
  const std::shared_ptr<IRBasicBlock>& merge_block) {
  // Create a temporary variable to store the result of the Phi node
  std::string temp_var_name = ir_program_->GetNextTempVar();
  auto temp_var = std::make_shared<IRValue>(temp_var_name, IRValueType::Temporary);

  // Create the Phi instruction
  auto phi_instr = std::make_shared<Phi>(
  temp_var_name, true_value, false_value);

  // Add the Phi instruction to the merge_block
  merge_block->add_instruction(phi_instr);

  // Return the temporary variable that holds the result of the Phi node
  return temp_var;
}

void AstToIrVisitor::VisitSymbol(const std::shared_ptr<SymbolNode>& node) {
  if (environment_.IsVariable(node->value_)) {
    // If the symbol is a variable, create an IRValue for it and set it as the last value.
    last_value_ = std::make_shared<IRValue>(node->value_, IRValueType::Variable);
  } else if (environment_.IsFunction(node->value_)) {
    // If the symbol is a function, create a function call IR instruction.
    // Note that this assumes the arguments for the function call have been processed and stored elsewhere.
    // You may need to adjust this based on your actual implementation.
    std::vector<AstNodePtr> args; // Assuming you have the arguments for the function call
    GenerateFunctionCall(node->value_, args);
  } else {
    // Handle other cases or report an error if the symbol is not recognized.
  }
}

/*
void AstToIrVisitor::VisitSymbol(const std::shared_ptr<SymbolNode>& node) {
  // Assuming 'node->value' is the name of the symbol (variable or function)
  std::string symbol_name = node->value;

  // Create a new temporary variable to hold the symbol's value
  std::string temp_var = ir_program_->GetNextTempVar();

  // Generate a load instruction that loads the symbol's value into the temporary variable
  auto load_instr = std::make_shared<IRLoadInstruction>(temp_var, symbol_name);

  // Add the load instruction to the current basic block
  current_basic_block_->AddInstruction(load_instr);
}
*/

void AstToIrVisitor::VisitNumber(const std::shared_ptr<NumberNode>& node) {
  std::cerr << "VisitNumber is Called\n";

  // Create a new IRValue to hold this number
  auto number_value = std::make_shared<IRValue>(std::stoi(node->value_));

  // Log that we've created a new constant
  std::cerr << "Visit number visited following:" << number_value<<std::endl; 

  // Save this as the last value we've visited
  last_value_ = number_value;
}


    
void AstToIrVisitor::VisitList(const std::shared_ptr<ListNode>& node) {
  std::cerr << "VisitList is Called" << std::endl;
  if (node->children_.empty()) {
    // Empty list, do nothing
    return;
  }
  // If the first node is a list, visit it recursively
  if (auto inner_list_node = std::dynamic_pointer_cast<ListNode>(node->children_[0])) {
    VisitList(inner_list_node);
    return;
  }
  for (const auto& child_node : node->children_) {
  if (auto symbol_node = std::dynamic_pointer_cast<SymbolNode>(child_node)) {
    std::cerr << "ListNode child is SymbolNode with value: " << symbol_node->value_ << std::endl;
  } else if (auto number_node = std::dynamic_pointer_cast<NumberNode>(child_node)) {
    std::cerr << "ListNode child is NumberNode with value: " << number_node->value_ << std::endl;
  } else if (auto inner_list_node = std::dynamic_pointer_cast<ListNode>(child_node)) {
    std::cerr << "ListNode child is ListNode with size: " << inner_list_node->children_.size() << std::endl;
  } else {
    std::cerr << "ListNode child is of unknown type." << std::endl;
  }
  }


  auto first_node = node->children_[0];
  auto first_symbol_node = std::dynamic_pointer_cast<SymbolNode>(first_node);
  
  if (!first_symbol_node) {
    std::cerr << "First node is not a symbol." << std::endl;
    return;
  }
  
  std::cerr << "First symbol node value: " << first_symbol_node->value_ << std::endl;


  if (!first_symbol_node) {
    // Handle the error: the first element of the list is not a symbol
    return;
  }

  if (IsSpecialForm(first_symbol_node->value_)) {
    // Handle special forms (e.g., 'let', 'if', 'lambda', etc.)
    if (first_symbol_node->value_ == "let") {
      if (node->children_.size() < 3) {
        // Handle the error: not enough children for 'let'
        return;
      }

      auto bindings_node = std::dynamic_pointer_cast<ListNode>(node->children_[1]);
      if (!bindings_node) {
        // Handle the error: bindings is not a list
        return;
      }

      for (const auto& binding : bindings_node->children_) {
        auto binding_pair = std::dynamic_pointer_cast<ListNode>(binding);
        if (!binding_pair || binding_pair->children_.size() != 2) {
          // Handle the error: binding is not a pair
          return;
        }

        auto var_name_node = std::dynamic_pointer_cast<SymbolNode>(binding_pair->children_[0]);
        if (!var_name_node) {
          // Handle the error: variable name is not a symbol
          return;
        }

        // Generate IR value for the expression
        Visit(binding_pair->children_[1]);

        // Add the new variable to the symbol table with the corresponding IR value
        environment_.AddVariable(var_name_node->value_);
      }

      // Visit the body of the 'let' expression
      Visit(node->children_[2]);
    // Handle other special forms here (e.g., 'if', 'lambda', etc.)
  } else if (first_symbol_node->value_ == "define") {
    // Handle define special form
    auto function_name_node = std::dynamic_pointer_cast<SymbolNode>(node->children_[1]);
    auto function_name = function_name_node->value_;

    auto arg_list_node = std::dynamic_pointer_cast<ListNode>(node->children_[2]);
    std::vector<std::string> arg_names;
    for (const auto& arg_node : arg_list_node->children_) {
      arg_names.push_back(std::dynamic_pointer_cast<SymbolNode>(arg_node)->value_);
    }

    auto function_body_node = node->children_[3];

    // Create a new IR function and set it as the current function
    current_function_ = std::make_shared<IRFunction>(function_name, arg_names);
    ir_program_->add_function(current_function_);

    // Create an entry basic block for the function
    current_basic_block_ = std::make_shared<IRBasicBlock>();
    current_function_->add_basic_block(current_basic_block_);

    // Visit the function body
    Visit(function_body_node);

  } else if (first_symbol_node->value_ == "if") {
    if (node->children_.size() != 4) {
      throw std::runtime_error("if special form expects 3 arguments: condition, true_expression, and false_expression");
    }

    auto condition_node = node->children_.at(1);
    auto true_expr_node = node->children_.at(2);
    auto false_expr_node = node->children_.at(3);

    Visit(condition_node);
    auto condition_value = last_value_;

    auto true_block = std::make_shared<IRBasicBlock>();
    auto false_block = std::make_shared<IRBasicBlock>();
    auto merge_block = std::make_shared<IRBasicBlock>();

    GenerateConditionalJump(condition_value, true_block, false_block);

    current_basic_block_ = true_block;
    Visit(true_expr_node);
    auto true_value = last_value_;
    true_block->add_instruction(std::make_shared<Jump>(merge_block));

    current_basic_block_ = false_block;
    Visit(false_expr_node);
    auto false_value = last_value_;
    false_block->add_instruction(std::make_shared<Jump>(merge_block));

    current_basic_block_ = merge_block;
    last_value_ = GeneratePhiNode(true_value, false_value, merge_block);

    current_function_->add_basic_block(true_block);
    current_function_->add_basic_block(false_block);
    current_function_->add_basic_block(merge_block);

    return;
  } else if (first_symbol_node->value_ == "lambda") {
    if (node->children_.size() != 3) {
      // Error: lambda requires exactly two arguments (parameters and body).
      // Handle the error according to your error handling strategy.
      return;
    }

    auto params_node = std::dynamic_pointer_cast<ListNode>(node->children_[1]);
    auto body_node = node->children_[2];

    if (!params_node) {
      // Error: lambda parameters should be a list.
      // Handle the error according to your error handling strategy.
      return;
    }

    // Extract parameter names from the params_node.
    std::vector<std::string> param_names;
    for (const auto& param : params_node->children_) {
      if (auto param_symbol = std::dynamic_pointer_cast<SymbolNode>(param)) {
        param_names.push_back(param_symbol->value_);
      } else {
        // Error: invalid parameter type in lambda.
        // Handle the error according to your error handling strategy.
        return;
      }
    }

    // Create a new IRFunction for the lambda function.
    std::string lambda_name = "lambda_" + std::to_string(ir_program_->get_functions().size());
    auto lambda_function = std::make_shared<IRFunction>(lambda_name, param_names);

    // Save the current function and basic block, and set the new ones.
    auto prev_function = current_function_;
    auto prev_basic_block = current_basic_block_;
    current_function_ = lambda_function;
    current_basic_block_ = std::make_shared<IRBasicBlock>();

    // Visit the body of the lambda function.
    Visit(body_node);

    // Restore the previous function and basic block.
    current_function_ = prev_function;
    current_basic_block_ = prev_basic_block;

    // Add the lambda function to the IRProgram.
    ir_program_->add_function(lambda_function);

    // Set the last_value_ to a reference to the newly created lambda function.
    last_value_ = std::make_shared<IRValue>(lambda_name, IRValueType::Variable);
    return;
    }
  } else {
  // Not a special form, assume it's a function call
  std::vector<AstNodePtr> args(node->children_.begin() + 1, node->children_.end());
  GenerateFunctionCall(first_symbol_node->value_, args);
}
std::cerr << "Exiting VisitList" << std::endl;
}

void AstToIrVisitor::Visit(const AstNodePtr& node) {
  std::cerr << "Visit is Called" << std::endl;
  if (auto symbol_node = std::dynamic_pointer_cast<SymbolNode>(node)) {
    std::cerr << "Visit visiting SymbolNode " << std::endl;
    VisitSymbol(symbol_node);
  } else if (auto number_node = std::dynamic_pointer_cast<NumberNode>(node)) {
    std::cerr << "Visit visiting NumberNode " << std::endl;
    VisitNumber(number_node);
  } else if (auto list_node = std::dynamic_pointer_cast<ListNode>(node)) {
    std::cerr << "Visit visiting ListNode " << std::endl;
    VisitList(list_node);
  } else {
    // Handle error: unexpected AST node type
  }
}

void AstToIrVisitor::GenerateFunctionCall(const std::string& function_name, const std::vector<AstNodePtr>& args) {
  std::cerr << "Generate FCall is Called" << std::endl;

  // Convert AST nodes to IR values
  std::vector<std::shared_ptr<IRValue>> ir_args;
  for (const auto& arg : args) {
    Visit(arg);  // assuming this updates last_value_
    ir_args.push_back(last_value_);
  }

  std::cerr << "Function name: " << function_name << std::endl;
  std::cerr << "Function arguments: " << std::endl;
  for(const auto& ir_arg: ir_args) {
    std::cerr << "Arg type: " << static_cast<int>(ir_arg->GetType()) << std::endl;
    if(ir_arg->GetType() == IRValueType::Constant) {
      std::cerr << "Arg value: " << ir_arg->GetConstantValue() << std::endl;
    } else if (ir_arg->GetType() == IRValueType::Variable || ir_arg->GetType() == IRValueType::Temporary) {
      std::cerr << "Arg name: " << ir_arg->GetName() << std::endl;
    }
  }

  // Create IRFunctionCall object
  auto func_call = std::make_shared<IRFunctionCall>(function_name, ir_args);

  // Add this function call to your current basic block
  current_basic_block_->add_instruction(func_call);

  // Update the last_instruction_ to this function call if you're going to use its result later
  //last_instruction_ = func_call;
}