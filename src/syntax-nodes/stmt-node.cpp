#include <syntax-node.h>

#include <sstream>
#include <stdexcept>

namespace bees {

StmtNode::StmtNode(StmtType kind)
    : SyntaxNode(NodeType::Stmt)
    , kind(kind)
{
}

std::string getStmtTypeName(StmtType type)
{
    switch(type)
    {
    case StmtType::Decl:
        return "Decl";
    case StmtType::Expr:
        return "Expr";
    case StmtType::For:
        return "For";
    case StmtType::IfElse:
        return "IfElse";
    case StmtType::Return:
        return "Return";
    case StmtType::Block:
        return "Block";
    default:
        std::stringstream ss;
        ss << "Unknown StmtType (" << static_cast<unsigned>(type) << ")";
        throw std::runtime_error(ss.str());
    }
}

} //namespace bees