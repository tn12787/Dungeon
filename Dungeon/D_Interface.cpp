#include "D_Headers.hpp"

using namespace std;

char plgnConvert(inputKey input)
{
    if (input == 25)
    {
        return 'y';
    }

    else if (input == 14)
    {
        return 'n';
    }

    else return 0;
}

char nxtConvert(inputKey input)
{
    if (input == 5)
    {
        return 'e';
    }

    else return 0;
}

char titleConvert(inputKey input)
{
    if (input == 5)
    {
        return 1;
    }
    else return 0;
}

char menuConvert(inputKey input)
{
    if(input == 5)
    {
        return 1;
    }
    else if(input == 28)
    {
        return 2;
    }
    else if(input == 30)
    {
        return 3;
    }
    return 0;
}

char directionConvert(inputKey input)
{
    if (input == 28)
    {
        return 'u';
    }
    else if (input == 29)
    {
        return 'r';
    }
    else if (input == 30)
    {
        return 'd';
    }
    else if (input == 27)
    {
        return 'l';
    }
    else return 0;
}

// Return values:
// 0: Do nothing
// 1: Go to game
char menulaunch(inputKey input)
{
    return titleConvert(input);
}

//Key Codes:
//  1-26:   A-Z
//  27-30:  Left-Up-Right-Down
//  31-40:  Numpad 0-9
inputKey processInput( WPARAM key )
{
    inputKey result;
    if( key <= 0x24 )
    {
        result = 0;
    }
    else if( key <= 0x28 )
    {
        result = key - 10;
    }
    else if( key <= 0x40 )
    {
        result = 0;
    }
    else if( key <= 0x5A )
    {
        result = key - 64;
    }
    else if( key <= 0x5F )
    {
        result = 0;
    }
    else if( key <= 0x69)
    {
        result = key - 65;
    }
    else result = 0;

    return result;
}

void stopMusic(irrklang::ISound** music){
    if(*music != 0){
        (*music)->stop();
        (*music)->drop();
        *music = 0;
    }
}

void Display(G_STATE state, GameInstance &game, HWND hwnd, int bgCurrent,
    char selection)
{
    if(state == PREPLAY || state == PREMENU) return;

    HDC hdc;
    hdc = GetDC(hwnd);

    RECT screenRect;
    SetRect(&screenRect, 0, 0, W_WIDTH, W_HEIGHT);
    InvalidateRect(hwnd,&screenRect,true);
    UpdateWindow(hwnd);


    RECT rc;
    HRGN hrect;
    HFONT hFont, hOldFont;
    hFont = (HFONT)GetStockObject(ANSI_FIXED_FONT);
    string print;
    HBRUSH blackBrush;
    blackBrush = CreateSolidBrush(RGB(0,0,0));
    HBRUSH whiteBrush;
    whiteBrush = CreateSolidBrush(RGB(255,255,255));
    HBITMAP bgImage = bgAssets.getBackground(bgCurrent);

    BITMAP bitmap;
    HDC hdcMem = CreateCompatibleDC(hdc);
    HGDIOBJ oldBitmap = SelectObject(hdcMem, bgImage);
    GetObject(bgImage, sizeof(bitmap), &bitmap);
    BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);

  /*  char* statetext = new char[1];
    sprintf(statetext, "%hhu", state);
    RECT tre;
    SetRect( &tre, 10, 10, 60, 60 );
    DrawText(hdc,statetext,1,&tre,0);

  */  

    if ( (hOldFont = (HFONT)SelectObject(hdc, hFont)) )
    {
        if (state == MAINMENU) {
            RECT menuRect;
            SetRect(&menuRect, (W_WIDTH/2)-150, (W_HEIGHT/2)-150, (W_WIDTH/2)+150, (W_HEIGHT/2)+150);
            unsigned int options = DT_CENTER;
            string print;
            for(int count = 0; count < 4; count++)
            {
                if( count == selection ) print += "> ";
                else print += "  ";
                print += MENU_MAIN[count];
                print += "  ";
                if( count != 3 ) print += '\n';
            }
            char *output = new char[print.length()];
            strcpy( output, print.c_str() );
            SetBkMode(hdc, TRANSPARENT);
            DrawText(hdc,output,print.length(),&menuRect,options);
        }
        else if(state == PLAY || state == VICTORY || state == MONSTER || state == TRAP)
        {
            RECT mapRect;
            SetRect( &mapRect, 120, 120, 350, 255 );
            HRGN mapRgn = CreateRectRgnIndirect(&mapRect);
            RECT borderRect;
            SetRect( &borderRect, 115, 115, 355, 260 );
            HRGN borderRgn = CreateRectRgnIndirect(&borderRect);
            FillRgn(hdc,borderRgn,blackBrush);
            FillRgn(hdc,mapRgn,whiteBrush);

            SetRect( &rc, 115, 300, 355, 420 );
            hrect = CreateRectRgnIndirect(&rc);
            game.mapprint(hwnd);
            if(state != PLAY){
                SetTextColor(hdc, 0x0000AF00);
                if (state == VICTORY)
                {
                    print = "\nLevel Complete!";
                    print += "\n\nPress E to continue.";
                }
                else if (state == TRAP)
                {
                    print = "You stepped on a Trap!\nYou have absolutely no tek. Come on.\nGAME OVER";
                    print += "\n\nPlay Again? (Y/N)";
                }
                else if (state == MONSTER)
                {
                    print = "You were eaten by a grue.\nYou have no tek.\nGAME OVER";
                    print += "\n\nPlay Again? (Y/N)";
                }
                char *output = new char[print.length() + 1];
                strcpy( output, print.c_str() );
                unsigned int options =  DT_CENTER | DT_WORDBREAK;
                SetBkMode(hdc, TRANSPARENT);
                FillRgn(hdc, hrect, blackBrush);
                DrawText(hdc,output,print.length(),&rc,options);
                SelectObject(hdc, hOldFont);
            }
        }
    }
    UpdateWindow(hwnd);
}
