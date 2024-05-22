#include "Tokenizer.h"

std::vector<Token> Tokenizer::tokenize(std::vector<std::string> lines)
{
    std::vector<Token> tokens;

    for (int lineIndex = 0; lineIndex < lines.size(); lineIndex++)
    {
        std::string line = lines[lineIndex];

        int columnIndex = 0;

        // Split in parts (for easier tokeization)
        std::vector<std::string> textElements = stringSplit(line, ' ');

        // Tokenize
        for (std::string el : textElements)
        {
            // Needed to compute the column index of the current token
            int lastElementLength = el.size();

            std::string remainingText = el;

            while (remainingText.length() > 0)
            {
                // Update current column index
                if (remainingText.length() < lastElementLength)
                {
                    columnIndex += lastElementLength - remainingText.length();

                    lastElementLength = remainingText.length();
                }

                // Variable check / read / print
                if (remainingText[0] >= 'a' && remainingText[0] <= 'z')
                {
                    std::string varName;

                    int firstNonLowercaseLetterIndex = stringIndexOf(remainingText, [](char c) -> bool { return c < 'a' || c > 'z'; });
                    if (firstNonLowercaseLetterIndex < 0)
                    {
                        varName = remainingText;
                        remainingText = "";
                    }
                    else
                    {
                        varName = remainingText.substr(0, firstNonLowercaseLetterIndex);
                        remainingText = remainingText.substr(firstNonLowercaseLetterIndex);
                    }

                    if (varName == "read")
                    {
                        tokens.push_back(Token(TokenType::read, varName, lineIndex + 1, columnIndex + 1));
                    }
                    else if (varName == "print")
                    {
                        tokens.push_back(Token(TokenType::print, varName, lineIndex + 1, columnIndex + 1));
                    }
                    else
                    {
                        tokens.push_back(Token(TokenType::variable, varName, lineIndex + 1, columnIndex + 1));
                    }
                }
                // Function check
                else if (remainingText[0] >= 'A' && remainingText[0] <= 'Z')
                {
                    std::string funcName;

                    int firstNonUppercaseLetterIndex = stringIndexOf(remainingText, [](char c) -> bool { return c < 'A' || c > 'Z'; });
                    if (firstNonUppercaseLetterIndex < 0)
                    {
                        funcName = remainingText;
                        remainingText = "";
                    }
                    else
                    {
                        funcName = remainingText.substr(0, firstNonUppercaseLetterIndex);
                        remainingText = remainingText.substr(firstNonUppercaseLetterIndex);
                    }

                    tokens.push_back(Token(TokenType::function, funcName, lineIndex + 1, columnIndex + 1));
                }
                // Number check
                else if (remainingText[0] >= '0' && remainingText[0] <= '9')
                {
                    std::string numText;

                    int firstNonNumberIndex = stringIndexOf(remainingText, [](char c) -> bool { return c < '0' || c > '9'; });
                    if (firstNonNumberIndex < 0)
                    {
                        numText = remainingText;
                        remainingText = "";
                    }
                    else
                    {
                        numText = remainingText.substr(0, firstNonNumberIndex);
                        remainingText = remainingText.substr(firstNonNumberIndex);
                    }

                    tokens.push_back(Token(TokenType::number, numText, lineIndex + 1, columnIndex + 1));
                }
                // Operator =
                else if (remainingText[0] == '=')
                {
                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::equals, "=", lineIndex + 1, columnIndex + 1));
                }
                // Operator +
                else if (remainingText[0] == '+')
                {
                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::add, "+", lineIndex + 1, columnIndex + 1));
                }
                // Operator -
                else if (remainingText[0] == '-')
                {
                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::subtract, "-", lineIndex + 1, columnIndex + 1));
                }
                // Operator *
                else if (remainingText[0] == '*')
                {
                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::multiply, "*", lineIndex + 1, columnIndex + 1));
                }
                // Operator /
                else if (remainingText[0] == '/')
                {
                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::division, "/", lineIndex + 1, columnIndex + 1));
                }
                // Operator %
                else if (remainingText[0] == '%')
                {
                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::modulo, "%", lineIndex + 1, columnIndex + 1));
                }
                // Left bracket
                else if (remainingText[0] == '[')
                {
                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::left_bracket, "[", lineIndex + 1, columnIndex + 1));
                }
                // Right bracket
                else if (remainingText[0] == ']')
                {
                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::right_bracket, "]", lineIndex + 1, columnIndex + 1));
                }
                //  Left parenthesis
                else if (remainingText[0] == '(')
                {
                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::left_parenthesis, "(", lineIndex + 1, columnIndex + 1));
                }
                //  Right parenthesis
                else if (remainingText[0] == ')')
                {
                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::right_parenthesis, ")", lineIndex + 1, columnIndex + 1));
                }
                // Undefined (if the symbol was not recognised)
                else
                {
                    std::string symbol = remainingText.substr(0, 1);

                    remainingText = remainingText.substr(1);

                    tokens.push_back(Token(TokenType::undefined, symbol, lineIndex + 1, columnIndex + 1));
                }
            }

            // Update current column index
            if (remainingText.length() < lastElementLength)
            {
                columnIndex += lastElementLength - remainingText.length();

                lastElementLength = remainingText.length();
            }

            // Add space after token to the column index (because we split the the initial line by spaces)
            columnIndex += 1;
        }

        tokens.push_back(Token(TokenType::end_of_line, lineIndex + 1, columnIndex + 1));
    }

    return tokens;
}

std::vector<std::string> Tokenizer::stringSplit(const std::string& str, char delimeter)
{
    std::vector<std::string> segments;
    std::istringstream stream(str);
    std::string segment;

    while (std::getline(stream, segment, delimeter))
    {
        segments.push_back(segment);
    }

    return segments;
}

int Tokenizer::stringIndexOf(const std::string& str, std::function<bool(char)> predicate)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (predicate(str[i]))
            return i;
    }

    return -1;
}
