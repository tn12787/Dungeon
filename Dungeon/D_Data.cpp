#include "D_Headers.hpp"

using namespace std;

backgroundData bgAssets;
int bgCurrent = BG_TITLE;
string *levelData = 0;

string intToString(int value){
    ostringstream ss;
    ss << value;
    return ss.str();
}

void loadAssets(){
    bgAssets.hTitleBackground = (HBITMAP)LoadImage(NULL,A_BG_TITLE.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    bgAssets.hMenuBackground = (HBITMAP)LoadImage(NULL,A_BG_MENU.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    for(int c = 0; c < BG_COUNT; c++){
        string bgPath = A_BG_LEVELS;
        string num = intToString(c+1);
        size_t cPos = bgPath.find_last_of('$');
        bgPath.erase(cPos,1);
        bgPath.insert(cPos,num);
        bgAssets.hGameBackground[c] = (HBITMAP)LoadImage(NULL,bgPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
}

void newRandomLevelBG(){
    if(bgCurrent >= BG_COUNT){
        int random = (rand() % BG_COUNT);
        bgCurrent = random;
    }
    else{
        int random = (rand() % (BG_COUNT-1));
        if(random >= bgCurrent) random++;
        bgCurrent = random;
    }
}

void save(int level)
{
    ofstream saveFile("savegame.sav");
    saveFile << level;
    saveFile.close();
}

void load(int &level, GameInstance &game)
{
    ifstream loader("savegame.sav");
    char lev;
    if(loader)
    {
        lev = loader.get();
        level = lev - '0';
    }
    else level = 1;
    game.mapreset(level);
}

void loadLevelData(string name, int maxLevel){
    maximumLevel = maxLevel;
    levelData = new string[maxLevel];
    for(unsigned level = 1; level <= maxLevel; level++)
    {
        stringstream ssLevelName;
        ssLevelName << PATH_MAIN << name << level << ".dat";
        string levelName = ssLevelName.str();

        ifstream fs(levelName.c_str());

        while(fs.good())
        {
            char c = fs.get();
            levelData[level-1] = levelData[level-1] + c;
        }
    }
}

