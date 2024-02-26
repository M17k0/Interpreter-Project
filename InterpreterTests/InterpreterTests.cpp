#include "pch.h"
#include "CppUnitTest.h"
#include <cstdio>
#include "../interpreter/Interpreter.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InterpreterTests
{
	TEST_CLASS(InterpreterTests)
	{
	public:
		
		TEST_METHOD(ReaderReadFromFile)
		{
			std::vector<std::string> lines
			{
				"a = 2",
				"read b",
				"print a + b"
			};
			std::ofstream outputFile("temp.txt", std::ios::out);

			Assert::IsTrue(outputFile.is_open());

			for (const std::string& line : lines)
			{
				outputFile << line << std::endl;
			}

			outputFile.close();

			std::vector<std::string> readLines = Reader::readAllLines("temp.txt");
			
			std::remove("temp.txt");

			Assert::IsTrue(lines.size() == readLines.size());

			for (int i = 0; i < lines.size(); i++)
			{
				Assert::IsTrue(lines[i] == readLines[i]);
			}
		}

		TEST_METHOD(TokenizeFunctionDefinition)
		{
			std::vector<std::string> lines
			{
				"D[x] = 1 + x" 
			};

			std::vector<Token> tokens = Tokenizer::tokenize(lines);

			Assert::IsTrue(tokens.size() == 9);
			Assert::IsTrue(tokens[0].type == TokenType::function);
			Assert::IsTrue(tokens[0].value == "D");
			Assert::IsTrue(tokens[1].type == TokenType::left_bracket);
			Assert::IsTrue(tokens[2].type == TokenType::variable);
			Assert::IsTrue(tokens[2].value == "x");
			Assert::IsTrue(tokens[3].type == TokenType::right_bracket);
			Assert::IsTrue(tokens[4].type == TokenType::equals);
			Assert::IsTrue(tokens[5].type == TokenType::number);
			Assert::IsTrue(tokens[6].type == TokenType::add);
			Assert::IsTrue(tokens[7].type == TokenType::variable);
			Assert::IsTrue(tokens[7].value == "x");
			Assert::IsTrue(tokens[8].type == TokenType::end_of_line);
		}

		TEST_METHOD(TokenizeWholeProgram)
		{
			std::vector<std::string> lines
			{
				"a = 2",
				"read b",
				"c = b - 1",
				"D[x] = (x + 4) * 2 / b % 10",
				"print c",
				"print D[3]"
			};

			std::vector<Token> tokens = Tokenizer::tokenize(lines);

			Assert::IsTrue(tokens.size() == 39);
			Assert::IsTrue(tokens[0].type == TokenType::variable);
			Assert::IsTrue(tokens[0].value == "a");
			Assert::IsTrue(tokens[1].type == TokenType::equals);
			Assert::IsTrue(tokens[2].type == TokenType::number);
			Assert::IsTrue(tokens[2].value == "2");
			Assert::IsTrue(tokens[3].type == TokenType::end_of_line);
			Assert::IsTrue(tokens[4].type == TokenType::read);
			Assert::IsTrue(tokens[5].type == TokenType::variable);
			Assert::IsTrue(tokens[5].value == "b");
			Assert::IsTrue(tokens[6].type == TokenType::end_of_line);
			Assert::IsTrue(tokens[7].type == TokenType::variable);
			Assert::IsTrue(tokens[7].value == "c");
			Assert::IsTrue(tokens[8].type == TokenType::equals);
			Assert::IsTrue(tokens[9].type == TokenType::variable);
			Assert::IsTrue(tokens[9].value == "b");
			Assert::IsTrue(tokens[10].type == TokenType::subtract);
			Assert::IsTrue(tokens[11].type == TokenType::number);
			Assert::IsTrue(tokens[11].value == "1");
			Assert::IsTrue(tokens[12].type == TokenType::end_of_line);
			Assert::IsTrue(tokens[13].type == TokenType::function);
			Assert::IsTrue(tokens[13].value == "D");
			Assert::IsTrue(tokens[14].type == TokenType::left_bracket);
			Assert::IsTrue(tokens[15].type == TokenType::variable);
			Assert::IsTrue(tokens[15].value == "x");
			Assert::IsTrue(tokens[16].type == TokenType::right_bracket);
			Assert::IsTrue(tokens[17].type == TokenType::equals);
			Assert::IsTrue(tokens[18].type == TokenType::left_parenthesis);
			Assert::IsTrue(tokens[19].type == TokenType::variable);
			Assert::IsTrue(tokens[19].value == "x");
			Assert::IsTrue(tokens[20].type == TokenType::add);
			Assert::IsTrue(tokens[21].type == TokenType::number);
			Assert::IsTrue(tokens[21].value == "4");
			Assert::IsTrue(tokens[22].type == TokenType::right_parenthesis);
			Assert::IsTrue(tokens[23].type == TokenType::multiply);
			Assert::IsTrue(tokens[24].type == TokenType::number);
			Assert::IsTrue(tokens[24].value == "2");
			Assert::IsTrue(tokens[25].type == TokenType::division);
			Assert::IsTrue(tokens[26].type == TokenType::variable);
			Assert::IsTrue(tokens[26].value == "b");
			Assert::IsTrue(tokens[27].type == TokenType::modulo);
			Assert::IsTrue(tokens[28].type == TokenType::number);
			Assert::IsTrue(tokens[28].value == "10");
			Assert::IsTrue(tokens[29].type == TokenType::end_of_line);
			Assert::IsTrue(tokens[30].type == TokenType::print);
			Assert::IsTrue(tokens[31].type == TokenType::variable);
			Assert::IsTrue(tokens[31].value == "c");
			Assert::IsTrue(tokens[32].type == TokenType::end_of_line);
			Assert::IsTrue(tokens[33].type == TokenType::print);
			Assert::IsTrue(tokens[34].type == TokenType::function);
			Assert::IsTrue(tokens[34].value == "D");
			Assert::IsTrue(tokens[35].type == TokenType::left_bracket);
			Assert::IsTrue(tokens[36].type == TokenType::number);
			Assert::IsTrue(tokens[36].value == "3");
			Assert::IsTrue(tokens[37].type == TokenType::right_bracket);
			Assert::IsTrue(tokens[38].type == TokenType::end_of_line);
		}

		TEST_METHOD(TokenizeUndefined)
		{
			std::vector<std::string> lines{ "$" };

			std::vector<Token> tokens = Tokenizer::tokenize(lines);

			Assert::IsTrue(tokens.size() == 2);
			Assert::IsTrue(tokens[0].type == TokenType::undefined);
			Assert::IsTrue(tokens[0].value == "$");
			Assert::IsTrue(tokens[1].type == TokenType::end_of_line);
		}

		TEST_METHOD(CompileSimpleAST)
		{
			std::vector<std::string> lines
			{
				"a = 2",
				"read b",
				"print a + b"
			};

			std::vector<Token> tokens = Tokenizer::tokenize(lines);

			Node astRoot = Compiler::compile(tokens);

			Assert::IsTrue(astRoot.type == NodeType::root);
			Assert::IsTrue(astRoot.children->size() == 3);

			Node lineOneRoot = (*astRoot.children)[0];
			Assert::IsTrue(lineOneRoot.type == NodeType::operation_assign);
			Assert::IsTrue(lineOneRoot.children->size() == 2);
			Node leftAssignChild = (*lineOneRoot.children)[0];
			Assert::IsTrue(leftAssignChild.type == NodeType::variable);
			Assert::IsTrue(leftAssignChild.value == "a");
			Assert::IsTrue(leftAssignChild.children->size() == 0);
			Node rightAssignChild = (*lineOneRoot.children)[1];
			Assert::IsTrue(rightAssignChild.type == NodeType::number);
			Assert::IsTrue(rightAssignChild.value == "2");
			Assert::IsTrue(rightAssignChild.children->size() == 0);

			Node lineTwoRoot = (*astRoot.children)[1];
			Assert::IsTrue(lineTwoRoot.type == NodeType::operation_read);
			Assert::IsTrue(lineTwoRoot.children->size() == 1);
			Node readChild = (*lineTwoRoot.children)[0];
			Assert::IsTrue(readChild.type == NodeType::variable);
			Assert::IsTrue(readChild.value == "b");
			Assert::IsTrue(readChild.children->size() == 0);

			Node lineThreeRoot = (*astRoot.children)[2];
			Assert::IsTrue(lineThreeRoot.type == NodeType::operation_print);
			Assert::IsTrue(lineThreeRoot.children->size() == 1);
			Node printChild = (*lineThreeRoot.children)[0];
			Assert::IsTrue(printChild.type == NodeType::operation_add);
			Assert::IsTrue(printChild.children->size() == 2);
			Node leftAddChild = (*printChild.children)[0];
			Assert::IsTrue(leftAddChild.type == NodeType::variable);
			Assert::IsTrue(leftAddChild.value == "a");
			Assert::IsTrue(leftAddChild.children->size() == 0);
			Node rightAddChild = (*printChild.children)[1];
			Assert::IsTrue(rightAddChild.type == NodeType::variable);
			Assert::IsTrue(rightAddChild.value == "b");
			Assert::IsTrue(rightAddChild.children->size() == 0);
		}

		TEST_METHOD(CompileComplexAST)
		{
			std::vector<std::string> lines
			{
				"a = 2",
				"D[x] = (x + 4) * 2 + 10",
				"print D[3]"
			};

			std::vector<Token> tokens = Tokenizer::tokenize(lines);

			Node astRoot = Compiler::compile(tokens);

			Assert::IsTrue(astRoot.type == NodeType::root);
			Assert::IsTrue(astRoot.children->size() == 3);

			Assert::IsTrue((*astRoot.children)[0].type == NodeType::operation_assign);
			Assert::IsTrue((*astRoot.children)[0].children->size() == 2);
			Assert::IsTrue((*(*astRoot.children)[0].children)[0].type == NodeType::variable);
			Assert::IsTrue((*(*astRoot.children)[0].children)[0].value == "a");
			Assert::IsTrue((*(*astRoot.children)[0].children)[0].children->size() == 0);
			Assert::IsTrue((*(*astRoot.children)[0].children)[1].type == NodeType::number);
			Assert::IsTrue((*(*astRoot.children)[0].children)[1].value == "2");
			Assert::IsTrue((*(*astRoot.children)[0].children)[1].children->size() == 0);

			Assert::IsTrue((*astRoot.children)[1].type == NodeType::define_function);
			Assert::IsTrue((*astRoot.children)[1].value == "D");
			Assert::IsTrue((*astRoot.children)[1].children->size() == 2);
			Assert::IsTrue((*(*astRoot.children)[1].children)[0].type == NodeType::variable);
			Assert::IsTrue((*(*astRoot.children)[1].children)[0].value == "x");
			Assert::IsTrue((*(*astRoot.children)[1].children)[0].children->size() == 0);
			Assert::IsTrue((*(*astRoot.children)[1].children)[1].type == NodeType::operation_add);
			Assert::IsTrue((*(*astRoot.children)[1].children)[1].children->size() == 2);
			Assert::IsTrue((*(*(*astRoot.children)[1].children)[1].children)[0].type == NodeType::operation_multipy);
			Assert::IsTrue((*(*(*astRoot.children)[1].children)[1].children)[0].children->size() == 2);
			Assert::IsTrue((*(*(*astRoot.children)[1].children)[1].children)[1].type == NodeType::number);
			Assert::IsTrue((*(*(*astRoot.children)[1].children)[1].children)[1].value == "10");
			Assert::IsTrue((*(*(*astRoot.children)[1].children)[1].children)[1].children->size() == 0);
			Assert::IsTrue((*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[0].type == NodeType::operation_add);
			Assert::IsTrue((*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[0].children->size() == 2);
			Assert::IsTrue((*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[1].type == NodeType::number);
			Assert::IsTrue((*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[1].value == "2");
			Assert::IsTrue((*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[1].children->size() == 0);
			Assert::IsTrue((*(*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[0].children)[0].type == NodeType::variable);
			Assert::IsTrue((*(*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[0].children)[0].value == "x");
			Assert::IsTrue((*(*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[0].children)[0].children->size() == 0);
			Assert::IsTrue((*(*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[0].children)[1].type == NodeType::number);
			Assert::IsTrue((*(*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[0].children)[1].value == "4");
			Assert::IsTrue((*(*(*(*(*astRoot.children)[1].children)[1].children)[0].children)[0].children)[1].children->size() == 0);

			Assert::IsTrue((*astRoot.children)[2].type == NodeType::operation_print);
			Assert::IsTrue((*astRoot.children)[2].children->size() == 1);
			Assert::IsTrue((*(*astRoot.children)[2].children)[0].type == NodeType::function);
			Assert::IsTrue((*(*astRoot.children)[2].children)[0].value == "D");
			Assert::IsTrue((*(*astRoot.children)[2].children)[0].children->size() == 1);
			Assert::IsTrue((*(*(*astRoot.children)[2].children)[0].children)[0].type == NodeType::number);
			Assert::IsTrue((*(*(*astRoot.children)[2].children)[0].children)[0].value == "3");
			Assert::IsTrue((*(*(*astRoot.children)[2].children)[0].children)[0].children->size() == 0);
		}
		
		TEST_METHOD(CompileSyntaxError)
		{
			std::vector<std::string> lines { "a = 1", "b 2 3" };
			std::vector<Token> tokens = Tokenizer::tokenize(lines);

			Assert::ExpectException<std::invalid_argument>([tokens]
			{
				Compiler::compile(tokens);
			});
		}

		TEST_METHOD(ExecuteSimpleTest)
		{
			Node treeRoot(NodeType::root);
			treeRoot.children->push_back(Node(NodeType::operation_assign));
			(*treeRoot.children)[0].children->push_back(Node(NodeType::variable, "a"));
			(*treeRoot.children)[0].children->push_back(Node(NodeType::number, "2"));

			treeRoot.children->push_back(Node(NodeType::operation_assign));
			(*treeRoot.children)[1].children->push_back(Node(NodeType::variable, "b"));
			(*treeRoot.children)[1].children->push_back(Node(NodeType::operation_add));
			(*(*treeRoot.children)[1].children)[1].children->push_back(Node(NodeType::variable, "a"));
			(*(*treeRoot.children)[1].children)[1].children->push_back(Node(NodeType::number, "3"));

			treeRoot.children->push_back(Node(NodeType::operation_print));
			(*treeRoot.children)[2].children->push_back(Node(NodeType::variable, "b"));

			std::ostringstream outputStream;

			std::istringstream inputStream;

			Executor::execute(treeRoot, outputStream, inputStream);

			std::ostringstream expectedOutputStream;
			expectedOutputStream << "5" << std::endl;

			Assert::IsTrue(outputStream.str() == expectedOutputStream.str());
		}

		TEST_METHOD(ExecuteWholeProgram)
		{
			std::vector<std::string> lines
			{
				"a = 2",
				"read b",
				"c = b - 1",
				"D[x] = (x + 4) * 2 / b + c",
				"print c",
				"print D[a + 5]"
			};

			std::vector<Token> tokens = Tokenizer::tokenize(lines);

			Node treeRoot = Compiler::compile(tokens);

			std::ostringstream outputStream;
			std::istringstream inputStream("7");

			Executor::execute(treeRoot, outputStream, inputStream);
			
			std::ostringstream expectedOutputStream;
			expectedOutputStream << "6" << std::endl << "9" << std::endl;

			Assert::IsTrue(outputStream.str() == expectedOutputStream.str());
		}
	};
}
