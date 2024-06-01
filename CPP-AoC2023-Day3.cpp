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

std::vector<int> findAdjacentNumbers(std::vector<Symbol> symbols, std::vector<std::string>& schematic)
{
    std::vector<int> nums;
    
    // The boundary of the search area
    // 
    //  x: 0123456789
    // y0: ....401...
    // y1: ...*......
    // y2: ..159.....
    // y3: ..........
    //
    // specialCharacterPosition: x3y1
    // searchAreaBounds: x3+xLeftOffset; x3+xRightOffset; y1+yTopOffset; y1+yBottomOffset
    //                 = x0            ; x6             ; y0           ; y2
    //
    //  x: 0123456789
    // y0: ####401...
    // y1: ###*###...
    // y2: ##159##...
    // y3: ..........
    // 
    // adjacentNums: 401, 159

    int xLeftOffset = -3;
    int xRightOffset = 3;
    int yTopOffset = -1;
    int yBottomOffset = 1;

    int schematicSizeX = schematic[0].size();
    int schematicSizeY = schematic.size();
    
    std::string selection;

    for (auto symbol : symbols)
    {
        int xLeftBoundary = ((symbol.posX + xLeftOffset) < 0) ? 0 : (symbol.posX + xLeftOffset);
        int xRightBoundary = ((symbol.posX + xRightOffset) > schematicSizeX) ? schematicSizeX : (symbol.posX + xRightOffset);
        int yTopBoundary = ((symbol.posY + yTopOffset) < 0) ? 0 : (symbol.posY + yTopOffset);
        int yBottomBoundary = ((symbol.posY + yBottomOffset) > schematicSizeY) ? schematicSizeY : (symbol.posY + yBottomOffset);

        int selectionSize = (xRightBoundary - xLeftBoundary) + 1;

        for (int yPos = yTopBoundary; yPos <= yBottomBoundary; yPos++)
        {
            std::string line = schematic[yPos];
            selection = line.substr(xLeftBoundary, selectionSize);
        }
            
    }
    
    return nums;
}

std::vector<Symbol> findSymbolPositions(std::vector<std::string>& schematic)
{
    std::vector<Symbol> symbolPositions;
   
    int schematicRowSize = schematic[0].size(); // Vector width
    int schematicColSize = schematic.size(); // Vector height

    std::set<char> symbols = { '*', '%', '#', '@', '+', '-', '=', '/', '&', '$' };

    for (int posY = 0; posY < schematicRowSize; posY++) // Y-Axis
    {
        for (int posX = 0; posX < schematicColSize; posX++) // X-Axis
        {
            std::string row = schematic[posY];
            char c = row[posX];
            
            if (symbols.find(c) != symbols.end()) {
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
    std::vector<int> nums = findAdjacentNumbers(symbols, schematic);
	return 0;
}
