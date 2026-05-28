#pragma once

#include <memory>
#include <string>

namespace bees {

enum class DAGType
{
    Assign,
    Deref,

    IAdd,
    ISub,
    IMul,
    IDiv,
    IMod,
    IPow,
    INeg,

    FAdd,
    FSub,
    FMul,
    FDiv,
    FMod,
    FPow,
    FNeg,

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
    LitStr
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