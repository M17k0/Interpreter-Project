#pragma once

#include <vector>
#include <string>
#include <fstream>

/// @brief Reader class with methods for reading from files and streams
class Reader
{
public:
	/// @brief Reads all lines from a file
	/// @param filePath The path of the file
	/// @return A vector with strings of the lines in the file
	static std::vector<std::string> readAllLines(std::string filePath);

private:
	/// @brief Reads all lines from a stream
	/// @param in The input stream
	/// @return A vector with strings of the lines in the stream
	static std::vector<std::string> readAllLines(std::istream& in);
};