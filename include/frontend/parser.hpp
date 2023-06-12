#ifndef KEI_FRONTEND_PARSER_H
#define KEI_FRONTEND_PARSER_H


#include <memory>
#include <vector>
#include <string>


#include "tokenizer.hpp"

    
class AstNode {
 public:
	virtual ~AstNode() = default;

	virtual std::string ToString(int indent) const = 0;
};

typedef std::shared_ptr<AstNode> AstNodePtr;

class SymbolNode : public AstNode {
 public:
	explicit SymbolNode(const std::string& value) : value_(value) {}

	std::string ToString(int indent) const override;

  std::string value_;
};

class NumberNode : public AstNode {
 public:
  explicit NumberNode(const std::string& value) : value_(value) {}

	std::string ToString(int indent) const override;

  std::string value_;
};

class ListNode : public AstNode {
 public:
  explicit ListNode(const std::vector<AstNodePtr>& children = {}) : children_(children) {}

	std::string ToString(int indent) const override;

  std::vector<AstNodePtr> children_;
};

AstNodePtr Parse(Tokenizer& tokenizer);
std::pair<AstNodePtr, bool> ReadList(Tokenizer& tokenizer);
AstNodePtr Read(Tokenizer& tokenizer, Token token);
std::string ASTtoString(const AstNodePtr& ast);
#endif