#include <beescript/type-check.h>

#include <beescript/errors.h>

#include <sstream>

namespace bees {

TypeNodePtr typeCheckExpr(ExprNodePtr node);
void typeCheckStmt(StmtNodePtr node, TypeNodePtr retType);
void typeCheckDecl(DeclNodePtr node);

void typeCheck(SyntaxNodePtr node)
{
    while(node)
    {
        if(node->nodeType != NodeType::Decl)
        {
            std::stringstream ss;
            ss << "Invalid node for typecheck: "
               << getNodeTypeName(node->nodeType);
            throw BeeError(ss.str());
        }

        typeCheckDecl(std::static_pointer_cast<DeclNode>(node));
        node = node->next;
    }
}

TypeNodePtr typeCheckExpr(ExprNodePtr node)
{
    if(!node)
    {
        return nullptr;
    }

    auto lType = typeCheckExpr(node->left);
    auto rType = typeCheckExpr(node->right);

    auto out = std::make_shared<TypeNode>();

    // TODO errors needs location
    // TODO errors replace getPrimitiveTypeName with something smarter to build
    // complex types
    // TODO errors need to not raise exceptions, return an invalid error type

    switch(node->type)
    {
    case ExprType::Add:
        if(lType->primType == PrimitiveType::Str)
        {
            // String concat
            // TODO rhs checks?
            out->primType = PrimitiveType::Str;
            break;
        }
        // Else fall through
    case ExprType::Sub:
    case ExprType::Div:
    case ExprType::Mul:
    case ExprType::Mod:
    case ExprType::Pow:
        // TODO more arithmetic types
        if(lType->primType != PrimitiveType::Int
           || rType->primType != PrimitiveType::Int)
        {
            std::stringstream ss;
            ss << "Operator " << getExprTypeName(node->type)

               << " invalid for types " << lType->toStr() << " and "
               << rType->toStr();
            throw BeeError(ss.str());
        }
        // Output must be int
        out->primType = lType->primType;
        break;
    case ExprType::Neg:
        if(lType->primType != PrimitiveType::Int
           || lType->primType != PrimitiveType::Byte)
        {
            std::stringstream ss;
            ss << "Cannot negate type " << lType->toStr();
            throw BeeError(ss.str());
        }

        out->primType = lType->primType;
        break;

    case ExprType::LogAnd:
    case ExprType::LogOr:
        // types must be boolean
        if(lType->primType != PrimitiveType::Bool
           || rType->primType != PrimitiveType::Bool)
        {
            std::stringstream ss;
            ss << "Boolean operator " << getExprTypeName(node->type)
               << " invalid for types " << lType->toStr() << " and "
               << rType->toStr();
            throw BeeError(ss.str());
        }

        out->primType = PrimitiveType::Bool;
        break;

    case ExprType::LogNot:
        if(lType->primType != PrimitiveType::Bool
           || lType->primType != PrimitiveType::Byte)
        {
            std::stringstream ss;
            ss << "Cannot negate type " << lType->toStr();
            throw BeeError(ss.str());
        }
        out->primType = lType->primType;
        break;

    case ExprType::Name:
        // Output type is the type set in the name resolution step;
        out = node->symbol->type;
        if(!out)
        {
            std::stringstream ss;
            ss << "Name has unknown type: " << node->toStr();
            throw BeeError(ss.str());
        }
        break;

    case ExprType::CmpLT:
    case ExprType::CmpLTEQ:
    case ExprType::CmpGT:
    case ExprType::CmpGTEQ:
    case ExprType::LogEqu:
    case ExprType::LogNeq:
        if(!lType->equals(rType))
        {
            std::stringstream ss;
            ss << "Cannot compare type " << lType->toStr() << " and "
               << rType->toStr();
            throw BeeError(ss.str());
        }
        switch(lType->primType)
        {
        case PrimitiveType::Void:
        case PrimitiveType::Func:
        case PrimitiveType::Array:
        {
            std::stringstream ss;
            ss << "Invalid compare type " << lType->toStr();
            throw BeeError(ss.str());
        }
        }

        out->primType = PrimitiveType::Bool;
        break;

    case ExprType::Call:
    {
        if(lType->primType != PrimitiveType::Func)
        {
            std::stringstream ss;
            ss << "Cannot call non-func type " << lType->toStr();
            throw BeeError(ss.str());
        }

        TypeNodePtr funcType = lType->symbol->type;
        ArgListPtr argType = funcType->args;
        ExprNodePtr arg = node->right;

        while(argType && arg)
        {
            TypeNodePtr temp = typeCheckExpr(arg->left);
            if(!argType->type->equals(temp))
            {
                std::stringstream ss;
                ss << "Invalid argument type, expected "
                   << argType->type->toStr() << " got " << temp->toStr();
                throw BeeError(ss.str());
            }

            argType = std::static_pointer_cast<ArgList>(argType->next);
            arg = arg->right;
        }

        if(argType || arg)
        {
            // TODO
            throw BeeError("Arguments do not match function definition");
        }

        // output is func return type
        out = funcType->subtype;
        break;
    }

    case ExprType::Subscript:
        if(lType->primType != PrimitiveType::Array)
        {
            std::stringstream ss;
            ss << "Cannot subscript non-array type " << lType->toStr();
            throw BeeError(ss.str());
        }
        if(rType->primType != PrimitiveType::Int
           || rType->primType != PrimitiveType::Byte)
        {
            std::stringstream ss;
            ss << "Invalid type as subscript index: " << rType->toStr();
            throw BeeError(ss.str());
        }
        out = lType->subtype;
        break;

    case ExprType::LitInt:
        out->primType = PrimitiveType::Int;
        break;

    case ExprType::LitBool:
        out->primType = PrimitiveType::Bool;
        break;

    case ExprType::LitStr:
        out->primType = PrimitiveType::Str;
        break;

    case ExprType::LitByte:
        out->primType = PrimitiveType::Byte;
        break;

    case ExprType::Assign:
        // Assignment does not have a value
        out->primType = PrimitiveType::Void;
        break;
    }

    return out;
}

void typeCheckStmt(StmtNodePtr node, TypeNodePtr retType)
{
    while(node)
    {
        switch(node->kind)
        {
        case StmtType::Decl:
            typeCheckDecl(node->decl);
            break;
        case StmtType::Block:
            typeCheckStmt(node->body, retType);
            break;
        case StmtType::For:
        {
            typeCheckExpr(node->initExpr);
            TypeNodePtr t = typeCheckExpr(node->expr);
            if(t->primType != PrimitiveType::Bool)
            {
                std::stringstream ss;
                ss << "Invalid for-loop check expr, expected bool, got "
                   << t->toStr();
                throw BeeError(ss.str());
            }
            typeCheckExpr(node->nextExpr);
            typeCheckStmt(node->body, retType);
            break;
        }
        case StmtType::IfElse:
        {
            TypeNodePtr t = typeCheckExpr(node->expr);
            if(t->primType != PrimitiveType::Bool)
            {
                std::stringstream ss;
                ss << "Invalid if expr, expected Bool, got " << t->toStr();
                throw BeeError(ss.str());
            }

            typeCheckStmt(node->body, retType);
            typeCheckStmt(node->elseBody, retType);
            break;
        }
        case StmtType::Return:
        {
            TypeNodePtr t = typeCheckExpr(node->expr);
            if(!retType->equals(t))
            {
                std::stringstream ss;
                ss << "Invalid type for return, expected " << retType->toStr()
                   << " got " << t->toStr();
                throw BeeError(ss.str());
            }
        }
        }

        node = std::static_pointer_cast<StmtNode>(node->next);
    }
}

void typeCheckDecl(DeclNodePtr node)
{
    switch(node->type->primType)
    {
    case PrimitiveType::Void:
        // TODO
        throw BeeError("Cannot make void variable");
    case PrimitiveType::Byte:
    case PrimitiveType::Int:
    case PrimitiveType::Str:
    case PrimitiveType::Bool:
    {
        auto valueType = typeCheckExpr(node->value);
        if(valueType && !node->type->equals(valueType))
        {
            std::stringstream ss;
            ss << "Cannot initialize var of type "
               << getPrimitiveTypeName(node->type->primType)
               << " with value of type "
               << getPrimitiveTypeName(valueType->primType);
            throw BeeError(ss.str());
        }

        // store the type in the symbol
        node->symbol->type = valueType;

        break;
    }
    case PrimitiveType::Func:
        typeCheckStmt(node->code, node->type->subtype);
        break;
    case PrimitiveType::Array:
    {
        if(node->value->type != ExprType::Arg)
        {
            // TODO
            throw BeeError("value is not array");
        }

        ExprNodePtr value = node->value;
        while(value)
        {
            TypeNodePtr t = typeCheckExpr(value->left);
            if(!node->type->equals(t))
            {
                std::stringstream ss;
                ss << "Invalid value in array literal, expected "
                   << getPrimitiveTypeName(node->type->primType) << " got"
                   << getPrimitiveTypeName(t->primType);
                throw BeeError(ss.str());
            }

            value = value->right;
        }
        break;
    }
    }
}

} //namespace bees