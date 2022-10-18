#pragma once

#include <string>
#include <unistd.h>

using std::string;

class ConsoleGfx
{
public:
    // Framing (Windows / DOS mode)
    const char TOP = 205, TOP_LEFT = 201, TOP_RIGHT = 187, LEFT = 186,
               RIGHT = 186, BOTTOM = 205, BOTTOM_LEFT = 200, BOTTOM_RIGHT = 188, BLOCK = 223;

    // Framing (Unix mode)
//     const string TOP = "═", TOP_LEFT = "╔", TOP_RIGHT = "╗", LEFT = "║",
//                  RIGHT = "║", BOTTOM = "═", BOTTOM_LEFT = "╚", BOTTOM_RIGHT = "╝", BLOCK = "▀";

    // Colors
    enum Color: uint8_t { BLACK = 0, RED, DARK_GREEN, GOLD, BLUE, GARNETT, ORANGE, LIGHT_GRAY,
                          GRAY, PEACH, GREEN, BRIGHT_GOLD, CYAN, MAGENTA, BRIGHT_ORANGE, WHITE };

    // Special symbols
    const uint8_t CLEAR = MAGENTA, TRANS_DISPLAY = BLACK;
    const string COLOR_RESET = "\033[0m";  // Text Reset

    // Tests
    unsigned char* testRainbow; // Test rainbow
    unsigned char* testImage; // Test image

    inline static ConsoleGfx* getInstance() { return instance ? instance : instance = new ConsoleGfx(); }
    void displayImage(unsigned char *imageData);
    unsigned char *loadFile(string filename);

private:
    // Palettes
    string *fgPalette, *emPalette, *ulPalette, *bgPalette;

    // Singleton instance
    static ConsoleGfx* instance;

    // Constructor (private; singleton)
    ConsoleGfx();
	~ConsoleGfx();
};
