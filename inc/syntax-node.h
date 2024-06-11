#pragma once

#include <memory>
#include <string>

enum class SymbolType
{
    Local,
    Global,
    Param
};

class TypeNode;
using TypeNodePtr = std::shared_ptr<TypeNode>;

class Symbol
{
public:
    SymbolType kind;
    TypeNodePtr type;
    std::string name;
    int which;
};

using SymbolPtr = std::shared_ptr<Symbol>;

class SyntaxNode
{
public:
    std::shared_ptr<SyntaxNode> next;
    std::shared_ptr<Symbol> symbol;
};

class StmtNode;
class DeclNode;

class ExprNode;

using SyntaxNodePtr = std::shared_ptr<SyntaxNode>;
using DeclNodePtr = std::shared_ptr<DeclNode>;
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

using ArgListPtr = std::shared_ptr<ArgList>;

class TypeNode : public SyntaxNode
{
public:
    Type type;
    TypeNodePtr subtype;
    ArgListPtr args;
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
    Assign,

    Name,
    Arg,

    LogAnd,
    LogOr,
    LogEqu,
    LogNeq,
    LogNot,

    CmpLT,
    CmpLTEQ,
    CmpGT,
    CmpGTEQ,

    Call,
    Subscript,

    LitInt,
    LitBool,
    LitStr,
    LitByte
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
