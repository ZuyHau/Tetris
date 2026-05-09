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

int x=4,y=0,b=1;

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

// --- HÀM MỚI ĐỂ VẼ UI ---
void drawUI() {
    int uiX = W + 5; // Toạ độ X nằm bên phải của bảng game

    // [Task 1 & 2] Vẽ nhãn và cập nhật số liệu
    // In thêm khoảng trắng phía sau để ghi đè/xóa các số cũ dài hơn nếu có
    gotoxy(uiX, 3); cout << "SCORE: " << score << "       ";
    gotoxy(uiX, 5); cout << "LEVEL: " << currentLevel << "       ";
    gotoxy(uiX, 7); cout << "LINES: " << linesClearedTotal << "       ";

    // [Task 3] Hiển thị thông báo Level Up với hiệu ứng nhấp nháy
    if (isLevelUp) {
        gotoxy(uiX, 10);
        if (levelUpTimer % 2 == 0) { // Nhấp nháy theo frame chẵn/lẻ
            cout << "!!! LEVEL UP !!!";
        } else {
            cout << "                "; // Xóa text để tạo cảm giác nhấp nháy
        }
        
        levelUpTimer--;
        if (levelUpTimer <= 0) {
            isLevelUp = false; // Tắt cờ khi hết thời gian
            gotoxy(uiX, 10);
            cout << "                "; // Xóa hẳn dòng chữ để làm sạch màn hình
        }
    }
}
// ------------------------

void draw(){
    gotoxy(0,0);
    for (int i = 0 ; i < H ; i++, cout<<endl)
        for (int j = 0 ; j < W ; j++)
            cout<<board[i][j];
            
    drawUI(); // Gọi hàm vẽ UI ngay sau khi vẽ bảng game
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

// Dummy rotateBlock để code có thể chạy được, bạn hãy thay bằng logic xoay của mình
void rotateBlock() {
    // Thêm code xoay ma trận blocks[b] ở đây
}

void removeLine(){
    int j;
    int linesClearedNow = 0; // Biến đếm số dòng ăn được trong 1 lần

    for (int i = H-2; i >0 ; i-- ){
        for (j = 0; j < W-1 ; j++)
            if (board[i][j] == ' ') break;
        if (j == W-1){
            linesClearedNow++; // Tăng biến đếm dòng

            for (int ii = i; ii >0 ; ii-- )
                for (int j = 0; j < W-1 ; j++ ) board[ii][j] = board[ii-1][j];
            i++;
            draw();
            Sleep(200);
        }
    }

    // --- LOGIC CẬP NHẬT ĐIỂM SỐ & LEVEL MỚI ---
    if (linesClearedNow > 0) {
        linesClearedTotal += linesClearedNow;
        
        // Tính điểm: Ăn nhiều dòng cùng lúc điểm càng cao
        score += linesClearedNow * 100 * currentLevel; 

        // Logic tính level: Mỗi 5 dòng lên 1 cấp
        int newLevel = (linesClearedTotal / 5) + 1;
        
        if (newLevel > currentLevel) {
            currentLevel = newLevel;
            isLevelUp = true;    // Bật cờ hiệu ứng
            levelUpTimer = 15;   // Hiển thị trong 15 frames (khoảng 3 giây do có Sleep)
        }
    }
    // ------------------------------------------
}

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
            if (c=='s' && canMove(0,1))  y++; // Đổi 'x' thành 's' cho chuẩn WASD nếu bạn muốn
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