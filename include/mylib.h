#pragma once

#include <windows.h>

char* Pwd();
int CheckPwd();
void gotoxy(short x, short y);
int wherex();
int wherey();
void clreol();
void SetColor(WORD color);
void SetBGColor(WORD color);
void clrscr();
