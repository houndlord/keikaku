#include <memory>
#include <vector>
#include <string>

#include "Tokenizer.hpp"

class AstNode {
public:
    virtual ~AstNode() = default;
};

typedef std::shared_ptr<AstNode> AstNodePtr;

class SymbolNode : public AstNode {
public:
    explicit SymbolNode(const std::string& value) : value(value) {}

    std::string value;
};

class NumberNode : public AstNode {
public:
    explicit NumberNode(const std::string& value) : value(value) {}

    std::string value;
};

class ListNode : public AstNode {
public:
    explicit ListNode(const std::vector<AstNodePtr>& children = {}) : children(children) {}

    std::vector<AstNodePtr> children;
};

AstNodePtr parse(std::vector<Token>& tokens);
