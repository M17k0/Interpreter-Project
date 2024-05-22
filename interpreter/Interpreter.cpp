#include <exception>

#include "Reader.h"
#include "Tokenizer.h"
#include "Executor.h"
#include "Compiler.h"

int main()
{
    try
    {
        std::vector<std::string> lines = Reader::readAllLines("test1.txt");

        std::vector<Token> tokens = Tokenizer::tokenize(lines);

        Node treeRoot = Compiler::compile(tokens);

        Executor::execute(treeRoot);

        Executor::deleteTree(treeRoot);
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }

    return 0;
}
