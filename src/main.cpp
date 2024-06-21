#include <fstream>
#include <iostream>

#include <hermes/beescript_loader.h>
#include <hermes/errors.h>

#include <beescript/ast-to-dag.h>
#include <beescript/symbol-table.h>
#include <beescript/type-check.h>

using namespace bees;

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "Usage: bees [input-file]\n";
        return 1;
    }

    auto input = std::make_shared<std::ifstream>(argv[1]);

    SyntaxNodePtr root;

    try
    {
        bool error = false;
        auto parser = hermes::load_beescript();
        root = parser->parse(input, error);
        if(error)
        {
            std::cout << "Error parsing" << std::endl;
            return 1;
        }
    }
    catch(const std::exception& err)
    {
        std::cout << "Error Parsing: " << err.what() << std::endl;
        return 1;
    }

    SymbolTable symbolTable;

    try
    {
        symbolTable.resolve(root);
    }
    catch(const std::exception& err)
    {
        std::cout << "Error resolving: " << err.what() << std::endl;
        return 1;
    }

    try
    {
        if(!typeCheck(root))
        {
            std::cout << "Error type-checking" << std::endl;
            return 1;
        }
    }
    catch(const std::exception& err)
    {
        std::cout << "Error type-checking: " << err.what() << std::endl;
        return 1;
    }

    DAGNodePtr dag = astToDag(root);

    return 0;
}
