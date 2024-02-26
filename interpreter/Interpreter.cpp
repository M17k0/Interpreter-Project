#include <exception>
#include "Reader.cpp"
#include "Tokenizer.cpp"
#include "Compiler.cpp"
#include "Executor.cpp"

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
