#include "D_Headers.hpp"

using namespace std;

int level = 1;
int maximumLevel = 1;

Enemy::Enemy()
{
    xpos = 5;
    ypos = 5;
}

Enemy::Enemy( int x, int y )
{
    xpos = x;
    ypos = y;
}

int Enemy::getX()
{
    return xpos;
}

int Enemy::getY()
{
    return ypos;
}

void Enemy::stdMove(int board[10][10])
{
    int dir = rand() % 5;
    int cant_move = 0;
    while(true)
    {

        int nextx,nexty;
        nextx = xpos;
        nexty = ypos;
        switch(dir)
        {
            case 1:

               if (nexty == 0) {}
               else --nexty;
               break;

            case 2:

               if (nextx == 9) {}
               else ++nextx;
               break;

            case 3:

               if (nexty == 9) {}
               else ++nexty;
               break;

            case 4:

               if (nextx == 0) {}
               else --nextx;
               break;

            default:
               break;
        }

        if (board[nextx][nexty] == 5 || board[nextx][nexty] == 10 || board[nextx][nexty] == 20)
        {
            cant_move++;
            dir = (dir + 1) % 5;
            if (cant_move == 5)
            {
                break;
            }
        }

        else
        {
            board[xpos][ypos] = 0;
            xpos = nextx;
            ypos = nexty;
            board[xpos][ypos] = 20;
            break;
        }
    }

}

void Enemy::chaseMove(int board[10][10], int pxpos, int pypos)
{
     int xdiff,ydiff,nextx,nexty;
     xdiff = abs(xpos-pxpos);
     ydiff = abs(ypos-pypos);
     nextx = xpos;
     nexty = ypos;

     if (xdiff > ydiff)
     {
         if (xpos > pxpos)
         {
           if (nextx == 0) {}
           else --nextx;
         }
         else if (xpos < pxpos)
         {  if (nextx == 9) {}
            else ++nextx;
         }

         else {}
     }

     else
     {
         if (ypos > pypos)
         {
           if (nexty == 0) {}
           else --nexty;
         }
         else if (ypos < pypos)
         {  if (nexty == 9) {}
            else ++nexty;
         }

         else {}
     }

     if (board[nextx][nexty] == 5 || board[nextx][nexty] == 10 || board[nextx][nexty] == 20)
     {
        stdMove(board);
     }

     else
     {
        board[xpos][ypos] = 0;
        xpos = nextx;
        ypos = nexty;
        board[xpos][ypos] = 20;
     }
}

void Enemy::typechaseMove(int board[10][10], int pxpos, int pypos, int opxpos, int opypos)
{
     int totaldiff,xdiff,ydiff;
     xdiff = abs(xpos-pxpos);
     ydiff = abs(ypos-pypos);
     totaldiff = xdiff + ydiff;
     if (totaldiff > 1)
     {
     chaseMove(board,opxpos,opypos);
     }

     else if (totaldiff == 0) {}

     else chaseMove(board,pxpos,pypos);
}

void Enemy::mobMove(int board[10][10], int pxpos, int pypos, int opxpos, int opypos)
{
  if (((abs(xpos - pxpos) + abs(ypos - pypos))) < 5 )
  {
    typechaseMove(board,pxpos,pypos,opxpos,opypos);
  }

  else stdMove(board);
}

GameInstance::GameInstance()
{
    mapreset(level);
    //cout << firstSpawn << endl;
    //cout << spawnRate << endl;
    //cout << spawnLoc.size() << endl;
}

int GameInstance::getCurrentTile()
{
    return board[xpos][ypos];
}

void GameInstance::enemygen()
{
    vector<point> openSpaces;
    for(vector<point>::iterator it = spawnLoc.begin();
        it != spawnLoc.end(); it++)
    {
        if(board[it->x][it->y] == 0){
            int xdiff = abs(it->x - xpos);
            int ydiff = abs(it->y - ypos);
            if((xdiff + ydiff) > 1) openSpaces.push_back(*it);
        }
    }
    if(openSpaces.size() > 0){
        int randPos = rand() % openSpaces.size();
        int expos = openSpaces[randPos].x;
        int eypos = openSpaces[randPos].y;
        Enemy enemy(expos,eypos);
        enemylist.push_back(enemy);
        board[expos][eypos] = 20;
    }
}

void GameInstance::mapreset(int level)
{
    xpos = 0;
    ypos = 0;
    oxpos = 0;
    oypos = 0;
    turncount = 1;
    firstSpawn = -1;
    spawnRate = -1;
    int i,j;
    for (i=0; i<10;i++)
    {
        for (j=0;j<10; j++)
        {
            board[j][i] = 0;
        }
    }

    if(levelData == 0) return;
    string levelFile = levelData[level-1];
    int levelSize = levelFile.length();

    int x = 0;
    int y = 0;
    int bitType = 0;
    firstSpawn = 0;
    spawnRate = 0;
    vector<point> spawnBoxes;

    for(int i = 0; i<levelSize; i += 2)
    {
        x = levelFile[i];
        y = levelFile[i+1];


        if (x == -1 && y == -1)
        {
            bitType++;
        }

        else if (bitType == 0)
        {
            board[x][y] = 5;
        }
        else if (bitType == 1)
        {
            board[x][y] = 10;
        }
        else if (bitType == 2)
        {
            firstSpawn = x;
            spawnRate = y;
        }
        else if (bitType == 3)
        {
            point next = {x, y};
            //cout << "New box: " << x << ", " << y << endl;
            spawnBoxes.push_back(next);
        }
    }

    //cout << "Spawnbox size: " << spawnBoxes.size() << endl;
    if(spawnBoxes.size() == 0){
        //cout << "Defaulting" << endl;
        spawnBoxes.push_back(DEF_SPAWN_TL);
        spawnBoxes.push_back(DEF_SPAWN_BR);
    }
    if(firstSpawn == -1) firstSpawn = DEF_SPAWN_FIRST;
    if(spawnRate == -1) spawnRate = DEF_SPAWN_RATE;

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            if(board[i][j] == 0){
                size_t k = 0;
                bool valid = false;
                while(k < spawnBoxes.size() && !valid){
                    if(i >= spawnBoxes[k].x   && j >= spawnBoxes[k].y &&
                       i <= spawnBoxes[k+1].x && j <= spawnBoxes[k+1].y)
                    {
                        point validSpawn = {i, j};
                        spawnLoc.push_back(validSpawn);
                        valid = true;
                    }
                    k += 2;
                }
            }
        }
    }

    enemylist.clear();
    return;
}

char GameInstance::playagain(inputKey input)
{
    char playagain = plgnConvert(input);
    if (playagain == 'y')
    {
        return 2;
    }
    else if (playagain == 'n')
    {
        return 1;
    }
    return 0;
}

char GameInstance::nextLevel(inputKey input)
{
    char nextLevel = nxtConvert(input);
    if (nextLevel == 'e')
    {
        return 1;
    }
    else return 0;
}

void GameInstance::mapprint(HWND hwnd)
{
    int i,j;
    string print;
    HFONT hFont, hOldFont;
    hFont = (HFONT)GetStockObject(ANSI_FIXED_FONT);
    HDC hdc;
    hdc = GetDC(hwnd);
    SetTextColor(hdc, 0x00000000);

    if ( (hOldFont = (HFONT)SelectObject(hdc, hFont)) )
    {

        for (i=0; i<10;i++)
        {
            for (j=0;j<10; j++)
            {
                if (board[j][i] == 10)
                {
                    print += "X  ";
                }
                else if (board[j][i] == 5)
                {
                    print += "T  ";
                }
                else if (board[j][i] == 20)
                {
                    print += "M  ";
                }
                else if( i == ypos && j == xpos )
                {
                    print += "P  ";
                }
                else if (board[j][i] == 0)
                {
                    print += ".  ";
                }
                else
                {
                    print += "E  ";
                }
            }
            print += "\n";
        }
        unsigned int options =  DT_CENTER | DT_WORDBREAK;
        SetBkMode(hdc, TRANSPARENT);
        RECT rc;
        SetRect( &rc, 120, 120, 360, 360 );
        char *output = new char[print.length() + 1];
        strcpy( output, print.c_str() );
        DrawText(hdc,output,print.length(),&rc,options);
        UpdateWindow(hwnd);
    }
}

void GameInstance::playerMove(char direction)
{
    oxpos = xpos;
    oypos = ypos;
    if (direction == 'u')
    {
       if (ypos == 0) {}
       else --ypos;
    }
    else if (direction == 'r')
    {
       if (xpos == 9) {}
       else ++xpos;
    }
    else if (direction == 'd')
    {
       if (ypos == 9) {}
       else ++ypos;
    }
    else if (direction == 'l')
    {
       if (xpos == 0) {}
       else --xpos;

    }
    int adjustedTurn = turncount - firstSpawn;
    if (spawnRate == 0 || firstSpawn == 0) {}
    else if(adjustedTurn >= 0 && adjustedTurn % spawnRate == 0) enemygen();
    ++turncount;
}

void GameInstance::moveEnemies()
{
    for (vector<Enemy>::iterator i = enemylist.begin(); i != enemylist.end(); ++i)
    {
        i->mobMove(board,xpos,ypos,oxpos,oypos);
    }
}

// Return values:
// 0: No Event
// 1: Trap (& Dank)
// 2: Monster
// 3: Victory
char runTurn(GameInstance &game, inputKey input)
{
    //char direction;
    char direction = directionConvert(input);
    if (direction != 0)
    {
        game.playerMove(direction);
        game.moveEnemies();
        int currentTile = game.getCurrentTile();
        if (currentTile == 5 || currentTile == 20 || currentTile == 10 )
        {
            if (currentTile == 5 )
            {
                return 1;
            }

            else if (currentTile == 10)
            {
                return 3;
            }

            else return 2;
        }
    }

    return 0;
}

//State values:
//  TITLE:      Pregame menu
//  PLAY:       Playing game
//  TRAP:       Died to a trap
//  MONSTER:    Died to a grue
//  VICTORY:    Won level
//  QUIT:       Exit game
//  PROBLEM:    Exit game due to error - Current state is invalid
//  Reset game state, immediately goes back to PLAY
//  Reset game state, immediately goes back to MAINMENU
G_STATE runLoop( G_STATE state, GameInstance &game, char &selection,
                inputKey input, irrklang::ISoundEngine* audio,
                irrklang::ISound** music, time_t &pause, bool &redraw)
{
    switch( state )
    {
        case TITLE:
        {
            char result = menulaunch(input);
            switch( result )
            {
                case 0:
                {
                    return TITLE;
                    break;
                }
                case 1:
                {
                    stopMusic(music);
                    irrklang::ISound** loadjingle;
                    *loadjingle = audio->play2D(A_M_START.c_str(),false,false,true);
                    pause = time(NULL) + 1;
                    return PREMENU;
                    break;
                }
            }
            break;
        }
        case MAINMENU:
        {
            char result = menuConvert(input);
            switch( result )
            {
                case 0:
                {
                    return MAINMENU;
                    break;
                }
                case 1:
                {
                    switch(selection)
                    {
                        case 0:
                        {
                            stopMusic(music);
                            *music = audio->play2D(A_M_START.c_str(),false,false,true);
                            pause = time(NULL) + 1;
                            return PREPLAY;
                            break;
                        }
                        case 1:
                        {
                            return MAINMENU;
                            break;
                        }
                        case 2:
                        {
                            irrklang::ISound** loadjingle;
                            *loadjingle = audio->play2D(A_M_LOAD.c_str(),false,false,true);
                            load(level, game);
                            return MAINMENU;
                            break;
                        }
                        case 3:
                        {
                            return QUIT;
                            break;
                        }
                    }
                    break;
                }
                case 2:
                {
                    selection--;
                    if( selection < 0 ) selection += 4;
                    return MAINMENU;
                    break;
                }
                case 3:
                {
                    selection++;
                    if( selection > 3 ) selection -= 4;
                    return MAINMENU;
                    break;
                }
            }
            break;
        }
        case SETTINGS:
        {
            return PROBLEM;
        }
        case PREPLAY:
        {
            game = GameInstance();
            stopMusic(music);
            *music = audio->play2D(A_M_TITLE.c_str(),false,false,true);
            newRandomLevelBG();
            redraw = true;
            return PLAY;
        }
        case PREMENU:
        {
            stopMusic(music);
            *music = audio->play2D(A_M_MENU.c_str(),false,false,true);
            bgCurrent = BG_MENU;
            redraw = true;
            return MAINMENU;
        }
        case PLAY:
        {
            char result = runTurn(game, input);
            switch( result )
            {
                case 0:
                {
                    return PLAY;
                    break;
                }
                case 1:
                {
                    stopMusic(music);
                    *music = audio->play2D(A_M_DEATH.c_str(),false,false,true);
                    pause = time(NULL) + 1;
                    level = 1;
                    save(level);
                    return TRAP;
                    break;
                }
                case 2:
                {
                    stopMusic(music);
                    *music = audio->play2D(A_M_DEATH.c_str(),false,false,true);
                    pause = time(NULL) + 1;
                    level = 1;
                    save(level);
                    return MONSTER;
                    break;
                }
                case 3:
                {
                    stopMusic(music);
                    *music = audio->play2D(A_M_WIN.c_str(),false,false,true);
                    pause = time(NULL) + 3;
                    level++;
                    save(level);
                    return VICTORY;
                    break;
                }
            }
        }
        case MONSTER:
        {
            if (game.playagain(input) == 1){
                return PREMENU;
            }
            else if (game.playagain(input) == 0) return MONSTER;
            return PREPLAY;
            break;
        }

        case TRAP:
        {

            if (game.playagain(input) == 1){
                return PREMENU;
            }
            else if (game.playagain(input) == 0) return TRAP;
            return PREPLAY;
            break;
        }
        case VICTORY:
        {
            if (game.nextLevel(input) == 1)
            {
                return PREPLAY;
            }
            else return VICTORY;
            break;
        }
        case PROBLEM:
        {
            //cout << "There's a problem.\n";
            return PROBLEM;
            break;
        }
        case QUIT:
        {
            return QUIT;
            break;
        }
    }
    return PROBLEM;
}

