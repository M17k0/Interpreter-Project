#pragma once
#include "TokenType.h"
#include <string>

/// @brief Token object representing the tokenized elements
class Token
{
public:
    /// @brief The type of the token
    TokenType type;
    /// @brief The value of the token
    std::string value;
    /// @brief Line number on which the token occured
    int line;
    /// @brief Column number on which the token occured
    int column;
    /// @brief Constructor for creating a token by type, line and column
    /// @param type Token type
    /// @param line Line number of token occurence
    /// @param column Column number of token occurence
    Token(TokenType type, int line, int column) : type(type), line(line), column(column) {}
    /// @brief Constructor for creating a token by type, value, line and column
    /// @param type Token type
    /// @param value Token value
    /// @param line Line number of token occurence
    /// @param column Column number of token occurence
    Token(TokenType type, std::string value, int line, int column) : type(type), value(value), line(line), column(column) {}
};