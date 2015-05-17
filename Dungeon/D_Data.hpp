#ifndef D_DATA
#define D_DATA
#include "D_Headers.hpp"

using namespace std;

const string PATH_MAIN = "C:/Users/Tom/Desktop/Dev/Dungeon/";
const string PATH_ASSETS = PATH_MAIN + "Assets/";
const string A_M_TITLE = PATH_MAIN + "Dungeon OST - Title Theme.mp3";
const string A_M_MAIN_SCR = PATH_MAIN + "Dungeon OST - Main Screen Theme.mp3";
const string A_M_WIN = PATH_MAIN + "Dungeon OST - Victory Music.wav";
const string A_M_DEATH = PATH_MAIN + "Dungeon OST - Death Music.wav";
const string A_M_START = PATH_MAIN + "Dungeon OST - Jingle.mp3";
const string A_M_LOAD = PATH_MAIN + "Dungeon OST - Load Jingle.mp3";
const string A_M_MENU = PATH_MAIN + "Dungeon OST - Menu Theme.mp3";
const string A_BG_TITLE = PATH_ASSETS + "TitlePic.bmp";
const string A_BG_MENU = PATH_ASSETS + "mscr.bmp";
const string A_BG_LEVELS = PATH_ASSETS + "scr$.bmp";

const int BG_COUNT = 5;
const int BG_TITLE = BG_COUNT;
const int BG_MENU = BG_TITLE + 1;
struct backgroundData{
    HBITMAP hTitleBackground;
    HBITMAP hMenuBackground;
    HBITMAP hGameBackground[BG_COUNT];
    HBITMAP getBackground(int bg){
        if(bg < 0){
            return hTitleBackground;
        }
        else if(bg < BG_COUNT){
            return hGameBackground[bg];
        }
        else if(bg == BG_TITLE){
            return hTitleBackground;
        }
        else if(bg == BG_MENU){
            return hMenuBackground;
        }
        else return hTitleBackground;
    }
};
extern backgroundData bgAssets;
extern int bgCurrent;

extern string *levelData;

void loadAssets();
void newRandomLevelBG();
void save(int level);
void load(int &level, GameInstance &game);
void loadLevelData(string name, int maxLevel);

#endif
