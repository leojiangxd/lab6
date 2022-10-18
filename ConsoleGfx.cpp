#include <fstream>
#include <iostream>
#include <string>

#include "ConsoleGfx.h"

using std::cout;
using std::ifstream;
using std::string;

ConsoleGfx* ConsoleGfx::instance = nullptr;

// Constructor - initializes the palettes
ConsoleGfx::ConsoleGfx()
{
    fgPalette = new string[16];
    emPalette = new string[16];
    ulPalette = new string[16];
    bgPalette = new string[16];

    for (int index = 0; index < 8; index++)
    {
        fgPalette[index] = "\033[0;3" + std::__cxx11::to_string(index) + "m";
        fgPalette[index+8] = "\033[0;9" + std::__cxx11::to_string(index) + "m";
        emPalette[index] = "\033[1;3" + std::__cxx11::to_string(index) + "m";
        emPalette[index+8] = "\033[1;9" + std::__cxx11::to_string(index) + "m";
        ulPalette[index] = "\033[4;3" + std::__cxx11::to_string(index) + "m";
        ulPalette[index+8] = "\033[4;9" + std::__cxx11::to_string(index) + "m";
        bgPalette[index] = "\033[4" + std::__cxx11::to_string(index) + "m";
        bgPalette[index+8] = "\033[10" + std::__cxx11::to_string(index) + "m";
    }

    testRainbow = new unsigned char[34]
            {
                    16, 2,
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
            };

    testImage = new unsigned char[86]
            {
                    14, 6,
                    CLEAR, CLEAR, GREEN, GREEN, GREEN, CLEAR, CLEAR, CLEAR, CLEAR, CLEAR, CLEAR, GREEN, GREEN, CLEAR,
                    CLEAR, GREEN, WHITE, BLACK, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, DARK_GREEN, GREEN, GREEN,
                    GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, CLEAR,
                    GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, BLACK, BLACK, BLACK, GREEN, CLEAR,
                    GREEN, GREEN, GREEN, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, GREEN, GREEN, GREEN, CLEAR, CLEAR,
                    CLEAR, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, CLEAR, CLEAR, CLEAR, CLEAR, CLEAR
            };
}

ConsoleGfx::~ConsoleGfx()
{
    delete[] fgPalette;
    delete[] emPalette;
    delete[] ulPalette;
    delete[] bgPalette;

    delete[] testRainbow;
    delete[] testImage;
}

void ConsoleGfx::displayImage(unsigned char *imageData)
{
    int width = imageData[0];
    int height = imageData[1];
    int dataIndex = 2;

    cout << TOP_LEFT;
    for (int xIndex = 0; xIndex < width; xIndex += 1)
        cout << TOP;

    cout << TOP_RIGHT << "\n";

    for (int yIndex = 0; yIndex < height; yIndex += 2)
    {
        cout << LEFT;
        for (int xIndex = 0; xIndex < width; xIndex += 1)
        {
            int outputColor = imageData[dataIndex];
            cout << fgPalette[outputColor == CLEAR ? TRANS_DISPLAY : outputColor];
            outputColor = yIndex + 1 < height ? imageData[dataIndex + width] : CLEAR;
            cout << bgPalette[outputColor == CLEAR ? TRANS_DISPLAY : outputColor];
            cout << BLOCK;
            dataIndex++;
        }
        dataIndex += width;
        cout << COLOR_RESET << RIGHT << "\n";
    }

    cout << BOTTOM_LEFT;
    for (int xIndex = 0; xIndex < width; xIndex += 1)
        cout << BOTTOM;
    cout << BOTTOM_RIGHT << "\n";
}

unsigned char *ConsoleGfx::loadFile(string filename)
{
    ifstream inFile;
    char *buffer = nullptr;

    inFile.open(filename, ifstream::binary);

    if (inFile.is_open())
    {
        inFile.seekg(0, inFile.end);
        int length = inFile.tellg();
        inFile.seekg(0, inFile.beg);

        buffer = new char[length];
        inFile.read(buffer, length);
        inFile.close();
    }

    return (unsigned char*) buffer;
}
