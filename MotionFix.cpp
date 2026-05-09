#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;

#define H 20
#define GAME_W 40

#define OFFSET_X 20
#define OFFSET_Y 2

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

    int j;

    for (int i = H-2 ; i > 0 ; i--){

        for (j = 2 ; j < GAME_W-2 ; j++){

            if (board[i][j] == ' ')
                break;
        }

        if (j == GAME_W-2){
            for (int blink = 0 ; blink < 3 ; blink++){

                for (int j = 2 ; j < GAME_W-2 ; j++)
                    board[i][j] = '#';

                draw();
                Sleep(100);

                for (int j = 2 ; j < GAME_W-2 ; j++)
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

                for (int j = 2 ; j < GAME_W-2 ; j++)
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

void drawInfo(){

    gotoxy(OFFSET_X + GAME_W + 2, OFFSET_Y);
    cout << "╔══════════╗";

    gotoxy(OFFSET_X + GAME_W + 2, OFFSET_Y + 1);
    cout << "║  TETRIS  ║";

    gotoxy(OFFSET_X + GAME_W + 2, OFFSET_Y + 2);
    cout << "╚══════════╝";

    gotoxy(OFFSET_X + GAME_W + 3, OFFSET_Y + 5);
    cout << "SCORE : " << score;

    gotoxy(OFFSET_X + GAME_W + 3, OFFSET_Y + 7);
    cout << "HIGH SCORE  : " << highScore;

    gotoxy(OFFSET_X + GAME_W + 3, OFFSET_Y + 9);
    cout << "LEVEL : " << level;

    drawNextBlock();
}

void draw(){

    for (int i = 0 ; i < H ; i++){

        gotoxy(OFFSET_X, OFFSET_Y + i);

        for (int j = 0 ; j < GAME_W ; j++){

            if (board[i][j] == ' ')
                cout << '.';
            else
                cout << board[i][j];
        }
    }

    drawInfo();
}

void drawMenu(){

    system("cls");
    system("color 0A");

    cout << R"(

        ,----,                 ,----,
      ,/   .`|               ,/   .`|
    ,`   .'  :   ,---,.    ,`   .'  :,-.----.     ,---,  .--.--.
  ;    ;     / ,'  .' |  ;    ;     /\    /  \ ,`--.' | /  /    '.
.'___,/    ,',---.'   |.'___,/    ,' ;   :    \|   :  :|  :  /`. /
|    :     | |   |   .'|    :     |  |   | .\ ::   |  ';  |  |--`
;    |.';  ; :   :  |-,;    |.';  ;  .   : |: ||   :  ||  :  ;_
`----'  |  | :   |  ;/|`----'  |  |  |   |  \ :'   '  ; \  \    `.
    '   :  ; |   :   .'    '   :  ;  |   : .  /|   |  |  `----.   \
    |   |  ' |   |  |-,    |   |  '  ;   | |  \'   :  ;  __ \  \  |
    '   :  | '   :  ;/|    '   :  |  |   | ;\  \   |  ' /  /`--'  /
    ;   |.'  |   |    \    ;   |.'   :   ' | \.'   :  |'--'.     /
    '---'    |   :   .'    '---'     :   : :-' ;   |.'   `--'---'
             |   | ,'                |   |.'   '---'
             `----'                  `---'

)";

    cout << "\n";


    cout << "                 >> START GAME\n";
    cout << "                    HIGH SCORE : " << highScore << "\n";
    cout << "                    PRESS ENTER\n\n";

    cout << "          ╔════════ CONTROLS ═══════════╗\n";
    cout << "          ║  [A] DI CHUYEN QUA TRAI     ║\n";
    cout << "          ║  [D] DI CHUYEN QUA PHAI     ║\n";
    cout << "          ║  [X] DI CHUYEN XUONG DUOI   ║\n";
    cout << "          ║  [W] XOAY                   ║\n";
    cout << "          ║  [Q] THOAT GAME             ║\n";
    cout << "          ╚═════════════════════════════╝\n";
}

int main(){
    system("chcp 65001 > nul");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    hideCursor();
    srand(time(0));

    drawMenu();
    while (getch() != 13);
    system("cls");

    initBoard();
    b = rand() % 8;
    nextBlock = rand() % 8;
    DWORD lastFall = GetTickCount();

    while (1){
        boardDelBlock();
        if (kbhit()){
            char c = getch();
            if (c == 'a' && canMove(-1,0)) x--;
            if (c == 'd' && canMove(1,0)) x++;
            if (c == 'x' && canMove(0,1)) y++;
            if (c == 'w') rotateBlock(); 
            if (c == 'q') break;
        }

        int speed = max(50, 200 - level * 10);
        if (GetTickCount() - lastFall >= speed) {
            if (canMove(0, 1)) y++;
            else {
                block2Board();
                removeLine();
                x = (GAME_W - 2) / 2;
                y = 0;
                b = nextBlock;
                nextBlock = rand() % 8;
                if (!canMove(0, 0))break;
            }
            lastFall = GetTickCount();
        }

        block2Board();
        draw();
        Sleep(1);
    }
    return 0;
}
