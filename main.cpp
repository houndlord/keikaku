#include <sstream>
#include <iostream>

#include "/home/houndlord/keikaku/lib/frontend/tokenizer.cpp"

int main() {
  std::stringstream s{"123"};
  Tokenizer tokenizer(&s);
  tokenizer.IsEnd();
  tokenizer.Next();
  auto token = tokenizer.GetToken();
  bool a;
  if (token == Token{ConstantToken{1}}) {
    a = true;
    std::cout<<"true";
    std::cout<<"yj";
  }
  std::cout << a;
  std::cout << "xxyj";
}