// CPP-AoC2023-Day3.cpp : Solution for Advent of Code 2023 Day 3
//

#include "CPP-AoC2023-Day3.h"

#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <regex>

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

// FOR DEBUG
void writeVectorToFile(const std::vector<std::string>& vec, const std::string& filename) {
    // Create an ofstream object to open the file
    std::ofstream outfile(filename);

    // Check if the file is open
    if (!outfile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Write each string in the vector to the file
    for (const auto& str : vec) {
        outfile << str << std::endl;
    }

    // Close the file
    outfile.close();

    // Inform the user that writing is complete
    std::cout << "Data written to file: " << filename << std::endl;
}

int findCompleteNumber(std::vector<std::string>& schematic, int posX, int posY, char symbol)
{
    int currentPosX = posX; //posX
    int currentPosY = posY; //posY

    int minSize = 0;
    int maxSize = schematic.size();

    std::string currentLine = schematic[currentPosY];
    char currentChar = currentLine[currentPosX];

    std::string number;

    while (true)
    {
        if (currentPosX == 0)
        {
            break;
        }
        char previousChar = currentLine[currentPosX - 1];
        if (!isdigit(previousChar))
        {
            break;
        }
        currentChar = currentLine[--currentPosX];
    }

    while (true)
    {
        char nextChar = currentLine[currentPosX + 1];
        number += currentChar;
        schematic[currentPosY][currentPosX] = 'F'; // FOR DEBUG (replaces all "spent" numbers with F's in the vector to be printed and visually analysed)

        if (!isdigit(nextChar))
        {
            break;
        }
        currentChar = currentLine[++currentPosX];
    }
    return stoi(number);
}

int findAdjacentNumbers(std::vector<Symbol>& symbols, std::vector<std::string>& schematic)
{
    // Adjacent number search logic:
    // 
    //  x0123456789
    //y
    //0  ....401...
    //1  ...*......
    //2  ..159.....
    //
    // Find symbol xy position
    //  x0123456789
    //y
    //0  ....401...
    //1  ...X......
    //2  ..159.....
    //
    // Search 1 character around symbol until digit found
    //  x0123456789
    //y
    //0  ...X401...
    //1  ...*......
    //2  ..159.....
    //
    //  x0123456789
    //y
    //0  ....X01...
    //1  ...*......
    //2  ..159.....
    // When digit found go all the way to the left until '.' found, then go right one characater at a time and record the number until the other '.' is found
    //  x0123456789
    //y
    //0  ....XXX...
    //1  ...*......
    //2  ..159.....
    //
    // Continue searching around the symbol one character at a time
    //  x0123456789
    //y
    //0  ....401...
    //1  ..X*X.....
    //2  ..159.....
    // 
    // When digit found go all the way to the left one digit at a time, then right one digit at a time recording the whole number left to right
    //  x0123456789
    //y
    //0  ....401...
    //1  ...*......
    //2  ..XXX.....
    // 
    // And continue to the next symbol xy position
    
    int total = 0;

    int schematicXSize = schematic[0].size();
    int schematicYSize = schematic.size();

    for (auto symbol : symbols)
    {
        int x = symbol.posX;
        int y = symbol.posY;
        
        std::string currentLine = schematic[y];
        char currentChar = currentLine[x];

        int xMin = x - 1;
        int xMax = x + 1;
        int yMin = y - 1;
        int yMax = y + 1;

        for (int i = yMin; i <= yMax; i++)
        {
            currentLine = schematic[i];
            for (int j = xMin; j <= xMax; j++)
            {
                currentChar = currentLine[j];
                if (isdigit(currentChar))
                {
                    char s = symbol.symbol;
                    int num = findCompleteNumber(schematic, j, i, s);
                    total += num;


                    break;
                }
            }
        }
    }
    
    return total;
}

std::vector<Symbol> findSymbolPositions(std::vector<std::string>& schematic)
{
    std::vector<Symbol> symbolPositions;
   
    int schematicXSize = schematic[0].size(); // Vector width
    int schematicYSize = schematic.size(); // Vector height

    std::set<char> symbols = { '*', '%', '#', '@', '+', '-', '=', '/', '&', '$' };

    for (int posY = 0; posY < schematicYSize; posY++) // Y-Axis
    {
        for (int posX = 0; posX < schematicXSize; posX++) // X-Axis
        {
            std::string row = schematic[posY];
            char symbol = row[posX];
            
            if (symbols.find(symbol) != symbols.end()) {
                Symbol s;
                s.symbol = symbol;
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
    std::string outfile = "aoc3debug.txt"; // FOR DEBUG
    std::vector<int> numbers;
    std::vector<std::string> schematic = readLinesFromFile(filename);
    std::vector<Symbol> symbols = findSymbolPositions(schematic);
    int total = findAdjacentNumbers(symbols, schematic);

    writeVectorToFile(schematic, outfile); // FOR DEBUG
	return 0;
}
