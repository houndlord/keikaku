#include <memory>
#include <vector>
#include <string>

#include "tokenizer.hpp"

    
class AstNode {
public:
    virtual ~AstNode() = default;
};

typedef std::shared_ptr<AstNode> AstNodePtr;

class SymbolNode : public AstNode {
public:
    explicit SymbolNode(const std::string& value) : value_(value) {}

    std::string value_;
};

class NumberNode : public AstNode {
public:
    explicit NumberNode(const std::string& value) : value_(value) {}

    std::string value_;
};

class ListNode : public AstNode {
public:
    explicit ListNode(const std::vector<AstNodePtr>& children = {}) : children_(children) {}

    std::vector<AstNodePtr> children_;
};

AstNodePtr Parse(Tokenizer& tokenizer);
AstNodePtr ReadList(Tokenizer& tokenizer);
AstNodePtr Read(Tokenizer& tokenizer, Token token);
