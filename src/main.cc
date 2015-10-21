#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "parser/Parser.hh"

int main(int argc, char *argv[])
{
    Parser parser;
    if(argc == 1 || strcmp(argv[1], "--debug") == 0)
    {
        std::string line;
        std::cout << ">>>";
        while(getline(std::cin, line))
        {
            try 
            {   
                std::stringstream input_stream{line};
                if(argc == 1)
                {
                    parser.setContent(input_stream);
                    parser.dump(*parser.result(), std::cout);
                }
                else
                {
                    Syntax syn{std::make_unique<Lexical>(input_stream)};
                    Parser::dump(*syn.getPtr(), std::cout);
                }
                std::cout << std::endl;
            }
            catch(std::logic_error& e)
            {
                std::cout << e.what() << std::endl;
            }
            std::cout << ">>>";
        }
        std::cout << std::endl;
    }
    else
    {
        try
        {
            std::ifstream file{argv[1]};
            parser.setContent(file);
            parser.dump(*parser.result(), std::cout);
            std::cout << std::endl;
        }
        catch(std::logic_error& e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}
