#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#define MAX_PLAYERS 100
#define INITIAL_LENGTH 4
#define MAX_INITIALS 100

struct Record {
    char initial[INITIAL_LENGTH];
    double time;
};

struct PlayerRecord {
    char initial[INITIAL_LENGTH];
    double time;
};

struct PlayerRecord playerRecords[MAX_PLAYERS][MAX_INITIALS];
int playerRecordCounts[MAX_PLAYERS] = { 0 };

/*
0: 검은색 (Black)
1: 파란색 (Blue)
2: 초록색 (Green)
3: 청록색 (Cyan)
4: 빨간색 (Red)
5: 자주색 (Magenta)
6: 노란색 (Yellow)
7: 흰색 (White)
8: 회색 (Gray)
9: 밝은 파란색 (Bright Blue)
10: 밝은 초록색 (Bright Green)
11: 밝은 청록색 (Bright Cyan)
12: 밝은 빨간색 (Bright Red)
13: 밝은 자주색 (Bright Magenta)
14: 밝은 노란색 (Bright Yellow)
15: 밝은 흰색 (Bright White)
*/

void saveRecord(struct Record records[], int* count, char initial[], double time, int playerIndex);
void showRecords(int index);


void display_rule(void);
void basic_map(void);
void display_map(int matrix[][11], int tx[], int ty[]);
void make_treasure(int tx[], int ty[]);
void move_arrow_key(char key, int* x1, int* y1, int x_b, int y_b);
void game_control(int tx[], int ty[]);
void gotoxy(int x, int y);
void setConsoleTextColor(int textColor);


int main(void) {
    struct Record records[MAX_PLAYERS];
    int count = 0;

    char initial[INITIAL_LENGTH];
    char userInput = 0;

    int tx[3], ty[3];
    int matrix[11][11] = { 0 };
    clock_t start, end;
    double pst;
    srand((unsigned)time(NULL));
    make_treasure(tx, ty);

    printf("게임을 시작합니다. 플레이어의 이니셜을 입력하세요: ");
    scanf(" %s", initial, (unsigned int)sizeof(initial));

    setConsoleTextColor(1);
    display_rule();
    start = clock();
    system("cls");
    game_control(tx, ty);
    end = clock();
    pst = (double)(end - start) / CLK_TCK;
    gotoxy(1, 14);
    printf("경과시간 : %1f초", pst);
    saveRecord(records, &count, initial, pst, 0);

    gotoxy(1, 15);
    setConsoleTextColor(1);
    printf("-----------------------------------------------\n");
    printf("|         보물을 모두 찾았습니다!!            |\n");
    printf("|   게임을 다시 하시려면 'r' 키를 누르세요.   |\n");
    printf("|      기록을 보시려면 'h' 키를 누르세요.     |\n");
    printf("|       종료하시려면 아무 키나 누르세요.      |\n");
    printf("-----------------------------------------------\n");

    setConsoleTextColor(7);
    while (getchar() != '\n');
    scanf("%c", &userInput, (unsigned int)sizeof(userInput));
    system("cls");

    if (userInput == 'r') {
        system("cls");
        main();
    }
    else if (userInput == 'h') {

        showRecords(0);
        printf("\n게임을 다시 플레이하시겠습니까? (r: 재시작, 아무키: 종료) ");
        while (getchar() != '\n');
        scanf("%c", &userInput, (unsigned int)sizeof(userInput));
        system("cls");
        if (userInput == 'r') {
            main();
        }
        else {
            printf("게임을 종료합니다.");
        }
    }
    else {
        printf("게임을 종료합니다.");
    }
    return 0;
}

void gotoxy(int x, int y) {
    COORD Pos = { x - 1, y - 1 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void setConsoleTextColor(int textColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), textColor);
}

void move_arrow_key(char key, int* x1, int* y1, int x_b, int y_b) {
    switch (key) {
    case 80:
        *y1 = *y1 + 1;
        if (*y1 > y_b)
            *y1 = y_b;
        break;
    case 75:
        *x1 = *x1 - 2;
        if (*x1 < 1)
            *x1 = 1;
        break;
    case 77:
        *x1 = *x1 + 2;
        if (*x1 > x_b)
            *x1 = x_b;
        break;
    case 72:
        *y1 = *y1 - 1;
        if (*y1 < 1)
            *y1 = 1;
        break;
    default:
        return;
    }
}

void display_rule(void) {
    setConsoleTextColor(1);
    gotoxy(1, 1);
    printf("------------------ 보물찾기 ------------------");
    gotoxy(1, 2);
    printf("|                                            |");
    gotoxy(1, 3);
    printf("|   아래의 바둑판 모양의 격자에는 보물이     |\n");
    gotoxy(1, 4);
    printf("|             숨겨져 있습니다.               |\n");
    gotoxy(1, 5);
    printf("|   방향키(↑↓←→) 키를 움직여서 찾습니다.     |\n");
    gotoxy(1, 6);
    printf("|   지뢰(※)를 밟으면 초기위치로 돌아갑니다.  |\n");
    gotoxy(1, 7);
    printf("|    문제를 틀려도 초기위치로 돌아갑니다.    |\n");
    gotoxy(1, 8);
    printf("----------------------------------------------");
    gotoxy(1, 9);
    basic_map();
    printf("아무키나 누르면 시작합니다.\n");
    int key = _getch();
}

void basic_map(void) {
    int i, j;
    setConsoleTextColor(7);
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++)
            printf("■ ");
        printf("\n");
    }
}

void make_treasure(int tx[], int ty[]) {
    int i;
    for (i = 0; i < 3; i++) {
        do {
            tx[i] = rand() % 11;
            ty[i] = rand() % 11;
        } while ((tx[i] <= 1) || (ty[i] <= 1));
    }
}

void display_map(int matrix[][11], int tx[], int ty[]) {
    int i, j;
    basic_map();
    for (i = 1; i <= 10; i++)
        for (j = 1; j <= 10; j++)
            if (matrix[i][j] == 1) {
                setConsoleTextColor(2);
                gotoxy(2 * i - 1, j);
                printf("□");
            }
            else if (matrix[i][j] == 2) {
                setConsoleTextColor(6);
                gotoxy(2 * i - 1, j);
                printf("★");
            }
            else if (matrix[i][j] == 3) {
                setConsoleTextColor(4);
                gotoxy(2 * i - 1, j);
                printf("※");
            }
}

void game_control(int tx[], int ty[]) {
    char key;
    int i, j, count = 0;
    int x = 1, y = 1;
    int matrix[11][11] = { 0 };

    do {
        gotoxy(x, y);
        printf("□");
        matrix[(x + 1) / 2][y] = 1;

        for (i = 0; i < 2; i++)
            if ((x + 1) / 2 == tx[i] && (y == ty[i])) {
                matrix[(x + 1) / 2][y] = 2;
                int answer, userAnswer;
                int num1 = rand() % 100;
                int num2 = rand() % 100;

                gotoxy(1, 14);
                if (count == 1) {
                    printf("\n문제: %d + %d = ", num1, num2);
                    scanf("%d", &userAnswer);

                    answer = num1 + num2;

                    if (userAnswer == answer) {
                        {
                            printf("정답! 보물을 찾았습니다.\n");
                            count++;
                            system("cls");
                        }
                    }
                    else {
                        { {
                                gotoxy(1, 13);
                                printf("문제를 틀렸습니다. 처음위치로 돌아갑니다.");
                                Sleep(1000);
                                gotoxy(1, 13);
                                printf("                  ");
                                system("cls");
                            }
                        x = 1; y = 1;
                        for (int a = 0; a <= 10; a++)
                            for (int b = 0; b <= 10; b++)
                                matrix[a][b] = 0;
                        }
                    }
                }
            }

        if ((x + 1) / 2 == tx[2] && (y == ty[2]))
            matrix[(x + 1) / 2][y] = 3;

        gotoxy(1, 1);
        display_map(matrix, tx, ty);

        count = 0;
        for (i = 1; i <= 10; i++)
            for (j = 1; j <= 10; j++)
                if (matrix[i][j] == 2)
                    count = count + 1;
                else if (matrix[i][j] == 3) {
                    gotoxy(1, 13);
                    printf("지뢰를 밟았습니다. 초기위치로 돌아갑니다");
                    Sleep(1000);
                    gotoxy(1, 13);
                    printf("                                        ");
                    x = 1; y = 1;
                    for (int a = 0; a <= 10; a++)
                        for (int b = 0; b <= 10; b++)
                            matrix[a][b] = 0;
                }
        gotoxy(1, 12);
        setConsoleTextColor(7);
        printf("찾은 보물(★)의 개수:%d", count);
        key = _getch();
        move_arrow_key(key, &x, &y, 19, 10);
    } while (count < 2);
}


void saveRecord(struct Record records[], int* count, char initial[], double time, int playerIndex) {
    if (*count < MAX_PLAYERS) {
        snprintf(records[*count].initial, INITIAL_LENGTH, "%s", initial);
        records[*count].time = time;
        (*count)++;
        snprintf(playerRecords[playerIndex][playerRecordCounts[playerIndex]].initial, INITIAL_LENGTH, "%s", initial);
        playerRecords[playerIndex][playerRecordCounts[playerIndex]].time = time;
        playerRecordCounts[playerIndex]++;
    }
    else {
        printf("더 이상 기록할 수 없습니다. 배열이 가득 찼습니다.\n");
    }
}

void showRecords(int playerIndex) {
    for (int i = 0; i < playerRecordCounts[playerIndex] - 1; i++) {
        for (int j = 0; j < playerRecordCounts[playerIndex] - i - 1; j++) {
            if (playerRecords[playerIndex][j].time > playerRecords[playerIndex][j + 1].time) {
                struct PlayerRecord temp = playerRecords[playerIndex][j];
                playerRecords[playerIndex][j] = playerRecords[playerIndex][j + 1];
                playerRecords[playerIndex][j + 1] = temp;
            }
        }
    }
    printf("\n<< 게임 기록 >>\n");
    for (int i = 0; i < playerRecordCounts[playerIndex]; i++) {
        printf("%d등: %s - %.2f초\n", i + 1, playerRecords[playerIndex][i].initial, playerRecords[playerIndex][i].time);
    } 
}

