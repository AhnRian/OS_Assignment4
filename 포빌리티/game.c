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


//발전소
void switchLever_map_8();
void switchLever_map_11();

void showText(char str[]) {
    printf("        ");
    SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH / 2 + 2, 1);
    printf("%s", str);
    SetCurrentCursorPos(player.pos.X, player.pos.Y);
}

void showPortalInfo() {
    printf("        ");
    SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH / 2 + 2, 1);
    printf("in: %d, out: %d", in.mode, out.mode);
    SetCurrentCursorPos(player.pos.X, player.pos.Y);
}
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


void nextStage()
{
    int stageNum = map_id;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    for (int x = 0; x < (GBOARD_WIDTH + 2) / 2; x++)
    {
        for (int y = 0; y < GBOARD_HEIGHT + 2; y++)
        {
            SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);
            printf("■");
            SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1 - x) * 2, GBOARD_ORIGIN_Y + y);
            printf("■");

        }
        Sleep(20);
    }

    SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH / 2) * 2, GBOARD_ORIGIN_Y + GBOARD_HEIGHT / 2);
    printf("Stage :  %d", stageNum + 1);

    Sleep(1000);

    map_id++;
    visited[map_id] = 1;
    if (map_id == 11 || map_id == 10)
        time_gameover = (int)time_check;

    for (int y = 0; y < GBOARD_HEIGHT + 2; y++)
    {
        for (int x = 0; x <= (GBOARD_WIDTH + 2); x++)
        {
            map[map_id - 1][y][x] = tmp[map_id - 1][y][x];
            if (map_id < 10)
                map[map_id + 1][y][x] = tmp[map_id + 1][y][x];
        }
    }

    if (map_id == 5 || map_id == 7) {

        for (int x = (GBOARD_WIDTH + 2) / 2 - 1; x >= 0; x--)
        {
            for (int y = 0; y < GBOARD_HEIGHT + 2; y++)
            {
                if (y == 0 || x == 0 || x == (GBOARD_WIDTH + 2) || y == GBOARD_HEIGHT + 1) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);
                    printf("  ");
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1 - x) * 2, GBOARD_ORIGIN_Y + y);
                    printf("  ");

                }
                else {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + x * 2, GBOARD_ORIGIN_Y + y);
                    printf("  ");
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + (GBOARD_WIDTH + 1 - x) * 2, GBOARD_ORIGIN_Y + y);
                    printf("  ");

                }

            }
            Sleep(20);
        }
    }
    else {
        for (int x = 0; x <= (GBOARD_WIDTH + 2) / 2; x++)
        {
            for (int y = 0; y < GBOARD_HEIGHT + 2; y++)
            {

                SetCurrentCursorPos(GBOARD_ORIGIN_X + ((GBOARD_WIDTH + 2) / 2 - x) * 2, GBOARD_ORIGIN_Y + y);




                //왼쪽
                if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 0)
                    printf("  ");
                else {
                    if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == -1)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 1 || map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 3 || map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 2) {
                        if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                        else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);

                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 20)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 22)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 23)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                        printf("▨");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 24)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 25)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                        printf("▨");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 31)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 64);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 32)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 16);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 33)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 80);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 5) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                        printf("■");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 7) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        if (inkflag == 0)
                            printf("＼");
                        else printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 8) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        if (inkflag == 1)
                            printf("／");
                        else printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 9) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                        printf("▣");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 71)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 72)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 73)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 16);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 74)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 75)
                    {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 20 || map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 22) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
                        printf("  ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 114) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                        printf("l ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 115) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                        printf("▶");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 112) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        printf("l ");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }
                    else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 113) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        printf("▶");
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    }

                }

                // 오른쪽
                if (x != (GBOARD_WIDTH + 2) / 2) {
                    SetCurrentCursorPos(GBOARD_ORIGIN_X + ((GBOARD_WIDTH + 2) / 2 + x) * 2, GBOARD_ORIGIN_Y + y);

                    if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 0)
                        printf("  ");
                    else {
                        if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == -1)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 1 || map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 3 || map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 2) {
                            if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 3)
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                            else
                                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 20)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 22)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 - x] == 31)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 64);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 32)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 16);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 33)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 80);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 5) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                            printf("■");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 7) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                            if (inkflag == 0)
                                printf("＼");
                            else printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 8) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                            if (inkflag == 1)
                                printf("／");
                            else printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 9) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                            printf("▣");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 71)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 192);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 72)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 73)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 16);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 74)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 75)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 20 || map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 22) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 23)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                            printf("▨");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 24)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 25)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 32);
                            printf("▨");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 114) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                            printf("l ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 115) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                            printf("▶");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 112) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                            printf("l ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == 113) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                            printf("▶");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                        else if (map[map_id][y][(GBOARD_WIDTH + 2) / 2 + x] == -22)
                        {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
                            printf("  ");
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                        }
                    }
                }


            }
            Sleep(20);
        }
    }

    bulletCnt = 20;
    deleteDirection();
    player.angle = 0;
    angle = 0b0000;
    DrawingDirection();
    setPortal(&in);
    setPortal(&out);
    setPortal(&Mirror);
    setObject(&lever, 0, 0);
    setObject(&plate, 0, 0);

    printUI();
}

void printUI(void)
{
    DrawingMiniMap_S();
    DrawingGunState();
    DrawingBullet();
    DrawingItem();
    DrawingDirection();
}

void DrawingMiniMap_S() {
    COORD pos = GetCurrentCursorPos();

    for (int y = 0; y < 9; y++)
        for (int x = 0; x < 9; x++)
        {
            SetCurrentCursorPos(GBOARD_ORIGIN_X + 10 + (GBOARD_WIDTH * 2) + (x * 2), GBOARD_ORIGIN_Y + y);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
            switch (miniMap[y][x])
            {
            case -1: // 세로
                printf("│"); break;
            case -2: //가로
                printf("─"); break;
            case -3: // 왼쪽 위
                printf("┌"); break;
            case -4: // 오른쪽 위
                printf("┐"); break;
            case -5: // 왼쪽 아래
                printf("└"); break;
            case -6: // 오른쪽 아래
                printf("┘"); break;

            case 1:
                if (visited[1])
                    printf("■"); break;
            case 11:
                if (visited[1])
                    printf("─"); break;
            case 2:
                if (visited[2])
                    printf("■"); break;
            case 12:
                if (visited[2])
                    printf("│"); break;
            case 3:
                if (visited[3])
                    printf("■"); break;
            case 13:
                if (visited[3])
                    printf("│"); break;
            case 23:
                if (visited[3])
                    printf("─"); break;
            case 4:
                if (visited[4])
                    printf("■"); break;
            case 14:
                if (visited[4])
                    printf("─"); break;
            case 5:
                if (visited[5])
                    printf("■"); break;
            case 6:
                if (visited[6])
                    printf("■"); break;
            case 16:
                if (visited[6])
                    printf("─"); break;
            case 7:
                if (visited[7])
                    printf("■"); break;
            case 17:
                if (visited[7])
                    printf("│"); break;
            case 8:
                if (visited[8])
                    printf("■"); break;
            case 18:
                if (visited[8])
                    printf("│"); break;
            case 9:
                if (visited[9])
                    printf("■"); break;
            }
        }

    for (int y = 0; y < 9; y++)
        for (int x = 0; x < 9; x++)
        {
            SetCurrentCursorPos(GBOARD_ORIGIN_X + 10 + (GBOARD_WIDTH * 2) + (x * 2), GBOARD_ORIGIN_Y + y);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
            switch (miniMap[y][x])
            {
            case 1:
                if (visited[1] && map_id == 1)
                    printf("■"); break;
            case 2:
                if (visited[2] && map_id == 2)
                    printf("■"); break;
            case 3:
                if (visited[3] && map_id == 3)
                    printf("■"); break;
            case 4:
                if (visited[4] && map_id == 4)
                    printf("■"); break;
            case 5:
                if (visited[5] && map_id == 5)
                    printf("■"); break;
            case 6:
                if (visited[6] && map_id == 6)
                    printf("■"); break;
            case 7:
                if (visited[7] && map_id == 7)
                    printf("■"); break;
            case 8:
                if (visited[8] && map_id == 8)
                    printf("■"); break;
            case 9:
                if (visited[9] && map_id == 9)
                    printf("■"); break;
            }
        }

  

    SetCurrentCursorPos(pos.X, pos.Y);
}

void extendMiniMap()
{
    for (int y = 1; y < 8; y++)
    {
        for (int x = 1; x < 8; x++)
        {
            for (int i = 3 * y - 2; i <= 3 * y; i++)
            {
                for (int j = 3 * x - 2; j <= 3 * x; j++)
                {
                    if (miniMap1[locate1][y][x] == 4) {
                        if (i == 3 * y - 1)miniMap_B1[0][i][j] = miniMap1[locate1][y][x];
                        else
                            miniMap_B1[0][i][j] = 0;
                    }
                    else if (miniMap1[locate1][y][x] == 5) {
                        if (j == 3 * x - 1)miniMap_B1[0][i][j] = miniMap1[locate1][y][x];
                        else
                            miniMap_B1[0][i][j] = 0;
                    }
                    else miniMap_B1[0][i][j] = miniMap1[locate1][y][x];
                }
            }
        }
    }

    for (int y = 1; y < 8; y++)
    {
        for (int x = 1; x < 8; x++)
        {
            for (int i = 3 * y - 2; i <= 3 * y; i++)
            {
                for (int j = 3 * x - 2; j <= 3 * x; j++)
                {
                    if (miniMap_B1[0][i][j] == 0) {
                        if (miniMap2[locate2][y][x] == 4) {
                            if (i == 3 * y - 1)miniMap_B1[0][i][j] = miniMap2[locate2][y][x];
                            else
                                miniMap_B1[0][i][j] = 0;
                        }
                        else if (miniMap2[locate2][y][x] == 5) {
                            if (j == 3 * x - 1)miniMap_B1[0][i][j] = miniMap2[locate2][y][x];
                            else
                                miniMap_B1[0][i][j] = 0;
                        }
                        else miniMap_B1[0][i][j] = miniMap2[locate2][y][x];
                    }
                }
            }
        }

    }

}


void DrawingItem(void)
{
    COORD pos = GetCurrentCursorPos();
    int num = 0;

    if (item == 1)num = 15;
    else num = 8;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            SetCurrentCursorPos(GBOARD_ORIGIN_X + 1 * 9 + 6 + GBOARD_WIDTH * 2 + x * 2 - 6, GBOARD_ORIGIN_Y + 39 + y);
            {
                switch (ItemState[0][y][x])
                {

                case 2:printf("│"); break;
                case 3:printf("─"); break;
                case 6:printf("┌"); break;
                case 7:printf("┐"); break;
                case 8:printf("└"); break;
                case 9:printf("┘"); break;
                default: printf("  ");
                    break;
                }
            }
        }
    }


    if (item == 1)num = 14;
    else num = 8;

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
    SetCurrentCursorPos(GBOARD_ORIGIN_X + 1 * 9 + 6 + GBOARD_WIDTH * 2 + 1 * 2 - 3, GBOARD_ORIGIN_Y + 39 + 1);

    printf("※손전등※");


    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    SetCurrentCursorPos(pos.X, pos.Y);
}
void DrawingGunState()
{

    COORD pos = GetCurrentCursorPos();
    SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 6, GBOARD_ORIGIN_Y + 10);
    int num = 0;
    for (int i = 0; i < 3; i++)
    {
        switch (i) {
        case 0: if (gunOption == 1)num = 9;
              else num = 8;
            break;

        case 1: if (gunOption == 2)num = 12;
              else num = 8;
            break;
        case 2: if (gunOption == 3)num = 14;
              else num = 8;
            break;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
        for (int y = 0; y < 3; y++)
        {

            for (int x = 0; x < 4; x++)
            {
                SetCurrentCursorPos(GBOARD_ORIGIN_X + i * 9 + 6 + GBOARD_WIDTH * 2 + x * 2, GBOARD_ORIGIN_Y + 10 + y);
                {


                    switch (gunState[0][y][x])
                    {

                    case 2:printf("│"); break;
                    case 3:printf("─"); break;
                    case 6:printf("┌"); break;
                    case 7:printf("┐"); break;
                    case 8:printf("└"); break;
                    case 9:printf("┘"); break;
                    default: printf("  ");
                        break;
                    }
                }
            }
        }
    }


    for (int i = 0; i < 3; i++) {

        switch (i) {
        case 0: if (gunOption == 1)num = 9;
              else num = 8;
            break;

        case 1: if (gunOption == 2)num = 12;
              else num = 8;
            break;
        case 2: if (gunOption == 3)num = 14;
              else num = 8;
            break;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
        SetCurrentCursorPos(GBOARD_ORIGIN_X + i * 9 + 6 + GBOARD_WIDTH * 2 + 1 * 2, GBOARD_ORIGIN_Y + 10 + 1);
        switch (i) {
        case 0: printf(" 총1");   break;
        case 1: printf(" 총2");   break;
        case 2: printf("거울");   break;
        }


    }



    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    SetCurrentCursorPos(pos.X, pos.Y);
}
void DrawingBullet(void)
{
    COORD pos = GetCurrentCursorPos();

    SetCurrentCursorPos(GBOARD_ORIGIN_X + 16 + (GBOARD_WIDTH * 2) + (bulletCnt) / 10 * 4, GBOARD_ORIGIN_Y + 26 + bulletCnt % 10);
    printf("  ");

    if (bulletCnt == 20)
    {
        int cnt = 10;
        if (bulletCnt < 10)cnt = bulletCnt;
        for (int y = 0; y < cnt; y++)
        {
            SetCurrentCursorPos(GBOARD_ORIGIN_X + 14 + (GBOARD_WIDTH * 2), GBOARD_ORIGIN_Y + 27 + y);
            printf("●");
        }
        if (bulletCnt > 10) {
            for (int y = 0; y < bulletCnt - 10; y++)
            {
                SetCurrentCursorPos(GBOARD_ORIGIN_X + 14 + (GBOARD_WIDTH * 2) + 8, GBOARD_ORIGIN_Y + 27 + y);
                printf("●");
            }
        }
    }
    SetCurrentCursorPos(GBOARD_ORIGIN_X + 14 + (GBOARD_WIDTH * 2) + 2, GBOARD_ORIGIN_Y + 25);
    printf("        ");
    SetCurrentCursorPos(GBOARD_ORIGIN_X + 14 + (GBOARD_WIDTH * 2) + 2, GBOARD_ORIGIN_Y + 25);
    printf("%d / 20", bulletCnt);

    SetCurrentCursorPos(pos.X, pos.Y);
}
void DeleteBullet()
{
    int num = bulletCnt;
    if (bulletCnt >= 10) {
        num = bulletCnt - 10;
        SetCurrentCursorPos(GBOARD_ORIGIN_X + 14 + (GBOARD_WIDTH * 2) + 8, GBOARD_ORIGIN_Y + 27 + num);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        printf("  ");
    }
    else {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + 14 + (GBOARD_WIDTH * 2), GBOARD_ORIGIN_Y + 27 + num);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        printf("  ");
    }



    SetCurrentCursorPos(GBOARD_ORIGIN_X + 14 + (GBOARD_WIDTH * 2) + 2, GBOARD_ORIGIN_Y + 25);
    printf("        ");
    SetCurrentCursorPos(GBOARD_ORIGIN_X + 14 + (GBOARD_WIDTH * 2) + 2, GBOARD_ORIGIN_Y + 25);
    printf("%d / 20", bulletCnt);
    SetCurrentCursorPos(player.pos.X, player.pos.Y);

    cnt_shoot++;
}
void DrawingDirection() {

    COORD pos = GetCurrentCursorPos();


    SetCurrentCursorPos(GBOARD_ORIGIN_X + 10 + GBOARD_WIDTH * 2, GBOARD_ORIGIN_Y + 14);
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {

            SetCurrentCursorPos(GBOARD_ORIGIN_X + 10 + GBOARD_WIDTH * 2 + x * 2, GBOARD_ORIGIN_Y + 14 + y);
            //direction[player.angle / 45][y][x] != 5 && direction[player.angle / 45][y][x] >= 2
            if (direction[player.angle / 45][y][x] != 1) {
                if (gunOption == 1)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                else if (gunOption == 2)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                else if (gunOption == 3)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            }
            switch (direction[player.angle / 45][y][x]) {
            case 1: printf("■");    break;
            case 2: printf("▶");    break;
            case 3: printf("◀");    break;
            case 4: printf("▲");    break;
            case 5: printf("▼");    break;
            case 6: printf("●");    break;
            case 7: printf("●");    break;

            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

        }
    }
    SetCurrentCursorPos(pos.X, pos.Y);

}

void deleteDirection()
{
    COORD pos = GetCurrentCursorPos();

    SetCurrentCursorPos(GBOARD_ORIGIN_X + 10 + GBOARD_WIDTH * 2, GBOARD_ORIGIN_Y + 14);
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {

            SetCurrentCursorPos(GBOARD_ORIGIN_X + 10 + GBOARD_WIDTH * 2 + x * 2, GBOARD_ORIGIN_Y + 14 + y);
            if (direction[player.angle / 45][y][x] > 1)
                printf("  ");

        }
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    SetCurrentCursorPos(pos.X, pos.Y);

}

void DrawUiFrame()
{

    for (int x = 0; x <= 14; x++) {

        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + x * 2 + 4, GBOARD_ORIGIN_Y - 1);
        if (x == 0)printf("┏");
        else if (x == 14)printf("┓");
        else printf("━");
    }

    for (int x = 0; x <= 14; x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + x * 2 + 4, GBOARD_ORIGIN_Y + GBOARD_HEIGHT + 2);
        if (x == 0)printf("┗");
        else if (x == 14)printf("┛");
        else printf("━");
    }

    for (int y = 0; y <= GBOARD_HEIGHT + 1; y++)
    {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 0 * 2 + 4, GBOARD_ORIGIN_Y + y);
        printf("┃");
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + 14 * 2 + 4, GBOARD_ORIGIN_Y + y);
        printf("┃");
    }
    /////////////////////////////////////////////////////////////////
    for (int x = 0; x <= 14; x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + x * 2 + 4, GBOARD_ORIGIN_Y + 9);
        if (x == 0)printf("┣");
        else if (x == 14)printf("┫");
        else printf("━");
    }

    for (int x = 0; x <= 14; x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + x * 2 + 4, GBOARD_ORIGIN_Y + 10 + 3);
        if (x == 0)printf("┣");
        else if (x == 14)printf("┫");
        else printf("━");
    }

    for (int x = 0; x <= 14; x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + x * 2 + 4, GBOARD_ORIGIN_Y + 23);
        if (x == 0)printf("┣");
        else if (x == 14)printf("┫");
        else printf("━");
    }

    for (int x = 0; x <= 14; x++) {
        SetCurrentCursorPos(GBOARD_ORIGIN_X + GBOARD_WIDTH * 2 + x * 2 + 4, GBOARD_ORIGIN_Y + 38);
        if (x == 0)printf("┣");
        else if (x == 14)printf("┫");
        else printf("━");
    }
}