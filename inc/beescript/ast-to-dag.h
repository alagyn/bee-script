#pragma once

#include <beescript/dag-node.h>
#include <beescript/syntax-node.h>

namespace bees {

DAGNodePtr astToDag(SyntaxNodePtr root);

}