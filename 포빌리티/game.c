#pragma warning(disable:4996)
#pragma warning(disable:28159)
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <math.h>
#include "Map1.h"
#include "cursor.h"


typedef struct player
{
    COORD pos;
    int angle;
    int box;
} Player;

typedef struct portal
{
    COORD pos;
    int mode;
} Portal;

typedef struct object {
    COORD pos;
    int active;
} Object;

typedef struct monstermaplever {
    COORD pos[10];
    int leverinfo[10];
}Levermonstermap;

typedef struct Monster
{
    COORD pos[10];
    int gun[10];
    COORD gunpos[10];
    int h[10];
} Monster;

Monster monster;
char monsterModel[1] = { 1 };
int visited[11];

Levermonstermap monstermaplever;

Object lever;
Object plate;

Player player;
Portal in;
Portal out;
Portal Mirror;
clock_t start, end;

COORD start_pos[] = {
    { 0,  0},   // MAP 0:
    { 5, 40},   // MAP 1: 튜토리얼
    { 5, 40},   // MAP 2: 상자/레버/거울
    { 5, 36},   // MAP 3: 세갈래길
    { 5, 40},   // MAP 4: 몬스터
    { 5, 40},   // MAP 5: 비밀번호
    {20, 40},   // MAP 6: 미니게임
    { 5, 40},   // MAP 7: 시야 축소
    {28, 40},   // MAP 8: 발전소
    {12, 40},   // MAP 9: 침수
    { 5, 40},   // MAP 10: Game Over
    { 5, 40},   // MAP 11: Ending Credit
};

COORD end_pos[] = {
    { 0,  0},   // MAP 0:
    {72,  8},   // MAP 1: 튜토리얼
    {36,  8},   // MAP 2: 상자/레버/거울
    {28,  3},   // MAP 3: 세갈래길
    {76,  3},   // MAP 4: 몬스터
    { 5, 40},   // MAP 5: 비밀번호
    {75,  7},   // MAP 6: 미니게임
    { 5, 19},   // MAP 7: 시야 축소
    {77, 16},   // MAP 8: 발전소
    {74,  6},   // MAP 9: 침수
    { 5, 40},   // MAP 10: Game Over
    { 5, 40},   // MAP 11: Ending Credit
};

int item = 0;

int map_id = 0;
int inkflag = 0;
int locate1 = 0, locate2 = 0;
int bulletCnt = 20;
int gunOption = 1;
int angle = 0b0001;
char playerModel[3] = { 2, 1, 3 };
int Item_sightExpansion = 1;
int Item_savePoint = 1;
int Item_randomTeleport = 1;
int player_height = 3;

int cnt_teleport = 0;
int cnt_shoot = 0;

void ShowMonster();
void ShowMonster2();
void ShowMonster3();
void ShowMonster4();
void MonsterFireGun();
void MonsterFireGun2();
void MonsterFireGun3();
void MonsterFireGun4();
void totalmonster();

int t[10], k[10], cnt[10];
int s[10];

double time_check;
clock_t block_st, block_ed;

char tmp[11][GBOARD_HEIGHT + 2][GBOARD_WIDTH + 2];
int mapToMap[12][12];

// 초기화 및 출력
void setPlayer_s();
void setPlayer_e();
void setPortal(Portal* p);
void setMap();
void setObject(Object* o, int x, int y);
void DrawingGameboard();
void ShowPlayer();
void DeletePlayer();
void setPortalSetting();

// 플레이어 이동
void keyInput();
int detectCollision_C(int posX, int posY, char arr[1], int mode);
void Down();
void Jump();
void moveToPortal(int mode);
void checkNextMap(int idx);

// 포탈 건
void ShootPortal_W();
int DetectCollision(int posX, int posY);
void reflectMirror();
void shootingangle();

// UI
void nextStage();
void printUI();
void DrawingMiniMap_S();
void extendMiniMap();
void DrawingBullet();
void DeleteBullet();
void DrawingItem();
void DrawingGunState();
void DrawingDirection();
void deleteDirection();
void DrawUiFrame();

// 상자, 레버
void interaction();
void switchPlate_map_2();
void switchLever_map_2();
void removeBlock();


//침수맵
void drawTheSurfaceOfWater(int surfaceH);
int surFaceHeight = GBOARD_HEIGHT + 2;
int flag = 0;
int floodingState = 0;
clock_t start_flooding, end_flooding;
void Map_9();
void switchLever();

//블록 on/off
void turnOnOffBlock();
#define start_X 26
#define end_X   68
int flag3 = 0;
int flag4 = 0;
double time_check = 0;

// 미니게임
void printQuestion();
int checkAnswer(int posX, int posY, char arr[1], int mode);
int questionNum = 0;
int score = 0;
char question[8][40] = { {"5초를 주겠다"}, {"4개 맞추면 손전등을 주지"}, {"Q. 1 + 1 = ?"}, {"Q. 세종대왕은 조선의 ?대 왕이다."}, {"Q. 0x1C - 0x14 = ?"}, {"Q. 7 > 4 ? 3 : 5"}, {"Q. 1 + 5 = ?"}, {"The END!!"} };
int answer[7] = { 0, 0, 2, 4, 8, 3, 6 };
int enterTime = 0;

//몬스터
int temp;
unsigned long Timer;
unsigned long Timer2;
unsigned long Timer3;
unsigned long Timer4;
unsigned long Timer5;
void map4Enter();
void Map4Move();
void test11();
void jumpmonster1();
void jumpmonster2();
void jumpmonster3();
void jumpmonster4();

unsigned long Test_Timer;
unsigned long Test_Timer1;
unsigned long Test_Timer2;
unsigned long Test_Timer3;
int kkkk;

int time_gameover = 0;
void showResult();
void showResult2();

int main() {

    system("mode con:cols=200 lines=48");



    // 게임 세팅
    RemoveCursor();
    setPortal(&in);
    setPortal(&out);
    setPortal(&Mirror);
    setObject(&lever, 0, 0);
    setObject(&plate, 0, 0);
    setPortalSetting();
    setMap();

    return 0;
}


void setPlayer_s() {
    player.pos.X = GBOARD_ORIGIN_X + start_pos[map_id].X * 2;
    player.pos.Y = GBOARD_ORIGIN_Y + start_pos[map_id].Y;
    player.angle = 0;
    player.box = 0;
}

void setPlayer_e() {
    player.pos.X = GBOARD_ORIGIN_X + end_pos[map_id].X * 2;
    player.pos.Y = GBOARD_ORIGIN_Y + end_pos[map_id].Y;
    player.angle = 0;
    player.box = 0;
}

void setPortal(Portal* p) {
    p->mode = 0;
    p->pos.X = -1;
    p->pos.Y = -1;

}

void setMap() {
    for (int i = 0; i < 12; i++)
    {
        for (int y = 0; y < GBOARD_HEIGHT + 2; y++)
        {
            for (int x = 0; x <= (GBOARD_WIDTH + 2); x++)
            {
                tmp[i][y][x] = map[i][y][x];
            }
        }
    }
}

void setObject(Object* o, int x, int y) {
    o->pos.X = x;
    o->pos.Y = y;
    o->active = 0;
}

void DrawingGameboard()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    if (map_id == 5 || map_id == 7) {
        for (int y = 0; y < GBOARD_HEIGHT + 2; y++) {
            for (int x = 0; x < GBOARD_WIDTH + 2; x++)
            {
                if (y == 0 || x == 0 || y == GBOARD_HEIGHT + 1 || x == GBOARD_WIDTH + 1) {

                    int block = map[map_id][y][x];
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);

                    switch (block) {
                    case -1:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;

                    case 1: case 2:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 3:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 5:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                        printf("▣");
                        break;
                    case 7:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        if (lever.active == 0)
                            printf("＼");
                        else
                            printf("　");
                        break;
                    case 8:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        if (lever.active == 1)
                            printf("／");
                        else
                            printf("　");
                        break;
                    case 9:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                        printf("▣");    // 레버 밑판
                        lever.pos.X = x;
                        lever.pos.Y = y;
                        break;
                    case 11:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 144);
                        printf("□");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 12:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                        printf("□");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 13:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
                        printf("▨");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 20:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 22:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 112:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        printf("l ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 113:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        printf("▶");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 114:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                        printf("l ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 115:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                        printf("▶");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 44:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 144);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case -3:
                        if (floodingState == 1 && surFaceHeight <= y && map_id == 9)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 144);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                    default:
                        printf("　");
                        break;
                    }
                }
                else {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    printf("  ");
                }

            }
        }
    }
    else {
        for (int y = 0; y < GBOARD_HEIGHT + 2; y++)
            for (int x = 0; x < GBOARD_WIDTH + 2; x++)
            {
                int block = map[map_id][y][x];
                if (map_id == 6 && 57 < x && 8 < y)
                    continue;

                SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);

                switch (block) {
                case -1:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 1: case 2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 3:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 4:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                    printf("▦");    // 발판에 박스 올려져 있을 때
                    break;

                case 5:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                    printf("▣");
                    break;
                case 6:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                    printf("＿");    // 발판(박스 올려 놓는 곳)
                    plate.pos.X = x;
                    plate.pos.Y = y;
                    break;

                case 7:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    if (lever.active == 0)
                        printf("＼");
                    else
                        printf("　");
                    break;
                case 8:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    if (lever.active == 1)
                        printf("／");
                    else
                        printf("　");
                    break;
                case 9:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                    printf("▣");    // 레버 밑판
                    lever.pos.X = x;
                    lever.pos.Y = y;
                    break;
                case 11:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 144);
                    printf("□");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 12:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                    printf("□");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 13:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
                    printf("▨");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 20:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 22:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case -22:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 112:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    printf("l ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 113:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    printf("▶");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 114:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                    printf("l ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 115:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                    printf("▶");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 23:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                    printf("▨");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 24:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 25:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                    printf("▨");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 31:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 64);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 32:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 16);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 33:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 80);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 44:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 144);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case -3:
                    if (floodingState == 1 && surFaceHeight <= y)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 144);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    if (map_id == 2) {

                        printf("　");
                    }
                    break;
                case 71://빨간색
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 72://검정색
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 73://파란색
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 16);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 74://초록색
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;
                case 75://흰색
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("  ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 91:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("① ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 92:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("② ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 93:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("③ ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 94:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("④ ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 95:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("⑤ ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 96:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("⑥ ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 97:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("⑦ ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 98:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("⑧ ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                case 99:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    printf("⑨ ");
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    break;

                default:
                    printf("　");
                    break;
                }
            }
    }
}

void ShowPlayer()
{
    if (map_id == 5 || map_id == 7) {
        for (int y = 1; y < GBOARD_HEIGHT + 1; y++)
            for (int x = 1; x < GBOARD_WIDTH + 1; x++)
            {
                int idxX = player.pos.X - GBOARD_ORIGIN_X;
                idxX /= 2;
                int idxY = player.pos.Y - GBOARD_ORIGIN_Y;

                if (!(idxX - 9 < x && x < idxX + 9 && idxY - 9 < y && y < idxY + 9)) {
                    continue;
                }
                if (idxX - 7 < x && x < idxX + 7 && idxY - 7 < y && y < idxY + 7)
                {
                    if (x == idxX && y == idxY)
                        continue;

                    int block = map[map_id][y][x];

                    SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);

                    switch (block) {
                    case -1:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                        printf("■");
                        break;

                    case 1: case 2:
                        if (map_id == 7) {
                            if (y == 0 || x == 0 || y == GBOARD_HEIGHT + 1 || x == GBOARD_WIDTH + 1)
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

                            }
                            else {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                            }
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else {
                            if (y == 0 || x == 0 || y == GBOARD_HEIGHT + 1 || x == GBOARD_WIDTH + 1)
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                                printf("  ");
                            }
                            else if (x < 20) {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                                printf("◆");
                            }
                            else if (x < 39)
                            {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                                printf("◆");
                            }
                            else if (x < 58) {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
                                printf("◆");
                            }
                            else {
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
                                printf("◆");
                            }
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
                        }
                        break;
                    case 3:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 5:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                        printf("▣");
                        break;
                    case 7:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        if (lever.active == 0)
                            printf("＼");
                        else
                            printf("　");
                        break;
                    case 8:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        if (lever.active == 1)
                            printf("／");
                        else
                            printf("　");
                        break;
                    case 9:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                        printf("▣");    // 레버 밑판
                        lever.pos.X = x;
                        lever.pos.Y = y;
                        break;
                    case 11:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 144);
                        printf("□");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 12:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                        printf("□");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 13:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
                        printf("▨");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 20:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 22:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 112:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        printf("l ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 113:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        printf("▶");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 114:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                        printf("l ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    case 115:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                        printf("▶");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        break;
                    default:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        printf("　");

                        break;
                    }
                }
                else
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);
                    printf("  ");
                }

            }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    if (player.pos.Y - 2 >= surFaceHeight && floodingState == 1 && map_id == 3) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 144 + 15);
    }
    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    for (int y = 0; y < 3; y++) {
        SetCurrentCursorPos(player.pos.X, player.pos.Y - 2 + y);
        if (playerModel[y] == 2 && player.box == 1)
            printf("▤");
        if (playerModel[y] == 1)
            printf("ο");
        if (playerModel[y] == 3)
            printf("∇");
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void DeletePlayer()
{
    SetCurrentCursorPos(player.pos.X, player.pos.Y);
    for (int y = 0; y < 3; y++) {
        SetCurrentCursorPos(player.pos.X, player.pos.Y - 2 + y);
        if ((player.box == 1 && playerModel[y] == 2) || playerModel[y] == 1 || playerModel[y] == 3)
            printf("  ");
    }
}

void setPortalSetting()
{
    mapToMap[1][2] = 22;
    mapToMap[2][1] = 20;
    mapToMap[2][3] = 22;
    mapToMap[3][2] = 20;
    mapToMap[3][4] = 22;
    mapToMap[3][6] = 24;
    mapToMap[4][3] = 20;
    mapToMap[4][5] = 22;
    mapToMap[5][4] = 20;
    mapToMap[6][3] = 20;
    mapToMap[6][7] = 22;
    mapToMap[7][6] = 20;
    mapToMap[7][8] = 22;
    mapToMap[8][7] = 20;
    mapToMap[8][9] = 22;
    mapToMap[9][8] = 20;
    mapToMap[9][10] = 22;
    mapToMap[9][11] = 44;
    mapToMap[10][9] = 20;
}