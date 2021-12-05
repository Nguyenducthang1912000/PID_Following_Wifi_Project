/*
 * Matrix_solver.h
 *
 *  Created on: Aug 26, 2021
 *      Author: Admin
 */
#pragma once
#include "main.h"
#include <stdio.h>
#include <string.h>
#ifndef SRC_MATRIX_SOLVER_H_
#define SRC_MATRIX_SOLVER_H_
int8_t Solver(int Dau, int Cuoi, int Truoc, int takeResult[4], int Step[4], int Run_Str[8]);
void GetString(int8_t Dau,int8_t Cuoi ,char *Result);
void GetString_Transfer(int8_t Dau,int8_t Cuoi ,char *Result);


#endif /* SRC_MATRIX_SOLVER_H_ */
