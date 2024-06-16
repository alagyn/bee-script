#include <beescript/syntax-node.h>

#include <beescript/errors.h>

#include <sstream>
#include <stdexcept>

namespace bees {

ExprNode::ExprNode()
    : SyntaxNode(NodeType::Expr)
{
}

ExprNode::ExprNode(ExprType type, SyntaxNodePtr left, SyntaxNodePtr right)
    : SyntaxNode(NodeType::Expr)
    , type(type)
    , left(std::static_pointer_cast<ExprNode>(left))
    , right(std::static_pointer_cast<ExprNode>(right))
{
}

std::string getExprTypeName(ExprType type)
{
    switch(type)
    {
    case ExprType::Error:
        return "Error";
    case ExprType::Add:
        return "Add";
    case ExprType::Sub:
        return "Sub";
    case ExprType::Mul:
        return "Mul";
    case ExprType::Div:
        return "Div";
    case ExprType::Mod:
        return "Mod";
    case ExprType::Pow:
        return "Pow";
    case ExprType::Neg:
        return "Neg";
    case ExprType::Assign:
        return "Assign";
    case ExprType::Name:
        return "Name";
    case ExprType::Arg:
        return "Arg";
    case ExprType::LogAnd:
        return "LogAnd";
    case ExprType::LogOr:
        return "LogOr";
    case ExprType::LogEqu:
        return "LogEqu";
    case ExprType::LogNeq:
        return "LogNeq";
    case ExprType::LogNot:
        return "LogNot";
    case ExprType::CmpLT:
        return "CmpLT";
    case ExprType::CmpLTEQ:
        return "CmpLTEQ";
    case ExprType::CmpGT:
        return "CmpGT";
    case ExprType::CmpGTEQ:
        return "CmpGTEQ";
    case ExprType::Call:
        return "Call";
    case ExprType::Subscript:
        return "Subscript";
    case ExprType::LitInt:
        return "LitInt";
    case ExprType::LitBool:
        return "LitBool";
    case ExprType::LitStr:
        return "LitStr";
    case ExprType::LitByte:
        return "LitByte";
    case ExprType::LitArray:
        return "LitArray";
    default:
        std::stringstream ss;
        ss << "Unknown ExprType (" << static_cast<unsigned>(type) << ")";
        throw BeeError(ss.str());
    }
}

} //namespace bees