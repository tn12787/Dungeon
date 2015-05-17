#ifndef D_GAME
#define D_GAME
#include "D_Headers.hpp"

using namespace std;

extern int level;
extern int maximumLevel;

struct Point
{
    int x;
    int y;
};

const point DEF_SPAWN_TL = {2,0};
const point DEF_SPAWN_BR = {9,9};
const int DEF_SPAWN_FIRST = 6;
const int DEF_SPAWN_RATE = 5;

class Enemy
{
    public:
        Enemy();
        Enemy( int x, int y );
        void mobMove(int board[10][10], int pxpos, int pypos, int opxpos, int opypos);
        int getX();
        int getY();
    private:
        int xpos;
        int ypos;
        void stdMove(int board[10][10]);
        void chaseMove(int board[10][10], int pxpos, int pypos);
        void typechaseMove(int board[10][10], int pxpos, int pypos, int opxpos, int opypos);
};

class GameInstance
{
    public:
        GameInstance();
        void mapreset(int level);
        void mapprint(HWND hwnd);
        void playerMove(char direction);
        char playagain(inputKey input);
        char nextLevel(inputKey input);
        void enemygen();
        void moveEnemies();
        int getCurrentTile();
    private:
        int xpos;
        int ypos;
        int oxpos;
        int oypos;
        int board[10][10];
        int turncount;
        int firstSpawn;
        int spawnRate;
        vector<point> spawnLoc;
        vector<Enemy> enemylist;
};

char runTurn(GameInstance &game, inputKey input);
G_STATE runLoop( G_STATE state, GameInstance &game, char &selection, inputKey input,
    irrklang::ISoundEngine* audio, irrklang::ISound** music, time_t &pause,
    bool &redraw);
void bootNewLevels(string name, int maxLevel);



#endif
