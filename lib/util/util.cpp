#include "util/util.hpp"


bool IsSpecialForm(const std::string& symbol) {
  static const std::set<std::string> special_forms = {
    "lambda", "if", "define", "quote", "set!", "begin", "cond"
    // Add more special forms as needed
  };

  return special_forms.find(symbol) != special_forms.end();
}

bool Environment::IsVariable(const std::string& name) const {
  auto it = symbols_.find(name);
  if (it == symbols_.end()) {
    return false;
  }
  return it->second.type == SymbolType::Variable;
}

bool Environment::IsFunction(const std::string& name) const {
  auto it = symbols_.find(name);
  if (it == symbols_.end()) {
    return false;
  }
  return it->second.type == SymbolType::Function;
}

void Environment::AddVariable(const std::string& name) {
  symbols_[name] = {SymbolType::Variable};
}