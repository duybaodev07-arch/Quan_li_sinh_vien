#include "mylib.h"

#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
const int ENTER_KEY = 13;
const char PASSWORD[] = "abcdef";
}

char* Pwd() {
    static char password[40];
    int index = 0;

    while (index < 39) {
        const int key = _getch();
        if (key == ENTER_KEY) {
            break;
        }
        if (key == 8) {
            if (index > 0) {
                --index;
                std::printf("\\b \\b");
            }
            continue;
        }

        password[index] = static_cast<char>(key);
        std::printf("*");
        ++index;
    }

    password[index] = '\0';
    return password;
}

int CheckPwd() {
    for (int attempt = 1; attempt <= 3; ++attempt) {
        std::printf("Password: ");
        if (std::strcmp(Pwd(), PASSWORD) == 0) {
            return 1;
        }
        std::printf("\nPassword sai. Hay nhap lai\n");
    }
    return 0;
}

void gotoxy(short x, short y) {
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    const COORD position = { x, y };
    SetConsoleCursorPosition(output, position);
}

int wherex() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(output, &info)) {
        return 0;
    }
    return info.dwCursorPosition.X;
}

int wherey() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(output, &info)) {
        return 0;
    }
    return info.dwCursorPosition.Y;
}

void clreol() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(output, &info)) {
        return;
    }

    COORD position = info.dwCursorPosition;
    DWORD written = 0;
    const DWORD remaining = info.dwSize.X - info.dwCursorPosition.X;
    FillConsoleOutputCharacter(output, ' ', remaining, position, &written);
    SetConsoleCursorPosition(output, position);
}

void SetColor(WORD color) {
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(output, &info)) {
        return;
    }

    const WORD attributes = static_cast<WORD>(
        (info.wAttributes & 0xFFF0) | (color & 0x000F)
    );
    SetConsoleTextAttribute(output, attributes);
}

void SetBGColor(WORD color) {
    const HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(output, &info)) {
        return;
    }

    const WORD background = static_cast<WORD>((color & 0x000F) << 4);
    const WORD attributes = static_cast<WORD>(
        (info.wAttributes & 0xFF0F) | background
    );
    SetConsoleTextAttribute(output, attributes);
}

void clrscr() {
    std::system("cls");
}
