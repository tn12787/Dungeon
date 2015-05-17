#ifndef D_INTERFACE
#define D_INTERFACE
#include "D_Headers.hpp"

using namespace std;

const int W_WIDTH = 480;
const int W_HEIGHT = 480;

const string MENU_MAIN[] = {
    "Play",
    "Settings",
    "Level Select/Load",
    "Quit"
};

struct Option
{
    char type;
    unsigned value;
};

struct OptionMenu
{
    string* options;
    Option* values;
};

char plgnConvert(inputKey input);
char nxtConvert(inputKey input);
char titleConvert(inputKey input);
char menuConvert(inputKey input);
char directionConvert(inputKey input);
char menulaunch(inputKey input);
inputKey processInput( WPARAM key );
void stopMusic(irrklang::ISound** music);
void Display(G_STATE state, GameInstance &game, HWND hwnd, int bgCurrent,
    char selection = 0);

#endif
