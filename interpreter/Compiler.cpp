#include "Compiler.h"

Node Compiler::compile(std::vector<Token> tokens)
{
	// Check tokens for correct syntax
	for (int i = 0; i < tokens.size(); i++)
	{
		switch (tokens[i].type)
		{
		case TokenType::undefined:
			throw std::invalid_argument("Unexpected symbol on line: " + std::to_string(tokens[i].line) + ", column: " + std::to_string(tokens[i].column));
		case TokenType::function:
			if (i + 1 >= tokens.size())
			{
				throw std::invalid_argument("Unexpected end of line: " + std::to_string(tokens[i].line));
			}

			if (tokens[i + 1].type != TokenType::left_bracket)
			{
				throw std::invalid_argument("Unexpected token after function '" + tokens[i].value + "' on line: " + std::to_string(tokens[i + 1].line) + ", column: " + std::to_string(tokens[i + 1].column));
			}

			// Check for recursion (if current function token is at the beggining of the line, and there is another util the end)
			if (i == 0
				|| tokens[i - 1].type == TokenType::end_of_line)
			{
				int curLineI = i + 1;
				while (curLineI < tokens.size() && tokens[curLineI].type != TokenType::end_of_line)
				{
					if (tokens[curLineI].type == TokenType::function
						&& tokens[curLineI].value == tokens[i].value)
					{
						throw std::invalid_argument("Recursion is not supported! Unexpected function call '" + tokens[curLineI].value + "' on line: " + std::to_string(tokens[curLineI].line) + ", column: " + std::to_string(tokens[curLineI].column));
					}
					curLineI++;
				}
			}
			break;
		case TokenType::variable:
			if (i + 1 < tokens.size())
			{
				if (tokens[i + 1].type == TokenType::variable
					|| tokens[i + 1].type == TokenType::function
					|| tokens[i + 1].type == TokenType::left_bracket
					|| tokens[i + 1].type == TokenType::left_parenthesis
					|| tokens[i + 1].type == TokenType::number
					|| tokens[i + 1].type == TokenType::print
					|| tokens[i + 1].type == TokenType::read)
				{
					throw std::invalid_argument("Unexpected token after variable '" + tokens[i].value + "' on line: " + std::to_string(tokens[i + 1].line) + ", column: " + std::to_string(tokens[i + 1].column));
				}

				if (tokens[i + 1].type == TokenType::equals
					&& (i != 0
						&& tokens[i - 1].type != TokenType::end_of_line))
				{
					throw std::invalid_argument("Unexpected token after variable '" + tokens[i].value + "' on line: " + std::to_string(tokens[i + 1].line) + ", column: " + std::to_string(tokens[i + 1].column));
				}
			}
			break;
		case TokenType::number:
			if (i + 1 < tokens.size())
			{
				if (tokens[i + 1].type == TokenType::variable
					|| tokens[i + 1].type == TokenType::function
					|| tokens[i + 1].type == TokenType::left_bracket
					|| tokens[i + 1].type == TokenType::left_parenthesis
					|| tokens[i + 1].type == TokenType::number
					|| tokens[i + 1].type == TokenType::print
					|| tokens[i + 1].type == TokenType::read
					|| tokens[i + 1].type == TokenType::equals)
				{
					throw std::invalid_argument("Unexpected token on line: " + std::to_string(tokens[i + 1].line) + ", column: " + std::to_string(tokens[i + 1].column));
				}
			}
			break;
		case TokenType::equals:
		case TokenType::add:
		case TokenType::subtract:
		case TokenType::multiply:
		case TokenType::division:
		case TokenType::modulo:
		case TokenType::left_bracket:
		case TokenType::left_parenthesis:
			if (i + 1 >= tokens.size())
			{
				throw std::invalid_argument("Unexpected end of line: " + std::to_string(tokens[i].line));
			}

			if (tokens[i + 1].type != TokenType::variable
				&& tokens[i + 1].type != TokenType::function
				&& tokens[i + 1].type != TokenType::number
				&& tokens[i + 1].type != TokenType::left_parenthesis)
			{
				throw std::invalid_argument("Unexpected token on line: " + std::to_string(tokens[i + 1].line) + ", column: " + std::to_string(tokens[i + 1].column));
			}
			break;
		case TokenType::right_bracket:
			if (i + 1 < tokens.size())
			{
				if (tokens[i + 1].type == TokenType::variable
					|| tokens[i + 1].type == TokenType::function
					|| tokens[i + 1].type == TokenType::left_bracket
					|| tokens[i + 1].type == TokenType::left_parenthesis
					|| tokens[i + 1].type == TokenType::number
					|| tokens[i + 1].type == TokenType::print
					|| tokens[i + 1].type == TokenType::read)
				{
					throw std::invalid_argument("Unexpected token on line: " + std::to_string(tokens[i + 1].line) + ", column: " + std::to_string(tokens[i + 1].column));
				}
			}
			break;
		case TokenType::right_parenthesis:
			if (i + 1 < tokens.size())
			{
				if (tokens[i + 1].type == TokenType::variable
					|| tokens[i + 1].type == TokenType::function
					|| tokens[i + 1].type == TokenType::left_bracket
					|| tokens[i + 1].type == TokenType::left_parenthesis
					|| tokens[i + 1].type == TokenType::number
					|| tokens[i + 1].type == TokenType::print
					|| tokens[i + 1].type == TokenType::read
					|| tokens[i + 1].type == TokenType::equals)
				{
					throw std::invalid_argument("Unexpected token on line: " + std::to_string(tokens[i + 1].line) + ", column: " + std::to_string(tokens[i + 1].column));
				}
			}
			break;
		case TokenType::print:
			if (i + 1 >= tokens.size())
			{
				throw std::invalid_argument("Unexpected end of line: " + std::to_string(tokens[i].line));
			}

			if (tokens[i + 1].type != TokenType::variable
				&& tokens[i + 1].type != TokenType::function
				&& tokens[i + 1].type != TokenType::number
				&& tokens[i + 1].type != TokenType::left_parenthesis)
			{
				throw std::invalid_argument("Unexpected token on line: " + std::to_string(tokens[i + 1].line) + ", column: " + std::to_string(tokens[i + 1].column));
			}
			break;
		case TokenType::read:
			if (i + 1 >= tokens.size())
			{
				throw std::invalid_argument("Unexpected end of line: " + std::to_string(tokens[i].line));
			}

			if (tokens[i + 1].type != TokenType::variable)
			{
				throw std::invalid_argument("Unexpected token on line: " + std::to_string(tokens[i + 1].line) + ", column: " + std::to_string(tokens[i + 1].column));
			}
			break;
		case TokenType::end_of_line:
			if (i + 1 < tokens.size()
				&& tokens[i + 1].type != TokenType::variable
				&& tokens[i + 1].type != TokenType::function
				&& tokens[i + 1].type != TokenType::print
				&& tokens[i + 1].type != TokenType::read
				&& tokens[i + 1].type != TokenType::end_of_line)
			{
				throw std::invalid_argument("Unexpected token on line: " + std::to_string(tokens[i + 1].line) + ", column: " + std::to_string(tokens[i + 1].column));
			}
			break;
		default:
			break;
		}
	}

	// Build AST using shunting yard algorithm for expressions directly form the tokens (infix syntax) 
	// without converting to prefix first
	Node treeRoot(NodeType::root);

	Node* parentNode = &treeRoot;

	// Determines if we are building an expression currently
	bool isInExpression = false;

	// Output stack for the shunting yard algorithm (contains the nodes for the AST)
	std::stack<Node> outputStack;
	// Operator stack for the shunting yard algorithm (stores the operators until they are processed to the output)
	std::stack<Token> operatorStack;

	for (int i = 0; i < tokens.size(); i++)
	{
		// When computing an expression use the shunting yard algorithm
		if (isInExpression)
		{
			switch (tokens[i].type)
			{
				// Case 1: Number / Var
			case TokenType::variable:
				outputStack.push(Node(NodeType::variable, tokens[i].value));
				break;
			case TokenType::number:
				outputStack.push(Node(NodeType::number, tokens[i].value));
				break;
				// Case 2: Function
			case TokenType::function:
				operatorStack.push(tokens[i]);
				break;
				// Case 3: Operator
			case TokenType::add:
			case TokenType::subtract:
			case TokenType::division:
			case TokenType::multiply:
			case TokenType::modulo:
				// While operator stack is not empty and the last element is not left parenthesis 
				// and the top element precedence is bigger than the current or the precedences are equal,
				// we pop the operators and move them to the output stack (building the AST) and
				// push the current operator to the operator stack
				while (!operatorStack.empty() && operatorStack.top().type != TokenType::left_parenthesis
					&& (getPrecedence(operatorStack.top().type) < getPrecedence(tokens[i].type)
						|| (getPrecedence(operatorStack.top().type) == getPrecedence(tokens[i].type))))
				{
					popOperator(operatorStack, outputStack, tokens[i]);
				}
				operatorStack.push(tokens[i]);
				break;
				// Case 4: left parenthesis/bracket
			case TokenType::left_parenthesis:
			case TokenType::left_bracket:
				operatorStack.push(tokens[i]);
				break;
				// Case 5: right parenthesis/bracket
			case TokenType::right_parenthesis:
			case TokenType::right_bracket:
			{
				// Pop operators until we find the matching left parenthesis/bracket and then pop it as well
				// Note: parenthesis and brackets are not part of the AST and are not added to the output
				TokenType bracketType = tokens[i].type == TokenType::right_bracket ? TokenType::left_bracket : TokenType::left_parenthesis;
				while (!operatorStack.empty() && operatorStack.top().type != bracketType)
				{
					popOperator(operatorStack, outputStack, tokens[i]);
				}
				if (!operatorStack.empty())
				{
					operatorStack.pop();
				}
				else
				{
					throw std::invalid_argument("Missmatched parenthesis on line: " + std::to_string(tokens[i].line));
				}
			}
			break;
			// Case 5: End of line
			case TokenType::end_of_line:
			{
				// Pop the remaining operators from the stack
				while (!operatorStack.empty())
				{
					if (operatorStack.top().type == TokenType::left_parenthesis)
					{
						throw std::invalid_argument("Missmatched parenthesis on line: " + std::to_string(tokens[i].line));
					}
					if (operatorStack.top().type == TokenType::left_bracket)
					{
						throw std::invalid_argument("Missmatched parenthesis on line: " + std::to_string(tokens[i].line));
					}

					popOperator(operatorStack, outputStack, tokens[i]);
				}

				// At the end the output should have only one element (the root node of the expression)
				if (outputStack.size() != 1)
				{
					throw std::invalid_argument("Invalid syntax on line: " + std::to_string(tokens[i].line));
				}

				Node node = outputStack.top();
				outputStack.pop();

				parentNode->children->push_back(node);
			}
			break;
			default: break;
			}
		}
		else // When in the beginning of the line
		{
			switch (tokens[i].type)
			{
				// Case 1: Variable declaration
			case TokenType::variable:
			{
				// Root is the assignment operator that has two children:
				// the first is the variable parameter 
				// the second is the root of the function expresion 

				if (tokens[i + 1].type != TokenType::equals)
				{
					throw std::invalid_argument("Expected '=' on line: " + std::to_string(tokens[i].line));
				}

				Node assignNode(NodeType::operation_assign);
				assignNode.children->push_back(Node(NodeType::variable, tokens[i].value));

				parentNode->children->push_back(assignNode);
				parentNode = &assignNode;

				i++;

				isInExpression = true;
			}
			break;
			// Case 2: Function declaration
			case TokenType::function:
			{
				// Root is the function name with two children:
				// the first is the function parameter 
				// the second is the root of the function expression 

				if (i + 5 >= tokens.size())
				{
					throw std::invalid_argument("Invalid function definition on line: " + std::to_string(tokens[i].line));
				}

				if (tokens[i + 1].type != TokenType::left_bracket
					|| tokens[i + 2].type != TokenType::variable
					|| tokens[i + 3].type != TokenType::right_bracket
					|| tokens[i + 4].type != TokenType::equals)
				{
					throw std::invalid_argument("Invalid function definition on line: " + std::to_string(tokens[i].line));
				}

				Node functionDefNode(NodeType::define_function, tokens[i].value);
				Node variableNode(NodeType::variable, tokens[i + 2].value);

				functionDefNode.children->push_back(variableNode);

				parentNode->children->push_back(functionDefNode);
				parentNode = &functionDefNode;

				i += 4;

				isInExpression = true;
			}
			break;
			// Case 3: read keyword
			case TokenType::read:
			{
				// Root is the read operator with only one child that should be a variable

				if (i + 2 >= tokens.size())
				{
					throw std::invalid_argument("Unexpected end of line on line: " + std::to_string(tokens[i].line));
				}
				if (tokens[i + 1].type != TokenType::variable)
				{
					throw std::invalid_argument("Expected variable on line: " + std::to_string(tokens[i].line));
				}
				if (tokens[i + 2].type != TokenType::end_of_line)
				{
					throw std::invalid_argument("Read accepts only one argument on line: " + std::to_string(tokens[i].line));
				}

				Node readNode(NodeType::operation_read);
				readNode.children->push_back(Node(NodeType::variable, tokens[i + 1].value));

				parentNode->children->push_back(readNode);

				i++;
			}
			break;
			// Case 4: print keyword
			case TokenType::print:
			{
				// Root is the print operator with only one child that will be the root of the expression

				Node printNode(NodeType::operation_print);

				parentNode->children->push_back(printNode);
				parentNode = &printNode;

				isInExpression = true;

			}
			break;
			default:
				break;
			}
		}

		if (tokens[i].type == TokenType::end_of_line)
		{
			isInExpression = false;

			parentNode = &treeRoot;
		}
	}

	return treeRoot;
}

int Compiler::getPrecedence(const TokenType& tokenType)
{
	switch (tokenType)
	{
	case TokenType::function:
		return 0;
	case TokenType::multiply:
	case TokenType::division:
	case TokenType::modulo:
		return 1;
	case TokenType::add:
	case TokenType::subtract:
		return 2;
	default:
		return 3;
	}
}

NodeType Compiler::getNodeType(const TokenType& type)
{
	switch (type)
	{
	case TokenType::number: return NodeType::number;
	case TokenType::variable: return NodeType::variable;
	case TokenType::add: return NodeType::operation_add;
	case TokenType::subtract: return NodeType::operation_subtract;
	case TokenType::multiply: return NodeType::operation_multipy;
	case TokenType::division: return NodeType::operation_divide;
	case TokenType::modulo: return NodeType::operation_modulo;
	case TokenType::function: return NodeType::function;
	default: return NodeType::undefined;
	}
}

void Compiler::popOperator(std::stack<Token>& operatorStack, std::stack<Node>& outputStack, Token token)
{
	// When popping operator we get its operands from the output stack
	// and build a node for the AST and push it back to the output stack

	Token operatorToken = operatorStack.top();
	operatorStack.pop();

	Node operatorNode(getNodeType(operatorToken.type), operatorToken.value);

	// Function has only one operand
	if (operatorToken.type == TokenType::function)
	{
		if (outputStack.empty())
		{
			throw std::invalid_argument("Invalid syntax on line: " + std::to_string(token.line));
		}
		Node element = outputStack.top();
		outputStack.pop();

		operatorNode.children->push_back(element);
	}
	else // All other operators have two operands
	{
		if (outputStack.empty())
		{
			throw std::invalid_argument("Invalid syntax on line: " + std::to_string(token.line));
		}
		Node right = outputStack.top();
		outputStack.pop();

		if (outputStack.empty())
		{
			throw std::invalid_argument("Invalid syntax on line: " + std::to_string(token.line));
		}
		Node left = outputStack.top();
		outputStack.pop();

		operatorNode.children->push_back(left);
		operatorNode.children->push_back(right);
	}

	outputStack.push(operatorNode);
}
