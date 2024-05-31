// CPP-AoC2023-Day3.cpp : Solution for Advent of Code 2023 Day 3
//

#include "CPP-AoC2023-Day3.h"

#include <vector>
#include <string>
#include <fstream>
#include <set>

struct Symbol
{
    char symbol;
    int posX;
    int posY;
};

// Read txt line by line into vector of strings
std::vector<std::string> readLinesFromFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return lines;
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return lines;
}

std::vector<int> findAdjacentNumbers(std::vector<Symbol>)
{
    std::vector<int> nums;
    
    return nums;
}

std::vector<Symbol> findSymbolPositions(std::vector<std::string>& vect)
{
    std::vector<Symbol> symbolPositions;
   
    int vectRowSize = vect[0].size(); // Vector width
    int vectColSize = vect.size(); // Vector height

    std::set<char> specialChars = { '*', '%', '#', '@', '+', '-', '=', '/', '&', '$' };

    for (int posY = 0; posY < vectRowSize; posY++) // Y-Axis
    {
        for (int posX = 0; posX < vectColSize; posX++) // X-Axis
        {
            std::string row = vect[posY];
            char c = row[posX];
            
            if (specialChars.find(c) != specialChars.end()) {
                Symbol s;
                s.symbol = c;
                s.posX = posX;
                s.posY = posY;
                
                symbolPositions.push_back(s);
            }
        }
    }
    return symbolPositions;
}

int main()
{
    std::string filename = "aoc3.txt";
    std::vector<std::string> schematic = readLinesFromFile(filename);
    std::vector<Symbol> symbols = findSymbolPositions(schematic);
	return 0;
}
