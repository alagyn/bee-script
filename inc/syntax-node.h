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

class ArgList : SyntaxNode
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
    ExprNode expr;
    ExprNodePtr nextExpr;
    StmtNodePtr body;
    StmtNodePtr elseBody;
};

class ExprNode : public SyntaxNode
{
};

class DeclNode : public SyntaxNode
{
public:
    std::string name;
    TypeNodePtr type;
    ExprNodePtr value;
    StmtNodePtr code;
};
