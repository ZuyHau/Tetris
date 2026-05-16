#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;

#define H 28
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

class Piece {
public:
    int x;
    int y;
    int color;
    char shape[4][4];

    Piece(int startX, int startY, int c, char s[4][4]) {
        x = startX;
        y = startY;
        color = c;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                shape[i][j] = s[i][j];
            }
        }
    }

    virtual void rotate() = 0;

    virtual ~Piece() {}
};

class IPiece : public Piece {
public:
    IPiece(int startX, int startY, int c, char s[4][4])
        : Piece(startX, startY, c, s) {}

    void rotate() override {
        char temp[4][4];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                temp[j][3 - i] = shape[i][j];
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                shape[i][j] = temp[i][j];
            }
        }
    }
};

class OPiece : public Piece {
public:
    OPiece(int startX, int startY, int c, char s[4][4])
        : Piece(startX, startY, c, s) {}

    void rotate() override {}
};

class TPiece : public Piece {
public:
    TPiece(int startX, int startY, int c, char s[4][4])
        : Piece(startX, startY, c, s) {
    }

    void rotate() override {
        char temp[4][4];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                temp[j][3 - i] = shape[i][j];
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                shape[i][j] = temp[i][j];
            }
        }
    }
};


class SPiece : public Piece {
public:
    SPiece(int startX, int startY, int c, char s[4][4])
        : Piece(startX, startY, c, s) {
    }

    void rotate() override {
        char temp[4][4];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                temp[j][3 - i] = shape[i][j];
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                shape[i][j] = temp[i][j];
            }
        }
    }
};


class ZPiece : public Piece {
public:
    ZPiece(int startX, int startY, int c, char s[4][4])
        : Piece(startX, startY, c, s) {
    }

    void rotate() override {
        char temp[4][4];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                temp[j][3 - i] = shape[i][j];
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                shape[i][j] = temp[i][j];
            }
        }
    }
};


class JPiece : public Piece {
public:
    JPiece(int startX, int startY, int c, char s[4][4])
        : Piece(startX, startY, c, s) {
    }

    void rotate() override {
        char temp[4][4];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                temp[j][3 - i] = shape[i][j];
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                shape[i][j] = temp[i][j];
            }
        }
    }
};

class LPiece : public Piece {
public:
    LPiece(int startX, int startY, int c, char s[4][4])
        : Piece(startX, startY, c, s) {
    }

    void rotate() override {
        char temp[4][4];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                temp[j][3 - i] = shape[i][j];
            }
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                shape[i][j] = temp[i][j];
            }
        }
    }
};


Piece* currentPiece = nullptr;
Piece* nextPiece = nullptr;

int score = 0;
int highScore = 0;
int level = 1;
int linesCleared = 0;

Piece* createPiece() {
    int type = rand() % 7;
    int startX = (GAME_W - 2) / 2;
    int startY = 0;

    switch (type) {
    case 0:
        return new IPiece(startX, startY, type, blocks[type]);
    case 1:
        return new OPiece(startX, startY, type, blocks[type]);
    case 2:
        return new TPiece(startX, startY, type, blocks[type]);
    case 3:
        return new SPiece(startX, startY, type, blocks[type]);
    case 4:
        return new ZPiece(startX, startY, type, blocks[type]);
    case 5:
        return new JPiece(startX, startY, type, blocks[type]);
    case 6:
        return new LPiece(startX, startY, type, blocks[type]);
    default:
        return new IPiece(startX, startY, type, blocks[type]);
    }
}

void gotoxy(int x, int y){
    COORD c = {SHORT(x), SHORT(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void boardDelBlock(){
    for (int i = 0 ; i < 4 ; i++) {
        for (int j = 0 ; j < 4 ; j++) {
            if (currentPiece->shape[i][j] != ' ') {
                board[currentPiece->y + i][currentPiece->x + j] = ' ';
            }
        }
    }
}

void block2Board(){
    for (int i = 0 ; i < 4 ; i++) {
        for (int j = 0 ; j < 4 ; j++) {
            if (currentPiece->shape[i][j] != ' ') {
                board[currentPiece->y + i][currentPiece->x + j] = currentPiece->shape[i][j];
            }
        }
    }
}

void initBoard(){

    for (int i = 0 ; i < H ; i++){

        for (int j = 0 ; j < GAME_W ; j++){

            board[i][j] = ' ';
        }
    }
}

bool canMove(Piece* piece, int dx, int dy) {
    for (int i = 0 ; i < 4 ; i++){
        for (int j = 0 ; j < 4 ; j++){
            if (piece->shape[i][j] != ' '){
                int tx = piece->x + j + dx;
                int ty = piece->y + i + dy;

                if (tx < 1 || tx >= GAME_W-1 || ty >= H-1)
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

    char oldShape[4][4];
    int oldX = currentPiece->x;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            oldShape[i][j] = currentPiece->shape[i][j];
        }
    }

    currentPiece->rotate();

    int kick[] = { 0, 1, -1, 2, -2 };
    bool canRotate = false;

    for (int k = 0; k < 5 && !canRotate; k++) {
        bool ok = true;
        int testX = oldX + kick[k];

        for (int i = 0; i < 4 && ok; i++) {

            for (int j = 0; j < 4; j++) {

                if (currentPiece->shape[i][j] != ' ') {

                    int tx = testX + j;
                    int ty = currentPiece->y + i;

                    if (tx < 1 ||
                        tx >= GAME_W - 1 ||
                        ty >= H - 1 ||
                        board[ty][tx] != ' ') {

                        ok = false;
                        break;
                    }
                }
            }
        }
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

void draw(Piece* piece);

void removeLine(){
    int linesThisTurn = 0;
    bool fullRows[H] = {false};

    for (int i = H-2 ; i > 0 ; i--){
        int j;
        for (j = 1 ; j < GAME_W-1 ; j++){
            if (board[i][j] == ' ') break;
        }
        if (j == GAME_W-1){
            fullRows[i] = true;
            linesThisTurn++;
        }
    }

    if (linesThisTurn == 0) return;

    for (int blink = 0 ; blink < 3 ; blink++){
        for (int i = 1; i < H-1; i++) {
            if (fullRows[i]) {
                for (int j = 1 ; j < GAME_W-1 ; j++) board[i][j] = '#';
            }
        }

        draw(currentPiece);
        Sleep(100);

        for (int i = 1; i < H-1; i++) {
            if (fullRows[i]) {
                for (int j = 1; j < GAME_W - 1; j++) board[i][j] = ' ';
            }
        }

        draw(currentPiece);
        Sleep(100);
    }

    int writeRow = H - 2;

    for (int readRow = H - 2; readRow > 0; readRow--) {
        if (!fullRows[readRow]) {
            for (int j = 1; j < GAME_W - 1; j++) {
                board[writeRow][j] = board[readRow][j];
            }
            writeRow--;
        }
    }

    for (; writeRow > 0; writeRow--) {
        for (int j = 1; j < GAME_W - 1; j++) {
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

    int infoX = OFFSET_X + GAME_W + 6;
    int infoY = OFFSET_Y + 16;

    gotoxy(infoX, infoY);
    cout << "NEXT BLOCK";

    gotoxy(infoX, infoY + 1);
    cout << "╔════════╗";

    for (int i = 0 ; i < 4 ; i++){

        gotoxy(infoX, infoY + 2 + i);
        cout << "║        ║";
    }

    gotoxy(infoX, infoY + 6);
    cout << "╚════════╝";

    int minX = 4;
    int maxX = 0;
    int minY = 4;
    int maxY = 0;

    for (int i = 0 ; i < 4 ; i++){

        for (int j = 0 ; j < 4 ; j++){

            if (nextPiece->shape[i][j] != ' '){

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
    int startY = infoY + 2 + (4 - blockHeight) / 2;

    for (int i = minY ; i <= maxY ; i++){

        gotoxy(startX, startY + (i - minY));

        for (int j = minX ; j <= maxX ; j++){

            char c = nextPiece->shape[i][j];

            if (c != ' ')
                cout << c;
            else
                cout << ' ';
        }
    }
}

void drawInfo(){

    int infoX = OFFSET_X + GAME_W + 6;
    int infoY = OFFSET_Y;

    string title[] = {

"████████╗███████╗████████╗██████╗ ██╗███████╗",
"╚══██╔══╝██╔════╝╚══██╔══╝██╔══██╗██║██╔════╝",
"   ██║   █████╗     ██║   ██████╔╝██║███████╗",
"   ██║   ██╔══╝     ██║   ██╔══██╗██║╚════██║",
"   ██║   ███████╗   ██║   ██║  ██║██║███████║",
"   ╚═╝   ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚══════╝"
    };

    for (int i = 0 ; i < 6 ; i++){

        gotoxy(infoX, infoY + i);

        cout << title[i];
    }

    gotoxy(infoX, infoY + 8);
    cout << "SCORE : " << score;

    gotoxy(infoX, infoY + 10);
    cout << "LEVEL : " << level;

    gotoxy(infoX, infoY + 12);
    cout << "HIGH SCORE : " << highScore;

    gotoxy(infoX, infoY + 14);
    cout << "LINES : " << linesCleared;

    drawNextBlock();

    gotoxy(infoX, infoY + 24);
    cout << "[A][D] MOVE";

    gotoxy(infoX, infoY + 25);
    cout << "[X] FAST DROP";

    gotoxy(infoX, infoY + 26);
    cout << "[W] ROTATE";

    gotoxy(infoX, infoY + 27);
    cout << "[Q] RETURN TO MENU";
}

int getDropPreviewY(Piece* piece){

    int previewY = piece->y;

    while (1){

        bool ok = true;

        for (int i = 0 ; i < 4 ; i++){

            for (int j = 0 ; j < 4 ; j++){

                if (piece->shape[i][j] != ' '){

                    int tx = piece->x + j;
                    int ty = previewY + i + 1;

                    if (tx < 1 ||
                        tx >= GAME_W - 1 ||
                        ty >= H - 1){

                        ok = false;
                    }

                    else if (board[ty][tx] != ' '){

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

void draw(Piece* piece){

    boardDelBlock();

    int previewY = getDropPreviewY(piece);

    for (int i = 0 ; i < H ; i++){

        gotoxy(OFFSET_X, OFFSET_Y + i);

        for (int j = 0 ; j < GAME_W ; j++){

            // border
            if (i == 0 && j == 0){

                cout << "╔";
            }

            else if (i == 0 && j == GAME_W - 1){

                cout << "╗";
            }

            else if (i == H - 1 && j == 0){

                cout << "╚";
            }

            else if (i == H - 1 && j == GAME_W - 1){

                cout << "╝";
            }

            else if (i == 0 || i == H - 1){

                cout << "═";
            }

            else if (j == 0 || j == GAME_W - 1){

                cout << "║";
            }

            else{

                char cell = board[i][j];

                bool drawn = false;

                // current block
                for (int bi = 0 ; bi < 4 ; bi++){

                    for (int bj = 0 ; bj < 4 ; bj++){

                        if (piece->shape[bi][bj] != ' '){

                            if (i == piece->y + bi &&
                                j == piece->x + bj){

                                cout << piece->shape[bi][bj];

                                drawn = true;
                            }
                        }
                    }
                }

                // preview block
                if (!drawn){

                    for (int bi = 0 ; bi < 4 ; bi++){

                        for (int bj = 0 ; bj < 4 ; bj++){

                            if (piece->shape[bi][bj] != ' '){

                                if (i == previewY + bi &&
                                    j == piece->x + bj){

                                    SetConsoleTextAttribute(
                                        GetStdHandle(STD_OUTPUT_HANDLE),
                                        8
                                    );

                                    cout << piece->shape[bi][bj];

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

                    if (cell == ' '){

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

                    else{

                        cout << cell;
                    }
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

void gameOverScreen(){

    system("cls");

    string over[] = {

"   █████████    █████████   ██████   ██████ ██████████       ███████    █████   █████ ██████████ ███████████  ",
"  ███▒▒▒▒▒███  ███▒▒▒▒▒███ ▒▒██████ ██████ ▒▒███▒▒▒▒▒█     ███▒▒▒▒▒███ ▒▒███   ▒▒███ ▒▒███▒▒▒▒▒█▒▒███▒▒▒▒▒███ ",
" ███     ▒▒▒  ▒███    ▒███  ▒███▒█████▒███  ▒███  █ ▒     ███     ▒▒███ ▒███    ▒███  ▒███  █ ▒  ▒███    ▒███ ",
"▒███          ▒███████████  ▒███▒▒███ ▒███  ▒██████      ▒███      ▒███ ▒███    ▒███  ▒██████    ▒██████████  ",
"▒███    █████ ▒███▒▒▒▒▒███  ▒███ ▒▒▒  ▒███  ▒███▒▒█      ▒███      ▒███ ▒▒███   ███   ▒███▒▒█    ▒███▒▒▒▒▒███ ",
"▒▒███  ▒▒███  ▒███    ▒███  ▒███      ▒███  ▒███ ▒   █   ▒▒███     ███   ▒▒▒█████▒    ▒███ ▒   █ ▒███    ▒███ ",
" ▒▒█████████  █████   █████ █████     █████ ██████████    ▒▒▒███████▒      ▒▒███      ██████████ █████   █████",
"  ▒▒▒▒▒▒▒▒▒  ▒▒▒▒▒   ▒▒▒▒▒ ▒▒▒▒▒     ▒▒▒▒▒ ▒▒▒▒▒▒▒▒▒▒       ▒▒▒▒▒▒▒         ▒▒▒      ▒▒▒▒▒▒▒▒▒▒ ▒▒▒▒▒   ▒▒▒▒▒ "
    };

    int startX = 10;
    int startY = 6;

    for (int i = 0 ; i < 8; i++){

        gotoxy(startX, startY + i);

        cout << over[i];
    }

    gotoxy(startX + 40, startY + 12);
    cout << "HIGH SCORE : " << highScore;

    gotoxy(startX + 40, startY + 14);
    cout << "YOUR SCORE : " << score;

    gotoxy(startX + 34, startY + 18);
    cout << "PRESS Q TO RETURN TO MENU";

    while (1){

        if (_kbhit()){

            char c = _getch();

            if (c == 'q' || c == 'Q')
                break;
        }

        Sleep(1);
    }
}

int main(){

    system("chcp 65001 > nul");

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    hideCursor();

    system("mode con cols=140 lines=40");

    srand(time(0));

    while (1){

        system("cls");

        centerGame();

        system("color 0A");

        drawMenu();

        while (1){

            char c = getch();

            if (c == 13)
                break;

            if (c == 'q' || c == 'Q')
                return 0;
        }

        system("cls");

        initBoard();

        score = 0;
        level = 1;
        linesCleared = 0;

        if (currentPiece != nullptr){

            delete currentPiece;
            currentPiece = nullptr;
        }

        if (nextPiece != nullptr){

            delete nextPiece;
            nextPiece = nullptr;
        }

        currentPiece = createPiece();
        nextPiece = createPiece();

        DWORD lastFall = GetTickCount();

        DWORD lastInput = GetTickCount();
        DWORD inputDelay = 50;

        bool quitToMenu = false;

        while (1){

            boardDelBlock();

            if (GetTickCount() - lastInput >= inputDelay){

                if (GetAsyncKeyState('A') & 0x8000){

                    if (canMove(currentPiece, -1, 0))
                        currentPiece->x--;

                    lastInput = GetTickCount();
                }

                if (GetAsyncKeyState('D') & 0x8000){

                    if (canMove(currentPiece, 1, 0))
                        currentPiece->x++;

                    lastInput = GetTickCount();
                }

                if (GetAsyncKeyState('X') & 0x8000){

                    if (canMove(currentPiece, 0, 1))
                        currentPiece->y++;

                    lastInput = GetTickCount();
                }

                if (GetAsyncKeyState('W') & 0x8000){

                    rotateBlock();

                    lastInput = GetTickCount();

                    Sleep(150);
                }

                if (GetAsyncKeyState('Q') & 0x8000){

                    quitToMenu = true;

                    while (GetAsyncKeyState('Q') & 0x8000);

                    break;
                }
            }

            DWORD speed =
                (DWORD)max(50, 200 - level * 10);

            if (GetTickCount() - lastFall >= speed){

                if (canMove(currentPiece, 0, 1)){

                    currentPiece->y++;
                }

                else{

                    block2Board();

                    removeLine();

                    delete currentPiece;

                    currentPiece = nextPiece;

                    nextPiece = createPiece();

                    if (!canMove(currentPiece, 0, 0)){

                        gameOverScreen();

                        break;
                    }
                }

                lastFall = GetTickCount();
            }

            block2Board();

            draw(currentPiece);

            Sleep(1);
        }

        if (quitToMenu){

            delete currentPiece;
            delete nextPiece;

            currentPiece = nullptr;
            nextPiece = nullptr;

            FlushConsoleInputBuffer(
                GetStdHandle(STD_INPUT_HANDLE)
            );

            continue;
        }
    }

    return 0;
}
