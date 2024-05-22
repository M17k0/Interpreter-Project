#pragma once

#include <stack>
#include <iostream>
#include "Node.h"
#include "Token.h"

class Compiler
{
public:
    /// @brief Builds an AST from the tokens vector
    /// @param tokens Vector with the tokens
    /// @return The AST root
    static Node compile(std::vector<Token> tokens);

private:
    /// @brief Computes the precedence of the operator
    /// @param tokenType The token type
    /// @return The precedence of the given operator
    static int getPrecedence(const TokenType& tokenType);

    /// @brief Gets the node type corresponding to a token type
    /// @param type The token type
    /// @return The corresponding node type
    static NodeType getNodeType(const TokenType& type);

    /// @brief Pops the top operator from the operator stack (part of the shunting yard algorithm)
    /// @param operatorStack The operator stack for the shunting yard algorithm
    /// @param outputStack The output stack for the shunting yard algorithm
    /// @param token The current token processed by the algorithm
    static void popOperator(std::stack<Token>& operatorStack, std::stack<Node>& outputStack, Token token);
};
