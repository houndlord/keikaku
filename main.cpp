#include <iostream>
#include <sstream>

#include "/home/houndlord/keikaku/lib/frontend/tokenizer.cpp"

int main() {
  std::stringstream s{"x"};
  Tokenizer tokenizer(&s);
  if (tokenizer.IsEnd() == false) {
    std::cout << "alex is awesome";
  }
  auto token = tokenizer.GetToken();
  std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, ConstantToken>) {
        std::cout << "ConstantToken with value " << arg.value << '\n';
    }
    else if constexpr (std::is_same_v<T, BracketToken>) {
        // Print something for BracketToken
    }
    else if constexpr (std::is_same_v<T, SymbolToken>) {
        std::cout << "ConstantToken with value " << arg.name << '\n';
    }
    else if constexpr (std::is_same_v<T, QuoteToken>) {
        // Print something for QuoteToken
    }
    else if constexpr (std::is_same_v<T, DotToken>) {
        // Print something for DotToken
    }
}, token);
  //tokenizer.Next();
  std::cout << "t";
  //auto token2 = tokenizer.GetToken();
  std::visit([](auto&& arg) {
    using T = std::decay_t<decltype(arg)>;
    if constexpr (std::is_same_v<T, ConstantToken>) {
        std::cout << "ConstantToken2 with value " << arg.value << '\n';
    }
    else if constexpr (std::is_same_v<T, BracketToken>) {
        // Print something for BracketToken
    }
    else if constexpr (std::is_same_v<T, SymbolToken>) {
        // Print something for SymbolToken
    }
    else if constexpr (std::is_same_v<T, QuoteToken>) {
        // Print something for QuoteToken
    }
    else if constexpr (std::is_same_v<T, DotToken>) {
        // Print something for DotToken
    }
}, token);
  bool a;
  if (token == Token{ConstantToken{2}}) {
    a = true;
    std::cout << "true";
    std::cout << "yj";
  }
  //std::cout << a;
  std::cout << "xxyj";
}