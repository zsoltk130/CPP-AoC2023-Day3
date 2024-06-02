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

int findCompleteNumber(std::vector<std::string>& schematic, int posX, int posY)
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
        // schematic[currentPosY][currentPosX] = 'F'; // FOR DEBUG (replaces all "spent" numbers with F's in the vector to be printed and visually analysed)

        if (!isdigit(nextChar))
        {
            break;
        }
        currentChar = currentLine[++currentPosX];
    }
    return stoi(number);
}

void findAdjacentNumbers(std::vector<Symbol> symbols, std::vector<std::string> schematic, std::vector<int>& numbers)
{
    // Adjacent number search logic:
    // 
    //  x0123456789
    //y
    //0  ....401...
    //1  ...*......
    //2  ..159.....
    //
    // 1. Find symbol xy position
    // 
    //  x0123456789
    //y
    //0  ....401...
    //1  ...X......
    //2  ..159.....
    //
    // 2. Search 1 character around symbol until digit found starting from top left
    // 
    //  x0123456789
    //y
    //0  ..X.401...
    //1  ...*......
    //2  ..159.....
    //
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
    // 
    // When digit found use findCompleteNumber -> go all the way to the left until '.' found, then go right one 
    // character at a time and record the number until the other '.' is found
    // 
    //  x0123456789
    //y
    //0  ....XXX...
    //1  ...*......
    //2  ..159.....
    // =>401
    //
    // Continue searching around the symbol one character at a time left to right
    // 
    //  x0123456789
    //y
    //0  ....401...
    //1  ..X*X.....
    //2  ..159.....
    // 
    // When digit found go all the way to the left one digit at a time, then right one digit at a time recording the whole number left to right
    // 
    //  x0123456789
    //y
    //0  ....401...
    //1  ...*......
    //2  ..XXX.....
    // =>159
    //
    // And continue to the next symbol xy position
    
    for (auto symbol : symbols)
    {
        int x = symbol.posX;
        int y = symbol.posY;
        
        // Define adjacent area to symbol
        int xMin = x - 1;
        int xMax = x + 1;
        int yMin = y - 1;
        int yMax = y + 1;
        
        std::string currentLine = schematic[y];
        char currentChar = currentLine[x];
        char previousChar;
        char nextChar;

        // Flag is set when we are looping over a number that we've already found
        // Flag is reset when we pass over a '.' or any of the other special chars
        // This is for cases when the surrounding area around the symbol looks like (or similar):
        // 
        // 3.4
        // .#5
        // 46.
        bool sameNumber = false;

        for (y = yMin; y <= yMax; y++)
        {
            currentLine = schematic[y];
            sameNumber = false;
            for (x = xMin; x <= xMax; x++)
            {
                currentChar = currentLine[x];
                if (!isdigit(currentChar))
                {
                    sameNumber = false;
                }

                if (isdigit(currentChar) && !sameNumber)
                {
                    char s = symbol.symbol;
                    int num = findCompleteNumber(schematic, x, y);

                    if (!sameNumber)
                    {
                        numbers.push_back(num);
                        sameNumber = true;
                    }
                }
            }
        }
    }
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

int findSum(std::vector<int> numbers)
{
    int total = 0;
    for (int number : numbers)
    {
        total += number;
    }
    return total;
}

int main()
{
    std::string filename = "aoc3.txt";
    // std::string outfile = "aoc3debug.txt"; // FOR DEBUG
    std::vector<int> numbers;
    std::vector<std::string> schematic = readLinesFromFile(filename);
    std::vector<Symbol> symbols = findSymbolPositions(schematic);
    findAdjacentNumbers(symbols, schematic, numbers);
    int total = findSum(numbers);

    // writeVectorToFile(schematic, outfile); // FOR DEBUG
	return 0;
}
