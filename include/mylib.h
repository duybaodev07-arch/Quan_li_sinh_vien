#pragma once

#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define Enter 13
#define PASSWORD "abcdef"

char* Pwd() {
    static char S[40];
    int i = 0;

    while (i < 39) {
        S[i] = getch();

        if (S[i] == Enter)
            break;

        printf("%c", '*');
        i++;
    }

    S[i] = '\0';

    return S;
}

int CheckPwd() {
    int dem = 0;

    for (dem = 1; dem <= 3; dem++) {
        printf("Password: ");

        if (strcmp(Pwd(), PASSWORD) == 0)
            return 1;

        printf("\nPassword sai. Hay nhap lai\n");
    }

    return 0;
}

void gotoxy(short x, short y) {
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = { x, y };

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleCursorPosition(
        hConsoleOutput,
        Cursor_an_Pos
    );
}

int wherex() {
    HANDLE hConsoleOutput;
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(
        hConsoleOutput,
        &screen_buffer_info
    );

    return screen_buffer_info.dwCursorPosition.X;
}

int wherey() {
    HANDLE hConsoleOutput;
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(
        hConsoleOutput,
        &screen_buffer_info
    );

    return screen_buffer_info.dwCursorPosition.Y;
}

void clreol() {
    COORD coord;
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO info;

    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(
        hConsoleOutput,
        &info
    );

    coord.X = info.dwCursorPosition.X;
    coord.Y = info.dwCursorPosition.Y;

    FillConsoleOutputCharacter(
        hConsoleOutput,
        ' ',
        info.dwSize.X - info.dwCursorPosition.X,
        coord,
        &written
    );

    gotoxy(
        info.dwCursorPosition.X,
        info.dwCursorPosition.Y
    );
}

void SetColor(WORD color) {
    HANDLE hConsoleOutput;
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(
        hConsoleOutput,
        &screen_buffer_info
    );

    WORD wAttributes = screen_buffer_info.wAttributes;

    color &= 0x000f;
    wAttributes &= 0xfff0;
    wAttributes |= color;

    SetConsoleTextAttribute(
        hConsoleOutput,
        wAttributes
    );
}

void SetBGColor(WORD color) {
    HANDLE hConsoleOutput;
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(
        hConsoleOutput,
        &screen_buffer_info
    );

    WORD wAttributes = screen_buffer_info.wAttributes;

    color &= 0x000f;
    color <<= 4;

    wAttributes &= 0xff0f;
    wAttributes |= color;

    SetConsoleTextAttribute(
        hConsoleOutput,
        wAttributes
    );
}

void clrscr() {
    system("cls");
}