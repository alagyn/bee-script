#include <beescript/syntax-node.h>

namespace bees {

DeclNode::DeclNode()
    : SyntaxNode(NodeType::Decl)
{
}

std::string DeclNode::toStr()
{
    std::stringstream ss;
    ss << name << ": " << type->toStr();
    return ss.str();
}

} //namespace bees