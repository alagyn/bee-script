#pragma once

#include <memory>

class SyntaxNode
{
public:
    std::shared_ptr<SyntaxNode> next;
};

class StmtNode;
class DeclNode;
class TypeNode;
class ExprNode;

using SyntaxNodePtr = std::shared_ptr<SyntaxNode>;
using DeclNodePtr = std::shared_ptr<DeclNode>;
using TypeNodePtr = std::shared_ptr<TypeNode>;
using StmtNodePtr = std::shared_ptr<StmtNode>;
using ExprNodePtr = std::shared_ptr<ExprNode>;

enum class Type
{
    Int,
    Str,
    Bool,
    Func,
    Array
};

class ArgList : public SyntaxNode
{
public:
    std::string name;
    TypeNodePtr type;
};

class TypeNode : public SyntaxNode
{
public:
    Type type;
    TypeNodePtr subtype;
    std::shared_ptr<ArgList> args;
};

enum class StmtType
{
    Decl,
    Expr,
    For,
    IfElse,
    Return,
    Block
};

class StmtNode : public SyntaxNode
{
public:
    StmtType kind;
    DeclNodePtr decl;
    ExprNodePtr initExpr;
    ExprNodePtr expr;
    ExprNodePtr nextExpr;
    StmtNodePtr body;
    StmtNodePtr elseBody;
};

enum class ExprType
{
    Error,

    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Pow,
    Neg,

    Name,
    Arg,

    LogAnd,
    LogOr,
    LogNot,
    LogEqu,
    LogNeq,

    CmpLT,
    CmpLTEQ,
    CmpGT,
    CmpGTEQ,

    Call,
    Subscript,

    LitInt,
    LitBool,
    LitStr
};

class ExprNode : public SyntaxNode
{
public:
    ExprType type;
    ExprNodePtr left, right;

    std::string name;
    int intValue;
    std::string strValue;

    ExprNode()
    {
    }

    ExprNode(ExprType type, SyntaxNodePtr left, SyntaxNodePtr right)
        : type(type)
        , left(std::static_pointer_cast<ExprNode>(left))
        , right(std::static_pointer_cast<ExprNode>(right))
    {
    }
};

class DeclNode : public SyntaxNode
{
public:
    std::string name;
    TypeNodePtr type;
    ExprNodePtr value;
    StmtNodePtr code;
};
