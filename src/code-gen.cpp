#include <beescript/code-gen.h>

#include <beescript/errors.h>

#include <fstream>
#include <sstream>

namespace bees {

CodeGenerator::CodeGenerator(const std::string& filename)
    : out(filename, std::ios_base::out | std::ios_base::binary)
{
}

CodeGenerator::~CodeGenerator()
{
    out.flush();
    out.close();
}

void CodeGenerator::generate(SyntaxNodePtr root)
{
    while(root)
    {
        if(root->nodeType != NodeType::Decl)
        {
            std::stringstream ss;
            ss << "Invalid node for codegen: "
               << getNodeTypeName(root->nodeType);
            throw BeeError(ss.str());
        }

        genDecl(std::static_pointer_cast<DeclNode>(root));
        root = root->next;
    }
}

void CodeGenerator::genDecl(DeclNodePtr node)
{
}

} //namespace bees