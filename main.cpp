#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

#define H 30
#define GAME_W 30

int OFFSET_X;
int OFFSET_Y;

char board[H][GAME_W] = {};

char blocks[][4][4] = {

    {
        {' ','I',' ',' '},
        {' ','I',' ',' '},
        {' ','I',' ',' '},
        {' ','I',' ',' '}
    },

    {
        {' ',' ',' ',' '},
        {'I','I','I','I'},
        {' ',' ',' ',' '},
        {' ',' ',' ',' '}
    },

    {
        {' ',' ',' ',' '},
        {' ','O','O',' '},
        {' ','O','O',' '},
        {' ',' ',' ',' '}
    },

    {
        {' ','T',' ',' '},
        {'T','T','T',' '},
        {' ',' ',' ',' '},
        {' ',' ',' ',' '}
    },

    {
        {' ','S','S',' '},
        {'S','S',' ',' '},
        {' ',' ',' ',' '},
        {' ',' ',' ',' '}
    },

    {
        {'Z','Z',' ',' '},
        {' ','Z','Z',' '},
        {' ',' ',' ',' '},
        {' ',' ',' ',' '}
    },

    {
        {'J',' ',' ',' '},
        {'J','J','J',' '},
        {' ',' ',' ',' '},
        {' ',' ',' ',' '}
    },

    {
        {' ',' ','L',' '},
        {'L','L','L',' '},
        {' ',' ',' ',' '},
        {' ',' ',' ',' '}
    }
};

int x = (GAME_W - 2) / 2;
int y = 0;
int b = 0;
int nextBlock = 1;

int score = 0;
int highScore = 0;
int level = 1;
int linesCleared = 0;

void gotoxy(int x, int y){
    COORD c = {SHORT(x), SHORT(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void boardDelBlock(){

    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ')
                board[y+i][x+j] = ' ';
}

void block2Board(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ')
                board[y+i][x+j] = blocks[b][i][j];
}

void initBoard(){
    for (int i = 0 ; i < H ; i++){
        for (int j = 0 ; j < GAME_W ; j++){
            if (i == H-1){
                if (j == 0)
                    board[i][j] = '<';
                else if (j == 1)
                    board[i][j] = '!';
                else if (j == GAME_W-2)
                    board[i][j] = '!';
                else if (j == GAME_W-1)
                    board[i][j] = '>';
                else
                    board[i][j] = '=';
            }
            else if (j == 0)
                board[i][j] = '<';
            else if (j == 1)
                board[i][j] = '!';
            else if (j == GAME_W-2)
                board[i][j] = '!';
            else if (j == GAME_W-1)
                board[i][j] = '>';
            else
                board[i][j] = ' ';
        }
    }
}

bool canMove(int dx, int dy){
    for (int i = 0 ; i < 4 ; i++){
        for (int j = 0 ; j < 4 ; j++){
            if (blocks[b][i][j] != ' '){
                int tx = x + j + dx;
                int ty = y + i + dy;
                if (tx < 2 || tx >= GAME_W-2 || ty >= H-1)
                    return false;
                if (board[ty][tx] != ' ')
                    return false;
            }
        }
    }

    return true;
}

void rotateBlock() {
    boardDelBlock(); 
    char rotated[4][4]; 
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            rotated[j][3 - i] = blocks[b][i][j];

    
    int kick[] = { 0, 1, -1, 2, -2 };
    int newX = x;
    bool canRotate = false;

    for (int k = 0; k < 5 && !canRotate; k++) {
        bool ok = true;
        int testX = x + kick[k];

        for (int i = 0; i < 4 && ok; i++) {
            for (int j = 0; j < 4; j++) {
                if (rotated[i][j] != ' ') {
                    int tx = testX + j;
                    int ty = y + i;
                    
                    if (tx < 2 || tx >= GAME_W - 2 || ty >= H - 1 || board[ty][tx] != ' ') {
                        ok = false;
                        break;
                    }
                }
            }
        }
        if (ok) {
            canRotate = true;
            newX = testX;
        }
    }
    if (canRotate) {
        x = newX;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                blocks[b][i][j] = rotated[i][j];
    }
}
void hideCursor()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO info;

    info.dwSize = 100;
    info.bVisible = FALSE;

    SetConsoleCursorInfo(h, &info);
}
void draw();

void removeLine(){
    int linesThisTurn = 0;
    bool fullRows[H] = {false};
   
    for (int i = H-2 ; i > 0 ; i--){
        int j;
        for (j = 2 ; j < GAME_W-2 ; j++){
            if (board[i][j] == ' ') break;
        }
        if (j == GAME_W-2){
            fullRows[i] = true;
            linesThisTurn++;
        }
    }
    if (linesThisTurn == 0) return;
    
    for (int blink = 0 ; blink < 3 ; blink++){
        for (int i = 1; i < H-1; i++) {
            if (fullRows[i]) {
                for (int j = 2 ; j < GAME_W-2 ; j++) board[i][j] = '#';
            }
        }
        draw();
        Sleep(100);

        for (int i = 1; i < H-1; i++) {
            if (fullRows[i]) {
                for (int j = 2 ; j < GAME_W-2 ; j++) board[i][j] = ' ';
            }
        }
        draw();
        Sleep(100);
    }
  
    int writeRow = H - 2;
    for (int readRow = H - 2; readRow > 0; readRow--) {
        if (!fullRows[readRow]) {
            for (int j = 2; j < GAME_W - 2; j++) {
                board[writeRow][j] = board[readRow][j];
            }
            writeRow--;
        }
    }
    for (; writeRow > 0; writeRow--) {
        for (int j = 2; j < GAME_W - 2; j++) {
            board[writeRow][j] = ' ';
        }
    }
  
    int points = 0;
    if(linesThisTurn == 1) points = 100;
    else if(linesThisTurn == 2) points = 300;
    else if(linesThisTurn == 3) points = 500;
    else if(linesThisTurn >= 4) points = 800;

    int oldLevel = level;
    score += points;
    linesCleared += linesThisTurn;
  
    if (score > highScore) highScore = score;
    level = (score / 500) + 1;
  
    if (level > oldLevel) {
        int notiX = OFFSET_X + GAME_W + 5;
        int notiY = OFFSET_Y + 20;
        for(int blink = 0; blink < 3; blink++) {
            gotoxy(notiX, notiY);     cout << "***************";
            gotoxy(notiX, notiY + 1); cout << "*  LEVEL UP!  *";
            gotoxy(notiX, notiY + 2); cout << "***************";
            Sleep(200);

            gotoxy(notiX, notiY);     cout << "               ";
            gotoxy(notiX, notiY + 1); cout << "               ";
            gotoxy(notiX, notiY + 2); cout << "               ";
            Sleep(200);
        }
    }
}

void drawNextBlock(){
    int infoX = OFFSET_X + GAME_W + 2;
    int infoY = OFFSET_Y + 11; 

    gotoxy(infoX, infoY);     cout << "╔══════════════════════╗";
    gotoxy(infoX, infoY + 1); cout << "║      NEXT BLOCK      ║";
    gotoxy(infoX, infoY + 2); cout << "╠══════════════════════╣";

    for (int i = 0; i < 4; i++) {
        gotoxy(infoX, infoY + 3 + i);
        cout << "║                      ║"; 
    }
    gotoxy(infoX, infoY + 7); cout << "╚══════════════════════╝";

    // Căn giữa khối Next Block vào trong khung
    int blockStartX = infoX + 10;
    int blockStartY = infoY + 3;

    for (int i = 0 ; i < 4 ; i++){
        gotoxy(blockStartX, blockStartY + i);
        for (int j = 0 ; j < 4 ; j++){
            cout << blocks[nextBlock][i][j];
        }
    }
}

void drawInfo(){
    int infoX = OFFSET_X + GAME_W + 2;
    int infoY = OFFSET_Y;

    // Vẽ toàn bộ bảng điều khiển nguyên khối
    gotoxy(infoX, infoY);     cout << "╔══════════════════════╗";
    gotoxy(infoX, infoY + 1); cout << "║        TETRIS        ║";
    gotoxy(infoX, infoY + 2); cout << "╠══════════════════════╣";
    gotoxy(infoX, infoY + 3); cout << "║ SCORE      :         ║";
    gotoxy(infoX, infoY + 4); cout << "╠══════════════════════╣";
    gotoxy(infoX, infoY + 5); cout << "║ HIGH SCORE :         ║";
    gotoxy(infoX, infoY + 6); cout << "╠══════════════════════╣";
    gotoxy(infoX, infoY + 7); cout << "║ LEVEL      :         ║";
    gotoxy(infoX, infoY + 8); cout << "╠══════════════════════╣";
    gotoxy(infoX, infoY + 9); cout << "║ LINES      :         ║";
    gotoxy(infoX, infoY + 10);cout << "╚══════════════════════╝";

    
    gotoxy(infoX + 15, infoY + 3); cout << "       ";
    gotoxy(infoX + 15, infoY + 3); cout << score;

    gotoxy(infoX + 15, infoY + 5); cout << "       ";
    gotoxy(infoX + 15, infoY + 5); cout << highScore;

    gotoxy(infoX + 15, infoY + 7); cout << "       ";
    gotoxy(infoX + 15, infoY + 7); cout << level;

    gotoxy(infoX + 15, infoY + 9); cout << "       ";
    gotoxy(infoX + 15, infoY + 9); cout << linesCleared;

    drawNextBlock();
}

void draw(){

    for (int i = 0 ; i < H ; i++){

        gotoxy(OFFSET_X, OFFSET_Y + i);

        for (int j = 0 ; j < GAME_W ; j++){

            if (board[i][j] == ' ') {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                cout << '.';
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            }
            else
                cout << board[i][j];
        }
    }

    drawInfo();
}

void drawMenu(){

    system("cls");
    system("color 0A");

    string logo[] = {

"        ,----,                 ,----,",
"      ,/   .`|               ,/   .`|",
"    ,`   .'  :   ,---,.    ,`   .'  :,-.----.     ,---,  .--.--.",
"  ;    ;     / ,'  .' |  ;    ;     /\\    /  \\ ,`--.' | /  /    '.",
".'___,/    ,',---.'   |.'___,/    ,' ;   :    \\|   :  :|  :  /`. /",
"|    :     | |   |   .'|    :     |  |   | .\\ ::   |  ';  |  |--`",
";    |.';  ; :   :  |-,;    |.';  ;  .   : |: ||   :  ||  :  ;_",
"`----'  |  | :   |  ;/|`----'  |  |  |   |  \\ :'   '  ; \\  \\    `.",
"    '   :  ; |   :   .'    '   :  ;  |   : .  /|   |  |  `----.   \\",
"    |   |  ' |   |  |-,    |   |  '  ;   | |  \\'   :  ;  __ \\  \\  |",
"    '   :  | '   :  ;/|    '   :  |  |   | ;\\  \\   |  ' /  /`--'  /",
"    ;   |.'  |   |    \\    ;   |.'   :   ' | \\.'   :  |'--'.     /",
"    '---'    |   :   .'    '---'     :   : :-' ;   |.'   `--'---'",
"             |   | ,'                |   |.'   '---'",
"             `----'                  `---'"
    };

    size_t longest = 0;

    for (int i = 0 ; i < 15 ; i++){

        if (logo[i].length() > longest)
            longest = logo[i].length();
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int consoleWidth =
        csbi.srWindow.Right - csbi.srWindow.Left + 1;

    int startX = (consoleWidth / 2) - 30;

    int y = 2;

    for (int i = 0 ; i < 15 ; i++){

        gotoxy(startX - 7, y++);
        cout << logo[i];
    }

    gotoxy(startX + 17, y + 2);
    cout << ">> START GAME";

    gotoxy(startX + 20, y + 4);
    cout << "HIGH SCORE : " << highScore;

    gotoxy(startX + 20, y + 6);
    cout << "PRESS ENTER";

    gotoxy(startX + 10, y + 10);
    cout << "╔════════ CONTROLS ═══════════╗";

    gotoxy(startX + 10, y + 11);
    cout << "║  [A] DI CHUYEN QUA TRAI     ║";

    gotoxy(startX + 10, y + 12);
    cout << "║  [D] DI CHUYEN QUA PHAI     ║";

    gotoxy(startX + 10, y + 13);
    cout << "║  [X] DI CHUYEN XUONG DUOI   ║";

    gotoxy(startX + 10, y + 14);
    cout << "║  [W] XOAY                   ║";

    gotoxy(startX + 10, y + 15);
    cout << "║  [Q] THOAT GAME             ║";

    gotoxy(startX + 10, y + 16);
    cout << "╚═════════════════════════════╝";
}

void centerGame(){

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE),
        &csbi
    );

    int consoleWidth =
        csbi.srWindow.Right - csbi.srWindow.Left + 1;

    int consoleHeight =
        csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int totalWidth = GAME_W + 30;

    OFFSET_X = (consoleWidth - totalWidth) / 2;

    OFFSET_Y = (consoleHeight - H) / 2;
}

int main(){

    system("chcp 65001 > nul");

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    hideCursor();

    system("mode con cols=140 lines=40");
    system("cls");
    centerGame();
    system("color 0A");

    srand(time(0));

    drawMenu();

    while (getch() != 13);

    system("cls");

    b = rand() % 8;
    nextBlock = rand() % 8;
    DWORD lastFall = GetTickCount();

    initBoard();
////
DWORD lastInput = GetTickCount();
DWORD inputDelay = 50;
////
    while (1){

        boardDelBlock();

        if (GetTickCount() - lastInput >= inputDelay) {
        if (GetAsyncKeyState('A') & 0x8000) {
            if (canMove(-1, 0)) x--;
            lastInput = GetTickCount();
        }
        if (GetAsyncKeyState('D') & 0x8000) {
            if (canMove(1, 0)) x++;
            lastInput = GetTickCount();
        }
        if (GetAsyncKeyState('X') & 0x8000) {
            if (canMove(0, 1)) y++;
            lastInput = GetTickCount();
        }
        if (GetAsyncKeyState('W') & 0x8000) {
            rotateBlock();
            lastInput = GetTickCount();
            Sleep(150); 
        }
        if (GetAsyncKeyState('Q') & 0x8000) break;
    }

        DWORD speed = (DWORD)max(50, 200 - level * 10);
        if (GetTickCount() - lastFall >= speed) {
            if (canMove(0, 1)) y++;
            else {
                block2Board();
                removeLine();
                x = (GAME_W - 2) / 2;
                y = 0;
                b = nextBlock;
                nextBlock = rand() % 8;
                if (!canMove(0,0)) break; 
            }
            lastFall = GetTickCount();
        }

        block2Board();
        draw();
        Sleep(1);
    }

    return 0;
}
