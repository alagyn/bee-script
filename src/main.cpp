#include <fstream>
#include <iostream>

#include <hermes/beescript_loader.h>
#include <hermes/errors.h>

#include <symbol-table.h>

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
        auto parser = hermes::load_beescript();
        root = parser->parse(input);
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

    return 0;
}
