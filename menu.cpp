#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

#define H 30
#define GAME_W 60

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

                if (tx < 1 || tx >= GAME_W-1 || ty >= H-1)
                    return false;

                if (board[ty][tx] != ' ')
                    return false;
            }
        }
    }

    return true;
}

void draw();

void removeLine(){

    int j;

    for (int i = H-2 ; i > 0 ; i--){

        for (j = 1 ; j < GAME_W-1 ; j++){

            if (board[i][j] == ' ')
                break;
        }

        if (j == GAME_W-1){

            for (int blink = 0 ; blink < 3 ; blink++){

                for (int j = 1 ; j < GAME_W-1 ; j++)
                    board[i][j] = '#';

                draw();
                Sleep(100);

                for (int j = 1 ; j < GAME_W-1 ; j++)
                    board[i][j] = ' ';

                draw();
                Sleep(100);
            }

            score += 100;

            if (score > highScore)
                highScore = score;

            if (score % 500 == 0)
                level++;

            for (int ii = i ; ii > 0 ; ii--){

                for (int j = 1 ; j < GAME_W-1 ; j++)
                    board[ii][j] = board[ii-1][j];
            }

            i++;
        }
    }
}

void drawNextBlock(){

    int startX = OFFSET_X + GAME_W + 4;
    int startY = OFFSET_Y + 14;

    for (int i = 0 ; i < 6 ; i++){

        gotoxy(startX, startY - 2 + i);
        cout << "            ";
    }

    gotoxy(startX, startY - 2);
    cout << "NEXT BLOCK";

    for (int i = 0 ; i < 4 ; i++){

        gotoxy(startX, startY + i);

        for (int j = 0 ; j < 4 ; j++){
            cout << blocks[nextBlock][i][j];
        }
    }
}

int getDropPreviewY(){

    int previewY = y;

    while (1){

        bool ok = true;

        for (int i = 0 ; i < 4 ; i++){

            for (int j = 0 ; j < 4 ; j++){

                if (blocks[b][i][j] != ' '){

                    int tx = x + j;
                    int ty = previewY + i + 1;

                    if (tx < 1 ||
                        tx >= GAME_W-1 ||
                        ty >= H-1 ||
                        board[ty][tx] != ' '){

                        ok = false;
                    }
                }
            }
        }

        if (!ok)
            break;

        previewY++;
    }

    return previewY;
}

void drawInfo(){

    int infoX = OFFSET_X + GAME_W + 5;

    string title[] = {

"████████╗███████╗████████╗██████╗ ██╗███████╗",
"╚══██╔══╝██╔════╝╚══██╔══╝██╔══██╗██║██╔════╝",
"   ██║   █████╗     ██║   ██████╔╝██║███████╗",
"   ██║   ██╔══╝     ██║   ██╔══██╗██║╚════██║",
"   ██║   ███████╗   ██║   ██║  ██║██║███████║",
"   ╚═╝   ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚══════╝"
    };

    for (int i = 0 ; i < 6 ; i++){

        gotoxy(infoX, OFFSET_Y + i);

        cout << title[i];
    }

    gotoxy(infoX, OFFSET_Y + 8);
    cout << "SCORE : " << score;

    gotoxy(infoX, OFFSET_Y + 10);
    cout << "LEVEL : " << level;

    gotoxy(infoX, OFFSET_Y + 12);
    cout << "HIGH SCORE : " << highScore;

    gotoxy(infoX, OFFSET_Y + 16);
    cout << "NEXT";

    gotoxy(infoX, OFFSET_Y + 17);
    cout << "╔════════╗";

    for (int i = 0 ; i < 4 ; i++){

        gotoxy(infoX, OFFSET_Y + 18 + i);
        cout << "║        ║";
    }

    gotoxy(infoX, OFFSET_Y + 22);
    cout << "╚════════╝";

    for (int i = 0 ; i < 4 ; i++){

        gotoxy(infoX + 1, OFFSET_Y + 18 + i);
        cout << "        ";
    }

    int minX = 4;
    int maxX = 0;
    int minY = 4;
    int maxY = 0;

    for (int i = 0 ; i < 4 ; i++){

        for (int j = 0 ; j < 4 ; j++){

            if (blocks[nextBlock][i][j] != ' '){

                minX = min(minX, j);
                maxX = max(maxX, j);

                minY = min(minY, i);
                maxY = max(maxY, i);
            }
        }
    }

    int blockWidth = maxX - minX + 1;
    int blockHeight = maxY - minY + 1;

    int startX = infoX + 1 + (8 - blockWidth) / 2;
    int startY = OFFSET_Y + 18 + (4 - blockHeight) / 2;

    for (int i = minY ; i <= maxY ; i++){

        gotoxy(startX, startY + (i - minY));

        for (int j = minX ; j <= maxX ; j++){

            char c = blocks[nextBlock][i][j];

            if (c != ' ')
                cout << c;
            else
                cout << ' ';
        }
    }

    gotoxy(infoX, OFFSET_Y + 24);
    cout << "[A][D] DI CHUYEN";

    gotoxy(infoX, OFFSET_Y + 25);
    cout << "[X] ROI NHANH";

    gotoxy(infoX, OFFSET_Y + 26);
    cout << "[Q] THOAT";
}

void draw(){
    boardDelBlock();
    int previewY = getDropPreviewY();

    for (int i = 0 ; i < H ; i++){

        gotoxy(OFFSET_X, OFFSET_Y + i);

        for (int j = 0 ; j < GAME_W ; j++){

            if (i == 0 && j == 0)
                cout << "╔";

            else if (i == 0 && j == GAME_W-1)
                cout << "╗";

            else if (i == H-1 && j == 0)
                cout << "╚";

            else if (i == H-1 && j == GAME_W-1)
                cout << "╝";

            else if (i == 0 || i == H-1)
                cout << "═";

            else if (j == 0 || j == GAME_W-1)
                cout << "║";

            else{

                bool drawn = false;

                // current block
                for (int bi = 0 ; bi < 4 ; bi++){

                    for (int bj = 0 ; bj < 4 ; bj++){

                        if (blocks[b][bi][bj] != ' '){

                            if (i == y + bi &&
                                j == x + bj){

                                cout << blocks[b][bi][bj];

                                drawn = true;
                            }
                        }
                    }
                }

                // preview block
                if (!drawn){

                    for (int bi = 0 ; bi < 4 ; bi++){

                        for (int bj = 0 ; bj < 4 ; bj++){

                            if (blocks[b][bi][bj] != ' '){

                                if (i == previewY + bi &&
                                    j == x + bj){

                                    SetConsoleTextAttribute(
                                        GetStdHandle(STD_OUTPUT_HANDLE),
                                        8
                                    );

                                    cout << blocks[b][bi][bj];

                                    SetConsoleTextAttribute(
                                        GetStdHandle(STD_OUTPUT_HANDLE),
                                        10
                                    );

                                    drawn = true;
                                }
                            }
                        }
                    }
                }

                // empty
                if (!drawn){

                    if (board[i][j] == ' '){

                        SetConsoleTextAttribute(
                            GetStdHandle(STD_OUTPUT_HANDLE),
                            8
                        );

                        cout << '.';

                        SetConsoleTextAttribute(
                            GetStdHandle(STD_OUTPUT_HANDLE),
                            10
                        );
                    }

                    else
                        cout << board[i][j];
                }
            }
        }
    }
    block2Board();
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
    cout << "║  [Q] THOAT GAME             ║";

    gotoxy(startX + 10, y + 15);
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

    OFFSET_X = (consoleWidth - GAME_W - 45) / 2;

    OFFSET_Y = (consoleHeight - H) / 2;
}

int main(){

    system("chcp 65001 > nul");

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

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

    initBoard();

    int tick = 0;

    while (1){

        boardDelBlock();

        if (GetAsyncKeyState('A') & 0x8000){

            if (canMove(-1,0))
                x--;
        }

        if (GetAsyncKeyState('D') & 0x8000){

            if (canMove(1,0))
                x++;
        }

        if (GetAsyncKeyState('X') & 0x8000){

            if (canMove(0,1))
                y++;
        }

        if (GetAsyncKeyState('Q') & 0x8000){

            break;
        }

        tick++;

        if (tick >= max(1, 4 - level / 2)){

            if (canMove(0,1))
                y++;

            else{

                block2Board();

                removeLine();

                x = (GAME_W - 2) / 2;
                y = 0;

                b = nextBlock;
                nextBlock = rand() % 8;
            }

            tick = 0;
        }

        block2Board();

        draw();

        Sleep(16);
    }

    return 0;
}
