#include <syntax-node.h>

#include <sstream>
#include <stdexcept>

namespace bees {

ArgList::ArgList()
    : SyntaxNode(NodeType::ArgList)
{
}

TypeNode::TypeNode()
    : SyntaxNode(NodeType::Type)
{
}

std::string getPrimitiveTypeName(PrimitiveType type)
{
    switch(type)
    {
    case PrimitiveType::Int:
        return "Int";
    case PrimitiveType::Str:
        return "Str";
    case PrimitiveType::Bool:
        return "Bool";
    case PrimitiveType::Func:
        return "Func";
    case PrimitiveType::Array:
        return "Array";
    default:
        std::stringstream ss;
        ss << "Unknown PrimitiveType (" << static_cast<unsigned>(type) << ")";
        throw std::runtime_error(ss.str());
    }
}

} //namespace bees