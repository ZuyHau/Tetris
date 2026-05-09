#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime> // Thêm thư viện để dùng time(0)

using namespace std;

#define H 20
#define W 15

char board[H][W] = {} ;
char blocks[][4][4] = {
        {{' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '},
         {' ','I',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','O','O',' '},
         {' ','O','O',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','T',' ',' '},
         {'T','T','T',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ','S','S',' '},
         {'S','S',' ',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'Z','Z',' ',' '},
         {' ','Z','Z',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {'J',' ',' ',' '},
         {'J','J','J',' '},
         {' ',' ',' ',' '}},
        {{' ',' ',' ',' '},
         {' ',' ','L',' '},
         {'L','L','L',' '},
         {' ',' ',' ',' '}}
};

int x=4, y=0, b=1;

// --- CÁC BIẾN MỚI CHO UI ---
int score = 0;
int currentLevel = 1;
int linesClearedTotal = 0;

bool isLevelUp = false;
int levelUpTimer = 0;
// ---------------------------

void gotoxy(int x, int y) {
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void boardDelBlock(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ' && y+i < H)
                board[y+i][x+j] = ' ';
}

void block2Board(){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' ' )
                board[y+i][x+j] = blocks[b][i][j];
}

void initBoard(){
    for (int i = 0 ; i < H ; i++)
        for (int j = 0 ; j < W ; j++)
            if ((i==H-1) || (j==0) || (j == W-1)) board[i][j] = '#';
            else board[i][j] = ' ';
}

// --- HÀM VẼ GIAO DIỆN UI ---
void drawUI() {
    int uiX = W + 5;

    gotoxy(uiX, 3); cout << "SCORE: " << score << "       ";
    gotoxy(uiX, 5); cout << "LEVEL: " << currentLevel << "       ";
    gotoxy(uiX, 7); cout << "LINES: " << linesClearedTotal << "       ";

    if (isLevelUp) {
        gotoxy(uiX, 10);
        if (levelUpTimer % 2 == 0) {
            cout << "!!! LEVEL UP !!!";
        } else {
            cout << "                ";
        }

        levelUpTimer--;
        if (levelUpTimer <= 0) {
            isLevelUp = false;
            gotoxy(uiX, 10);
            cout << "                ";
        }
    }
}
// ---------------------------

void draw(){
    gotoxy(0,0);
    for (int i = 0 ; i < H ; i++, cout<<endl)
        for (int j = 0 ; j < W ; j++)
            cout<<board[i][j];

    drawUI(); // Vẽ UI ngay sau khi vẽ bảng game
}

bool canMove(int dx, int dy){
    for (int i = 0 ; i < 4 ; i++)
        for (int j = 0 ; j < 4 ; j++)
            if (blocks[b][i][j] != ' '){
                int tx = x + j + dx;
                int ty = y + i + dy;
                if ( tx<1 || tx >= W-1 || ty >= H-1) return false;
                if ( board[ty][tx] != ' ') return false;
            }
    return true;
}

// Dummy rotateBlock để code biên dịch được (Bạn chèn logic xoay vào đây)
void rotateBlock() {
    // Logic xoay khối của bạn
}

// --- HÀM XÓA DÒNG ĐƯỢC REFACTOR ---
void removeLine() {
    int linesClearedNow = 0;

    // Thuật toán 2 con trỏ: Duyệt từ dưới lên
    int write_row = H - 2;

    for (int read_row = H - 2; read_row >= 0; read_row--) {
        // Kiểm tra hàng đã đầy chưa
        bool isFull = true;
        for (int j = 1; j < W - 1; j++) {
            if (board[read_row][j] == ' ') {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            // Xóa dữ liệu hàng đó (gián tiếp bằng cách bỏ qua, không ghi xuống)
            linesClearedNow++;
        } else {
            // Cập nhật tọa độ (dịch xuống)
            if (write_row != read_row) {
                for (int j = 1; j < W - 1; j++) {
                    board[write_row][j] = board[read_row][j];
                }
            }
            write_row--;
        }
    }

    // Làm sạch các hàng trống phía trên cùng
    while (write_row >= 0) {
        for (int j = 1; j < W - 1; j++) {
            board[write_row][j] = ' ';
        }
        write_row--;
    }

    // Cập nhật điểm số
    if (linesClearedNow > 0) {
        linesClearedTotal += linesClearedNow;

        int pointsEarned = 0;
        switch(linesClearedNow) {
            case 1: pointsEarned = 100; break;
            case 2: pointsEarned = 300; break;
            case 3: pointsEarned = 500; break;
            case 4: pointsEarned = 800; break;
            default: pointsEarned = linesClearedNow * 200; break;
        }
        score += pointsEarned * currentLevel;

        int newLevel = (linesClearedTotal / 5) + 1;
        if (newLevel > currentLevel) {
            currentLevel = newLevel;
            isLevelUp = true;
            levelUpTimer = 15;
        }

        // Chỉ vẽ lại màn hình 1 lần sau khi cập nhật toàn bộ mảng xong
        // Loại bỏ Sleep ở đây để game không bị khựng lại
    }
}
// ----------------------------------

int main()
{
    srand(time(0));
    b = rand() % 7;
    system("cls");
    initBoard();
    while (1){
        boardDelBlock();
        if (_kbhit()){
            char c = _getch();
            if (c=='a' && canMove(-1,0)) x--;
            if (c=='d' && canMove(1,0) ) x++;
            if (c=='x' && canMove(0,1))  y++;
            if (c=='w') rotateBlock();
            if (c=='q') break;
        }
        if (canMove(0,1)) y++;
        else {
            block2Board();
            removeLine();
            x = 5; y = 0; b = rand() % 7;
        }
        block2Board();
        draw();
        Sleep(200);
    }
    return 0;
}
