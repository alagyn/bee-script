#pragma once

#include <memory>
#include <string>

namespace bees {

enum class DAGType
{
    Assign,
    Name,

    Add,
    Sub,
    Mul,
    Div,
    Mod,
    Pow,
    Neg,

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

    LitBool,
    LitByte,
    LitInt,
    LitFloat,
    LitStr,
    LitArray
};

class DAGNode
{
public:
    DAGType type;

    union
    {
        const std::string name;
        double floatVal;
        int intVal;
    } value;

    std::shared_ptr<DAGNode> left, right;
};

using DAGNodePtr = std::shared_ptr<DAGNode>;

} //namespace bees