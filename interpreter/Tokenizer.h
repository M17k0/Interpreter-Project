#pragma once

#include "Token.h"

#include <vector>
#include <iostream>
#include <functional>
#include <sstream>

/// @brief Class with methods for tokenizing text
class Tokenizer
{
public:
    /// @brief Converts the text to tokens
    /// @param Lines vector with strings of the text to tokenize
    /// @return Vector with tokens
    static std::vector<Token> tokenize(std::vector<std::string> lines);

private:
    /// @brief Splits the string into vector with strings by a delimeter
    /// @param str The string to split
    /// @param delimeter The delimeter to use for splitting
    /// @return Vector of the splited strings
    static std::vector<std::string> stringSplit(const std::string& str, char delimeter);

    /// @brief Finds the index of the first occurence determined by a predicate
    /// @param str The string to search
    /// @param predicate The predicate used for searching
    /// @return The index of the first occurence; -1 if not found
    static int stringIndexOf(const std::string& str, std::function<bool(char)> predicate);
};