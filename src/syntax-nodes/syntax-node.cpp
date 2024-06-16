#include <beescript/syntax-node.h>

#include <beescript/errors.h>

#include <sstream>
#include <stdexcept>

namespace bees {

SyntaxNode::SyntaxNode(NodeType nodeType)
    : nodeType(nodeType)
{
}

std::string getNodeTypeName(NodeType type)
{
    switch(type)
    {
    case NodeType::ArgList:
        return "ArgList";
    case NodeType::Decl:
        return "Decl";
    case NodeType::Stmt:
        return "Stmt";
    case NodeType::Expr:
        return "Expr";
    case NodeType::Type:
        return "Type";
    default:
        std::stringstream ss;
        ss << "Unknown NodeType (" << static_cast<unsigned>(type) << ")";
        throw BeeError(ss.str());
    }
}

} //namespace bees