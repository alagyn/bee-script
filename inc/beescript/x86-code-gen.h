#pragma once

#include <beescript/code-gen.h>

namespace bees {
class X86Generator : public CodeGenerator
{
public:
    X86Generator(const std::string& filename);
};

} //namespace bees