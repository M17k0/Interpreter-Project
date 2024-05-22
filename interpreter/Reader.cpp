#include "Reader.h"

std::vector<std::string> Reader::readAllLines(std::string filePath)
{
	std::ifstream programFile(filePath);
	if (!programFile.is_open())
		throw std::exception("Couldn't open file for reading");

	std::vector<std::string> lines = readAllLines(programFile);

	programFile.close();

	return lines;
}

std::vector<std::string> Reader::readAllLines(std::istream& in)
{
	std::vector<std::string> lines;

	std::string line;

	while (std::getline(in, line))
	{
		lines.push_back(line);
	}

	return lines;
}
