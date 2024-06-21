#include <beescript/type-check.h>

#include <beescript/errors.h>

#include <iostream>

#include <sstream>

namespace bees {

class TypeChecker
{
public:
    bool error;

    TypeChecker()
        : error(false)
    {
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
                std::cerr << "Operator " << getExprTypeName(node->type)
                          << " invalid for types " << lType->toStr() << " and "
                          << rType->toStr() << std::endl;
                out->primType = PrimitiveType::Invalid;
                error = true;
            }
            else
            {
                // Output must be int
                out->primType = lType->primType;
            }
            break;
        case ExprType::Neg:
            if(lType->primType != PrimitiveType::Int
               || lType->primType != PrimitiveType::Byte)
            {
                std::cerr << "Cannot negate type " << lType->toStr()
                          << std::endl;
                out->primType = PrimitiveType::Invalid;
                error = true;
            }
            else
            {
                out->primType = lType->primType;
            }

            break;

        case ExprType::LogAnd:
        case ExprType::LogOr:
            // types must be boolean
            if(lType->primType != PrimitiveType::Bool
               || rType->primType != PrimitiveType::Bool)
            {
                std::cerr << "Boolean operator " << getExprTypeName(node->type)
                          << " invalid for types " << lType->toStr() << " and "
                          << rType->toStr() << std::endl;

                out->primType = PrimitiveType::Invalid;
                error = true;
            }
            else
            {
                out->primType = PrimitiveType::Bool;
            }

            break;

        case ExprType::LogNot:
            if(lType->primType != PrimitiveType::Bool
               || lType->primType != PrimitiveType::Byte)
            {
                std::cerr << "Cannot negate type " << lType->toStr()
                          << std::endl;
                out->primType = PrimitiveType::Invalid;
                error = true;
            }
            else
            {
                out->primType = lType->primType;
            }
            break;

        case ExprType::Name:

            if(!node->symbol->type)
            {
                std::cerr << "Undeclared name: " << node->toStr() << std::endl;
                out->primType = PrimitiveType::Invalid;
                error = true;
            }
            else
            {
                // Output type is the type set in the name resolution step;
                out = node->symbol->type;
                out->symbol = node->symbol;
                out->ref = true;
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
                std::cerr << "Cannot compare type " << lType->toStr() << " and "
                          << rType->toStr() << std::endl;
                //out->primType = PrimitiveType::Invalid;
                error = true;
                break;
            }
            switch(lType->primType)
            {
            case PrimitiveType::Void:
            case PrimitiveType::Func:
            case PrimitiveType::Array:
            {
                std::cerr << "Cannot compare type " << lType->toStr()
                          << std::endl;
                //out->primType = PrimitiveType::Invalid;
                error = true;
                break;
            }
            default:
                break;
            }

            out->primType = PrimitiveType::Bool;
            break;

        case ExprType::Call:
        {
            if(lType->primType != PrimitiveType::Func)
            {
                std::cerr << "Cannot call non-func type " << lType->toStr()
                          << std::endl;
                out->primType = PrimitiveType::Invalid;
                error = true;
                break;
            }

            TypeNodePtr funcType = lType->symbol->type;
            ArgListPtr argType = funcType->args;
            ExprNodePtr arg = node->right;
            size_t argIdx = 0;

            bool argError = false;
            while(argType && arg)
            {
                TypeNodePtr temp = typeCheckExpr(arg->left);
                if(!argType->type->equals(temp))
                {
                    std::cerr << "Invalid argument type, expected "
                              << argType->type->toStr() << " got "
                              << temp->toStr() << std::endl;
                    out->primType = PrimitiveType::Invalid;
                    error = true;
                    argError = true;
                    break;
                }

                argType = std::static_pointer_cast<ArgList>(argType->next);
                arg = arg->right;
                ++argIdx;
            }

            if(argError)
            {
                break;
            }

            if(argType || arg)
            {
                if(argType)
                {
                    std::cerr << "Function call missing argument " << argIdx
                              << " (" << argType->toStr() << ")" << std::endl;
                }
                else
                {
                    std::cerr << "Too many arguments supplied to function call "
                              << node->toStr() << std::endl;
                }
                error = true;
                out->primType = PrimitiveType::Invalid;
                break;
            }

            // output is func return type
            out = funcType->subtype;
            break;
        }

        case ExprType::Subscript:
            if(lType->primType != PrimitiveType::Array
               && lType->primType != PrimitiveType::Str)
            {
                std::cerr << "Cannot subscript non-array type "
                          << lType->toStr() << std::endl;
                error = true;
                out->primType = PrimitiveType::Invalid;
                break;
            }
            if(rType->primType != PrimitiveType::Int
               && rType->primType != PrimitiveType::Byte)
            {
                std::cerr << "Invalid type as subscript index: "
                          << rType->toStr() << std::endl;
                error = true;
                out->primType = PrimitiveType::Invalid;
                break;
            }
            if(lType->primType == PrimitiveType::Array)
            {
                out = lType->subtype;
                out->ref = true;
            }
            else // is string
            {
                out->primType = PrimitiveType::Byte;
                out->ref = true;
            }

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

        case ExprType::LitFloat:
            out->primType = PrimitiveType::Float;
            break;

        case ExprType::LitArray:
        {
            out->primType = PrimitiveType::Array;
            out->subtype = nullptr;
            ExprNodePtr value = node->left;
            size_t numValues = 0;
            while(value)
            {
                ++numValues;
                TypeNodePtr t = typeCheckExpr(value->left);
                if(!out->subtype)
                {
                    out->subtype = t;
                }
                else if(!out->subtype->equals(t))
                {
                    std::cerr << "Invalid value in array literal, expected "
                              << out->subtype->toStr() << " got " << t->toStr()
                              << std::endl;
                    error = true;
                }

                value = value->right;
            }

            out->size = numValues;

            break;
        }

        case ExprType::Assign:
        {
            if(!lType->ref)
            {
                std::cerr << "Invalid assignment target, not a reference ("
                          << node->left->toStr() << ": " << lType->toStr()
                          << ") " << std::endl;
                error = true;
                out->primType = PrimitiveType::Invalid;
                break;
            }
            if(!lType->equals(rType))
            {
                std::cerr << "Cannot assign var (" << node->left->toStr()
                          << ": " << lType->toStr() << ") value ("
                          << node->right->toStr() << "): (" << rType->toStr()
                          << ")" << std::endl;
                error = true;
                out->primType = PrimitiveType::Invalid;
                break;
            }
            // Assignment does not have a value
            out->primType = PrimitiveType::Void;
            break;
        }
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
            case StmtType::Expr:
                typeCheckExpr(node->expr);
                break;
            case StmtType::For:
            {
                typeCheckExpr(node->initExpr);
                TypeNodePtr t = typeCheckExpr(node->expr);
                if(t->primType != PrimitiveType::Bool)
                {
                    std::cerr
                        << "Invalid for-loop check expr, expected bool, got "
                        << t->toStr() << std::endl;
                    error = true;
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
                    std::cerr << "Invalid if expr, expected bool, got "
                              << t->toStr() << " (" << node->expr->toStr()
                              << ")" << std::endl;
                    error = true;
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
                    std::cerr << "Invalid type for return, expected "
                              << retType->toStr() << " got " << t->toStr()
                              << std::endl;
                    error = true;
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
            std::cerr << "Cannot declare variable of type void" << std::endl;
            break;
        case PrimitiveType::Byte:
        case PrimitiveType::Int:
        case PrimitiveType::Str:
        case PrimitiveType::Bool:
        {
            auto valueType = typeCheckExpr(node->value);
            if(valueType && !node->type->equals(valueType))
            {
                std::cerr << "Cannot initialize var (" << node->toStr()
                          << ") with value of type " << valueType->toStr()
                          << std::endl;
                error = true;
                break;
            }

            break;
        }
        case PrimitiveType::Func:
            typeCheckStmt(node->code, node->type->subtype);
            break;
        case PrimitiveType::Array:
        {
            if(node->value)
            {
                TypeNodePtr t = typeCheckExpr(node->value);
                if(!node->type->equals(t))
                {
                    std::cerr << "Cannot initialize var (" << node->toStr()
                              << ") with type (" << t->toStr() << ")"
                              << std::endl;
                    error = true;
                    break;
                }
                if(node->type->size != 0 && node->type->size != t->size)
                {
                    std::cerr
                        << "Warning: initializing array (" << node->toStr()
                        << ") with different sized literal (" << t->toStr()
                        << ")" << std::endl;
                    // Not an error, just a warning
                }
            }
            break;
        }
        }

        // store the type in the symbol
        node->symbol->type = node->type;
    }
};

bool typeCheck(SyntaxNodePtr node)
{
    TypeChecker tc;
    // Root nodes will always be decl
    while(node)
    {
        if(node->nodeType != NodeType::Decl)
        {
            std::stringstream ss;
            ss << "Invalid node for typecheck: "
               << getNodeTypeName(node->nodeType);
            throw BeeError(ss.str());
        }

        tc.typeCheckDecl(std::static_pointer_cast<DeclNode>(node));
        node = node->next;
    }

    return !tc.error;
}

} //namespace bees