#include "Matrix_solver.h"
#define max 12
static void Control_File(int A[12][12][12])
{
    for (int8_t i = 0; i < 12; i++)
        for (int8_t j = 0; j < 12; j++)
            for (int8_t k = 0; k < 5; k++)
                A[i][j][k] = -1;
    //A[][][]=5;

    //di thang
    A[0][1][7] = 0;
    A[1][0][2] = 0;
    A[2][3][1] = 0;
    A[3][2][4] = 0;
    A[4][5][3] = 0;
    A[5][4][6] = 0;
    A[6][7][5] = 0;
    A[7][6][0] = 0;
    A[8][9][6] = 0;
    A[8][9][7] = 0;
    A[9][8][2] = 0;
    A[9][8][3] = 0;
    A[10][11][0] = 0;
    A[10][11][1] = 0;
    A[11][10][4] = 0;
    A[11][10][5] = 0;

    //quay 180 đi thẳng
    A[0][1][10] = 1;
    A[0][1][1] = 1;
    A[1][0][0] = 1;
    A[1][0][10] = 1;
    A[2][3][3] = 1;
    A[2][3][9] = 1;
    A[3][2][9] = 1;
    A[3][2][2] = 1;
    A[4][5][5] = 1;
    A[4][5][11] = 1;
    A[5][4][4] = 1;
    A[5][4][11] = 1;
    A[6][7][7] = 1;
    A[6][7][8] = 1;
    A[7][6][6] = 1;
    A[7][6][8] = 1;
    A[8][9][10] = 1;
    A[8][9][9] = 1;
    A[8][9][11] = 1;
    A[9][8][8] = 1;
    A[9][8][10] = 1;
    A[9][8][11] = 1;
    A[10][11][8] = 1;
    A[10][11][9] = 1;
    A[10][11][11] = 1;
    A[11][10][8] = 1;
    A[11][10][9] = 1;
    A[11][10][10] = 1;

    //quay 180 đi thẳng
    A[0][10][10] = 2;
    A[0][10][1] = 2;
    A[1][2][2] = 2;
    A[2][9][9] = 2;
    A[2][9][3] = 2;
    A[3][4][4] = 2;
    A[4][11][5] = 2;
    A[5][6][6] = 2;
    A[6][8][8] = 2;
    A[6][8][7] = 2;
    A[7][0][0] = 2;
    A[8][11][9] = 2;
    A[8][11][10] = 2;
    A[8][11][11] = 2;
    A[8][7][6] = 2;
    A[8][7][7] = 2;
    A[9][3][2] = 2;
    A[9][3][3] = 2;
    A[9][10][8] = 2;
    A[9][10][10] = 2;
    A[9][10][11] = 2;
    A[10][1][0] = 2;
    A[10][1][1] = 2;
    A[10][8][8] = 2;
    A[10][8][9] = 2;
    A[10][8][11] = 2;
    A[11][5][4] = 2;
    A[11][5][5] = 2;
    A[11][9][8] = 2;
    A[11][9][9] = 2;
    A[11][9][10] = 2;

    //thẳng phải
    A[0][10][7] = 3;
    A[1][2][10] = 3;
    A[1][2][0] = 3;
    A[2][9][1] = 3;
    A[3][4][9] = 3;
    A[3][4][2] = 3;
    A[4][11][3] = 3;
    A[5][6][4] = 3;
    A[5][6][11] = 3;
    A[6][8][5] = 3;
    A[7][0][6] = 3;
    A[7][0][8] = 3;
    A[8][11][6] = 3;
    A[8][11][7] = 3;
    A[8][7][9] = 3;
    A[8][7][10] = 3;
    A[8][7][11] = 3;
    A[9][3][8] = 3;
    A[9][3][10] = 3;
    A[9][3][11] = 3;
    A[9][10][2] = 3;
    A[9][10][3] = 3;
    A[10][1][8] = 3;
    A[10][1][9] = 3;
    A[10][1][11] = 3;
    A[10][8][0] = 3;
    A[10][8][1] = 3;
    A[11][5][8] = 3;
    A[11][5][9] = 3;
    A[11][5][10] = 3;
    A[11][9][4] = 3;
    A[11][9][5] = 3;

    //thẳng, trái
    A[0][7][10] = 4;
    A[0][7][1] = 4;
    A[1][10][2] = 4;
    A[2][1][3] = 4;
    A[2][1][9] = 4;
    A[3][9][4] = 4;
    A[4][3][11] = 4;
    A[4][3][5] = 4;
    A[5][11][6] = 4;
    A[6][5][7] = 4;
    A[6][5][8] = 4;
    A[7][8][0] = 4;
    A[8][10][6] = 4;
    A[8][10][7] = 4;
    A[8][6][9] = 4;
    A[8][6][10] = 4;
    A[8][6][11] = 4;
    A[9][2][8] = 4;
    A[9][2][10] = 4;
    A[9][2][11] = 4;
    A[9][11][2] = 4;
    A[9][11][3] = 4;
    A[10][0][8] = 4;
    A[10][0][9] = 4;
    A[10][0][11] = 4;
    A[10][9][0] = 4;
    A[10][9][1] = 4;
    A[11][4][8] = 4;
    A[11][4][9] = 4;
    A[11][4][10] = 4;
    A[11][8][4] = 4;
    A[11][8][5] = 4;

    //quay 180, thẳng, trái
    A[0][7][7] = 5;
    A[1][10][10] = 5;
    A[1][10][0] = 5;
    A[2][1][1] = 5;
    A[3][9][9] = 5;
    A[3][9][2] = 5;
    A[4][3][3] = 5;
    A[4][11][11] = 2;
    A[5][11][11] = 5;
    A[5][11][4] = 5;
    A[6][5][5] = 5;
    A[7][8][8] = 5;
    A[7][8][6] = 5;
    A[8][10][9] = 5;
    A[8][10][10] = 5;
    A[8][10][11] = 5;
    A[8][6][6] = 5;
    A[8][6][7] = 5;
    A[9][2][2] = 5;
    A[9][2][3] = 5;
    A[9][11][8] = 5;
    A[9][11][10] = 5;
    A[9][11][11] = 5;
    A[10][0][0] = 5;
    A[10][0][1] = 5;
    A[10][9][8] = 5;
    A[10][9][11] = 5;
    A[10][9][9] = 5;
    A[11][4][4] = 5;
    A[11][4][5] = 5;
    A[11][8][8] = 5;
    A[11][8][9] = 5;
    A[11][8][10] = 5;
}
static int Dijkstra(int A[12][12], int n, int D, int C, int result[max])
{
    char DanhDau[max];
    int Nhan[max], Truoc[max], XP, min;
    for (int i = 0; i < n; i++)
    {
        Nhan[i] = 999;
        DanhDau[i] = 0;
        Truoc[i] = D;
    }
    Nhan[D] = 0;
    DanhDau[D] = 1;
    XP = D;
    while (XP != C)
    {
        for (int8_t j = 0; j < n; j++)
            if (A[XP][j] > 0 && Nhan[j] > A[XP][j] + Nhan[XP] && DanhDau[j] == 0)
            {
                Nhan[j] = A[XP][j] + Nhan[XP];
                Truoc[j] = XP;
            }
        min = 999;
        for (int8_t j = 0; j < n; j++)
            if (min > Nhan[j] && DanhDau[j] == 0)
            {
                min = Nhan[j];
                XP = j;
            }
        DanhDau[XP] = 1;
    }
    int lenResult = Nhan[C] + 1;
    int k = Nhan[C];
    result[k--] = C;
    result[k--] = Truoc[C];
    int i = Truoc[C];
    while (i != D)
    {
        i = Truoc[i];
        result[k--] = i;
    }
    return lenResult;
}
static void Control(int C[12][12][12], char control[6][3], int result[4], int len, char controlArr[12][12], int truoc, int takeResult[4])
{
    for (int8_t i = 0; i < len - 1; i++)
    {
        strcpy(controlArr[i], control[C[result[i]][result[i + 1]][truoc]]);
        // printf("%d", C[result[i]][result[i + 1]][truoc]);
        // printf("\n");
        // printf(control[C[result[i]][result[i + 1]][truoc]]);
        // printf("\n");
        takeResult[i] = C[result[i]][result[i + 1]][truoc];
        truoc = result[i];
    }
}
int8_t Solver(int Dau, int Cuoi, int Truoc, int takeResult[4], int Step[4], int Run_Str[8])
{
    char controlArr[12][12];
    int A[12][12] = {
        {0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
        {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1},
        {0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1},
        {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0}};
    // int result[max];
    char control[6][3] = {"0", "1", "2", "3", "4", "5"};
    int C[12][12][12];
    Control_File(C);
    int length = Dijkstra(A, 12, Dau, Cuoi, Step);
    Control(C, control, Step, length, controlArr, Truoc, takeResult);

    Run_Str[0] = 7;
	Run_Str[1] = takeResult[0];
	Run_Str[2] = 7;
	Run_Str[3] = takeResult[1];
	Run_Str[4] = 7;
	Run_Str[5] = takeResult[2];
	Run_Str[6] = 7;
	Run_Str[7] = takeResult[3];
    return length;
}
void GetString(int8_t Dau,int8_t Cuoi ,char *Result)
{
    char buffer[5] = {0};
    char concat_buffer[20] = {0};
    int getStep[4];
    int length = Solver(Dau,Cuoi,12,0,getStep,0);
    for (int8_t i = 0; i < length; i++)
    {
        if (i == length - 1)
        {
            sprintf(buffer, "%d", getStep[i]);
            strcat(concat_buffer, buffer);
        }
        else
        {
            sprintf(buffer, "%d -> ", getStep[i]);
            strcat(concat_buffer, buffer);
        }
    }
    strcpy(Result, concat_buffer);
}
void GetString_Transfer(int8_t Dau,int8_t Cuoi ,char *Result)
{
    char buffer[5] = {0};
    char concat_buffer[20] = {0};
    int getStep[4];
    int length = Solver(Dau,Cuoi,12,0,getStep,0);
    for (int8_t i = 0; i < length; i++)
    {
        if (i == length - 1)
        {
            sprintf(buffer, "%d\"", getStep[i]);
            strcat(concat_buffer, buffer);
        }
        else
        {
            sprintf(buffer, "%d ", getStep[i]);
            strcat(concat_buffer, buffer);
        }
    }
    strcpy(Result, concat_buffer);
}
