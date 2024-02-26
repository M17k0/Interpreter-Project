#pragma once

/// @brief Enumeration for the possible types the tokenizer uses
enum class TokenType
{
    undefined,
    function,
    variable,
    number,
    equals,
    add,
    subtract,
    multiply,
    division,
    modulo,
    left_bracket,
    right_bracket,
    left_parenthesis,
    right_parenthesis,
    print,
    read,
    end_of_line,
};