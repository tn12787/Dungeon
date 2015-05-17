#ifndef D_TYPES
#define D_TYPES
struct backgroundData;
typedef struct backgroundData backgroundData;

struct Option;
typedef struct Option Option;
struct OptionMenu;
typedef struct OptionMenu OptionMenu;
typedef char inputKey;
struct Point;
typedef struct Point point;

enum G_STATE {TITLE, MAINMENU, SETTINGS, PREPLAY, PREMENU, PLAY, MONSTER, TRAP, VICTORY, QUIT, PROBLEM};
class Enemy;
class GameInstance;
#endif
