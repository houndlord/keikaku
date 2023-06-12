#include <cstddef>
#include <memory>
#include <string>
#include <utility>


#include "frontend/parser.hpp"


std::string SymbolNode::ToString(int indent) const {
  std::string indention(indent, ' ');
  return indention + "SYMBOL: " + value_;
}

std::string ListNode::ToString(int indent) const {
  std::string indention(indent, ' ');
  std::string result = indention + "List:\n";
  for (const auto& child : children_) {
    result += child->ToString(indent + 2) + "\n";
  }
  return result;
}

std::string NumberNode::ToString(int indent) const {
  std::string indention(indent, ' ');
  return indention + "NUMBER: " + value_;
}

AstNodePtr Read(Tokenizer& tokenizer, Token token) {
  if (std::holds_alternative<ConstantToken>(token)) {
    return std::make_shared<NumberNode>(
        std::to_string(std::get<ConstantToken>(token).value));
  } else if (std::holds_alternative<SymbolToken>(token)) {
    return std::make_shared<SymbolNode>(std::get<SymbolToken>(token).name);
  } else if (token == Token{BracketToken::LEFT}) {
    return ReadList(tokenizer).first;
  } else if (token == Token{BracketToken::RIGHT}) {
    throw std::runtime_error("Unexpected right bracket");
  }
}

std::pair<AstNodePtr, bool> ReadList(Tokenizer& tokenizer) { 
  auto node = std::make_shared<ListNode>();
  bool consume_next = true;

  while (!tokenizer.IsEnd()) {
    auto token = tokenizer.GetToken();
    if (std::holds_alternative<EOFToken>(token)) {
      break;
    }
    if (token == Token{BracketToken::RIGHT}) {
      consume_next = false;
      break;
    }
    node->children_.push_back(Read(tokenizer, token));
  }
  return {node, consume_next};
}

AstNodePtr Parse(Tokenizer& tokenizer) {
  auto root = std::make_shared<ListNode>();
  while (!tokenizer.IsEnd()) {
    auto token = tokenizer.GetToken();
    if (std::holds_alternative<EOFToken>(token)) {
      break;
    }
    bool consume_next = true;
    if (token == Token{BracketToken::LEFT}) {
      auto [node, should_consume_next] = ReadList(tokenizer);
      root->children_.push_back(node);
      consume_next = should_consume_next;
    } else if (std::holds_alternative<ConstantToken>(token) || std::holds_alternative<SymbolToken>(token)) {
      root->children_.push_back(Read(tokenizer, token));
    } else {
      throw std::runtime_error("Unexpected token");
    }
  }
  return root;
}

std::string ASTtoString(const AstNodePtr &ast) {
  if (auto symbolNode = std::dynamic_pointer_cast<SymbolNode>(ast)) {
    return symbolNode->ToString(0);
  } 
  else if (auto numberNode = std::dynamic_pointer_cast<NumberNode>(ast)) {
    return numberNode->ToString(0);
  } 
  else if (auto listNode = std::dynamic_pointer_cast<ListNode>(ast)) {
    return listNode->ToString(0);
  }
}