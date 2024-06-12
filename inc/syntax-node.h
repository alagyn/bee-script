#pragma once

#include <memory>
#include <string>

#include <symbol.h>

namespace bees {

enum class NodeType
{
    ArgList,
    Decl,
    Stmt,
    Expr,
    Type
};

std::string getNodeTypeName(NodeType type);

class SyntaxNode
{
public:
    const NodeType nodeType;
    SyntaxNode(NodeType nodeType);
    std::shared_ptr<SyntaxNode> next;
    std::shared_ptr<Symbol> symbol;
};

using SyntaxNodePtr = std::shared_ptr<SyntaxNode>;

class StmtNode;
class ExprNode;
class TypeNode;
class DeclNode;
class ArgList;

using TypeNodePtr = std::shared_ptr<TypeNode>;
using StmtNodePtr = std::shared_ptr<StmtNode>;
using ExprNodePtr = std::shared_ptr<ExprNode>;
using DeclNodePtr = std::shared_ptr<DeclNode>;
using ArgListPtr = std::shared_ptr<ArgList>;

// TYPE NODE -------------------------------------------------
enum class PrimitiveType
{
    Int,
    Str,
    Bool,
    Func,
    Array
};

std::string getPrimitiveTypeName(PrimitiveType type);

class ArgList : public SyntaxNode
{
public:
    ArgList();
    std::string name;
    TypeNodePtr type;
};

class TypeNode : public SyntaxNode
{
public:
    TypeNode();

    PrimitiveType type;
    TypeNodePtr subtype;
    ArgListPtr args;
};

// STMT NODE -------------------------------------------------
enum class StmtType
{
    Decl,
    Expr,
    For,
    IfElse,
    Return,
    Block
};

std::string getStmtTypeName(StmtType type);

class StmtNode : public SyntaxNode
{
public:
    StmtNode(StmtType kind);

    StmtType kind;
    DeclNodePtr decl;
    ExprNodePtr initExpr;
    ExprNodePtr expr;
    ExprNodePtr nextExpr;
    StmtNodePtr body;
    StmtNodePtr elseBody;
};

// EXPR NODE -------------------------------------------------
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

std::string getExprTypeName(ExprType type);

class ExprNode : public SyntaxNode
{
public:
    ExprType type;
    ExprNodePtr left, right;

    std::string name;
    int intValue;
    std::string strValue;

    ExprNode();
    ExprNode(ExprType type, SyntaxNodePtr left, SyntaxNodePtr right);
};

// DECL NODE -------------------------------------------------
class DeclNode : public SyntaxNode
{
public:
    DeclNode();

    std::string name;
    TypeNodePtr type;
    ExprNodePtr value;
    StmtNodePtr code;
};

} //namespace bees
