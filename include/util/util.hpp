#include <set>
#include <string>

bool IsSpecialForm(const std::string& symbol);


#include <map>
#include <string>

enum class SymbolType {
  Variable,
  Function
};

class SymbolInfo {
public:
  SymbolType type;
};

class Environment {
 public:
  bool IsVariable(const std::string& name) const;

  bool IsFunction(const std::string& name) const;

  void AddVariable(const std::string& name);

  void AddFunction(const std::string& name) {
    symbols_[name] = {SymbolType::Function};
  }

 private:
  std::map<std::string, SymbolInfo> symbols_;
};
