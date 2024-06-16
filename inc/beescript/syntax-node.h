#pragma once

#include <memory>
#include <sstream>
#include <string>

#include <beescript/symbol.h>

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

    virtual std::string toStr() = 0;
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
    Invalid,
    Void,
    Byte,
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

    std::string toStr() override;
};

class TypeNode : public SyntaxNode
{
public:
    TypeNode();

    PrimitiveType primType;
    TypeNodePtr subtype;
    ArgListPtr args;
    size_t size;
    bool ref; // Flag if this value is assignable

    bool equals(const TypeNodePtr other);
    bool equals(const TypeNode& other);

    std::string toStr() override;
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
    ExprNodePtr initExpr; //for loop init
    ExprNodePtr expr; // for loop check, if check
    ExprNodePtr nextExpr; // for loop update
    StmtNodePtr body;
    StmtNodePtr elseBody;

    std::string toStr() override;
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
    LitByte,
    LitArray
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

    std::string toStr() override;
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

    std::string toStr() override;
};

} //namespace bees
