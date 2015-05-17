#include "D_Headers.hpp"

using namespace std;

const DWORD W_STYLE = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
const DWORD W_EXSTYLE = WS_EX_CLIENTEDGE;
const char g_szClassName[] = "myWindowClass";

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstsance,
    LPSTR lpCmdLine, int nCmdShow)
{
    srand(time(NULL));
    loadAssets();
    bgCurrent = BG_TITLE;
    level = 1;
    time_t pause = 0;


    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
    if (!engine)
    {
        MessageBox(NULL, "Could not start up engine!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0; // error starting up the engine
    }
    else printf("Engine startup successful\n");

    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    RECT windowSize = {0,0,W_WIDTH,W_HEIGHT};
    AdjustWindowRectEx(&windowSize,W_STYLE,false,W_EXSTYLE);

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        W_EXSTYLE,
        g_szClassName,
        "DUNGEON",
        W_STYLE,
        CW_USEDEFAULT, CW_USEDEFAULT, windowSize.right-windowSize.left, windowSize.bottom-windowSize.top,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    G_STATE state = TITLE;
    inputKey input = 0;
    bool running = true;
    GameInstance game;
    char selection = 0;
    loadLevelData("level", 5);

    irrklang::ISound* music;
    music = engine->play2D(A_M_MAIN_SCR.c_str(), false, false, true);
    // Step 3: The Message Loop
    Display(state, game, hwnd, bgCurrent);
    while( running )
    {
        if( PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE))
        {
            if( Msg.message == WM_KEYDOWN )
            {
                input = processInput(Msg.wParam);
            }
            if( Msg.message == WM_QUIT )
            {
                running = false;
            }
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        bool redraw = (bool)input;
        if (pause == 0) state = runLoop(state,game,selection,input,engine,
                                        &music,pause,redraw);
        else if (time(NULL) > pause)
        {
            pause = 0;
        }
        else {}
        if (state == PROBLEM || state == QUIT) break;
        if(redraw) Display(state, game, hwnd, bgCurrent, selection);
        input = 0;
    }

    engine->drop();
    return Msg.wParam;
}
