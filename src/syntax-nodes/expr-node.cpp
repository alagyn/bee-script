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

std::string ExprNode::toStr()
{
    std::stringstream ss;

    // Single arg expr
    switch(type)
    {
    case ExprType::Neg:
        ss << "-" << left->toStr();
        return ss.str();
    case ExprType::LogNot:
        ss << "not " << left->toStr();
        return ss.str();
    case ExprType::Name:
        return name;
    case ExprType::LitInt:
        ss << intValue;
        return ss.str();
    case ExprType::LitBool:
        ss << (intValue ? "true" : "false");
        return ss.str();
    case ExprType::LitStr:
        ss << "\"" << strValue << "\"";
        return ss.str();
    case ExprType::LitByte:
        ss << intValue;
        return ss.str();
    case ExprType::LitArray:
    {
        ss << "[";
        ExprNodePtr temp = right;
        while(temp)
        {
            ss << temp->toStr();
            temp = std::static_pointer_cast<ExprNode>(temp->next);
            if(temp)
            {
                ss << ", ";
            }
        }

        ss << "]";
        return ss.str();
    }
    case ExprType::Call:
    {
        ss << left->toStr() << "(";
        ExprNodePtr temp = right;
        while(temp)
        {
            ss << temp->toStr();
            temp = std::static_pointer_cast<ExprNode>(temp->next);
            if(temp)
            {
                ss << ", ";
            }
        }
        ss << ")";
        return ss.str();
    }
    case ExprType::Subscript:
    {
        ss << left->toStr() << "[" << right->toStr() << "]";
        return ss.str();
    }
    }

    ss << left->toStr();
    switch(type)
    {
    case ExprType::Add:
        ss << " + ";
        break;
    case ExprType::Sub:
        ss << " - ";
        break;
    case ExprType::Mul:
        ss << " * ";
        break;
    case ExprType::Div:
        ss << " / ";
        break;
    case ExprType::Mod:
        ss << " % ";
        break;
    case ExprType::Pow:
        ss << " ** ";
        break;
    case ExprType::Assign:
        ss << " = ";
        break;
    case ExprType::CmpLT:
        ss << " < ";
        break;
    case ExprType::CmpLTEQ:
        ss << " <= ";
        break;
    case ExprType::CmpGT:
        ss << " > ";
        break;
    case ExprType::CmpGTEQ:
        ss << " >= ";
        break;
    case ExprType::LogAnd:
        ss << " and ";
        break;
    case ExprType::LogOr:
        ss << " or ";
        break;
    case ExprType::LogEqu:
        ss << " == ";
        break;
    case ExprType::LogNeq:
        ss << " != ";
        break;
    }

    ss << right->toStr();
    return ss.str();
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