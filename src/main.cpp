#include <fstream>
#include <iostream>

#include <hermes/beescript_loader.h>
#include <hermes/errors.h>

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "Usage: bees [input-file]\n";
        return 1;
    }

    auto input = std::make_shared<std::ifstream>(argv[1]);

    try
    {
        auto parser = hermes::load_beescript();
        auto root = parser->parse(input);
    }
    catch(const HermesError& err)
    {
        std::cout << "Error: " << err.what() << std::endl;
        return 1;
    }

    return 0;
}
