/*
 * Matrix_solver.c
 *
 *  Created on: Aug 26, 2021
 *      Author: Admin
 */
#include "Matrix_solver.h"
#include <stdio.h>
#include <string.h>
#define max 100

int Dijkstra(int A[12][12], int n, int D, int C, int result[max])
{
    char DanhDau[max];
    int Nhan[max], Truoc[max], XP, min;
    for (int i = 0; i < n; i++)
    {
        Nhan[i] = 99;
        DanhDau[i] = 0;
        Truoc[i] = D;
    }
    Nhan[D] = 0;
    DanhDau[D] = 1;
    XP = D;
    while (XP != C)
    {
        for (int j = 0; j < n; j++)
            if (A[XP][j] > 0 && Nhan[j] > A[XP][j] + Nhan[XP] && DanhDau[j] == 0)
            {
                Nhan[j] = A[XP][j] + Nhan[XP];
                Truoc[j] = XP;
            }
        min = 99;
        for (int j = 0; j < n; j++)
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
void Control(int C[12][12], char control[12][10], int result[max], int len, char controlArr[99][99])
{
    for (int i = 0; i < len - 1; i++)
    {
        strcpy(controlArr[i], control[C[result[i]][result[i + 1]]]);
    }
}
void Solver(int Dau ,int Cuoi,char *takeResult)
{
    char buffer[5]={0};
    char concat_buffer[20]={0};
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

    int Action_Matrix[12][12] = {
        {-1, 8, -1, -1, -1, -1, -1, 3, -1, -1, 0, -1},
        {9, -1, 0, -1, -1, -1, -1, -1, -1, -1, 3, -1},
        {-1, 6, -1, 11, -1, -1, -1, -1, -1, 4, -1, -1},
        {-1, -1, 10, -1, 4, -1, -1, -1, -1, 6, -1, -1},
        {-1, -1, -1, 1, -1, 9, -1, -1, -1, -1, -1, 2},
        {-1, -1, -1, -1, 8, -1, 2, -1, -1, -1, -1, 1},
        {-1, -1, -1, -1, -1, 5, -1, 10, 7, -1, -1, -1},
        {7, -1, -1, -1, -1, -1, 11, -1, 5, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, 3, 2, -1, 8, 1, 0},
        {-1, -1, 1, 0, -1, -1, -1, -1, 9, -1, 2, 3},
        {6, 7, -1, -1, -1, -1, -1, -1, 4, 5, -1, 11},
        {-1, -1, -1, -1, 5, 4, -1, -1, 6, 7, 10, -1}};
    //result la mang chua quang duong
    //len la do dai mang
    int result[max];
    char control[12][10] = {"0", "1", "2", "3", "2", "3", "3", "2", "8", "8", "8", "8"};
    int length = Dijkstra(A, 12, Dau, Cuoi, result);
    char controlArr[99][99];
    Control(Action_Matrix, control, result, length, controlArr);
    // for (int i = 0; i < length - 1; i++)
    // {
    //     if (i == length - 2)
    //     {
    //         printf("%s\n", controlArr[i]);
    //     }
    //     else
    //         printf("%s -> ", controlArr[i]);
    // }
    for (int i = 0; i < length; i++)
    {
        if(i == length - 1)
        {
            sprintf(buffer,"%d",result[i]);
            strcat(concat_buffer,buffer);
        }
        else
        {
            sprintf(buffer,"%d -> ",result[i]);
            strcat(concat_buffer,buffer);
        }
    }
    strcpy(takeResult,concat_buffer);
}

	/*	Cac buoc di
	  8 -> quay ngược/đi thẳng
	  3 -> quẹo trái
	  0 -> quẹo phải
      1 -> quẹo trái
      2 -> quẹo phải
    */
