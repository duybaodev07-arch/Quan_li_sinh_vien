#include "ui/GiaoDienConsole.h"
#include <windows.h>
using namespace std;
namespace {
HANDLE hOut = nullptr;
HANDLE hIn = nullptr;
int lastMouseX = -1;
int lastMouseY = -1;
}

void GiaoDienConsole::khoiTaoConsole(const char* tieuDe) {
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hIn = GetStdHandle(STD_INPUT_HANDLE);

        SetConsoleOutputCP(65001); // ma code utf8
        SetConsoleCP(65001);
        SetConsoleTitleA(tieuDe);

        batNhapChuot();
        hienConTro(false);
        xoaManHinh();
    }

void GiaoDienConsole::batNhapChuot() {
        DWORD cheDoMoi = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
        SetConsoleMode(hIn, cheDoMoi);
    }

void GiaoDienConsole::hienConTro(bool dangHien) {
        CONSOLE_CURSOR_INFO thongTinConTro;
        GetConsoleCursorInfo(hOut, &thongTinConTro);
        if ((thongTinConTro.bVisible == TRUE) != dangHien) {
            thongTinConTro.bVisible = dangHien ? TRUE : FALSE;
            thongTinConTro.dwSize = 20;
            SetConsoleCursorInfo(hOut, &thongTinConTro);
        }
    }

void GiaoDienConsole::diDenToaDo(int x, int y) {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(hOut, coord);
    }

void GiaoDienConsole::datMau(int mauChu, int mauNen) {
        SetConsoleTextAttribute(hOut, (WORD)((mauNen << 4) | (mauChu & 0x0F)));
    }

void GiaoDienConsole::xoaManHinh(int mauNen) {
        hienConTro(false);
        datMau(XAM_NHAT, mauNen);
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
            DWORD soLuongO = csbi.dwSize.X * csbi.dwSize.Y;
            DWORD count;
            COORD toaDoBanDau = { 0, 0 };
            FillConsoleOutputCharacterA(hOut, ' ', soLuongO, toaDoBanDau, &count);
            FillConsoleOutputAttribute(hOut, csbi.wAttributes, soLuongO, toaDoBanDau, &count);
            SetConsoleCursorPosition(hOut, toaDoBanDau);
        }
    }

SuKienNhap GiaoDienConsole::docSuKien() {
        batNhapChuot();
        INPUT_RECORD ir;
        DWORD soLuongDaDoc;
        while (true) {
            if (!ReadConsoleInputA(hIn, &ir, 1, &soLuongDaDoc) || soLuongDaDoc == 0) {
                continue;
            }
            if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
                WORD vk = ir.Event.KeyEvent.wVirtualKeyCode;
                char ch = ir.Event.KeyEvent.uChar.AsciiChar;
                SuKienNhap suKien = { SK_KHONG, 0, lastMouseX, lastMouseY };

                switch (vk) {
                    case VK_UP:     suKien.type = SK_PHIM_LEN; return suKien;
                    case VK_DOWN:   suKien.type = SK_PHIM_XUONG; return suKien;
                    case VK_LEFT:   suKien.type = SK_PHIM_TRAI; return suKien;
                    case VK_RIGHT:  suKien.type = SK_PHIM_PHAI; return suKien;
                    case VK_RETURN: suKien.type = SK_PHIM_ENTER; return suKien;
                    case VK_ESCAPE: suKien.type = SK_PHIM_ESC; return suKien;
                    case VK_TAB:    suKien.type = (ir.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED) ? SK_PHIM_SHIFT_TAB : SK_PHIM_TAB; return suKien;
                    case VK_BACK:   suKien.type = EVT_KEY_BACKSPACE; return suKien;
                    case VK_DELETE: suKien.type = EVT_KEY_DELETE; return suKien;
                    case VK_INSERT: suKien.type = EVT_KEY_INSERT; return suKien;
                    case VK_HOME:   suKien.type = EVT_KEY_HOME; return suKien;
                    case VK_END:    suKien.type = EVT_KEY_END; return suKien;
                    case VK_PRIOR:  suKien.type = EVT_KEY_PGUP; return suKien;
                    case VK_NEXT:   suKien.type = EVT_KEY_PGDN; return suKien;
                    case VK_F1:     suKien.type = EVT_KEY_F1; return suKien;
                    case VK_F2:     suKien.type = EVT_KEY_F2; return suKien;
                    case VK_F3:     suKien.type = EVT_KEY_F3; return suKien;
                    case VK_F4:     suKien.type = EVT_KEY_F4; return suKien;
                    case VK_F5:     suKien.type = EVT_KEY_F5; return suKien;
                    default:
                        if (ch >= 32 && ch <= 126) {
                            suKien.type = SK_KY_TU;
                            suKien.ch = ch;
                            return suKien;
                        }
                        break;
                }
            } else if (ir.EventType == MOUSE_EVENT) {
                MOUSE_EVENT_RECORD mer = ir.Event.MouseEvent;
                int toaDoX = mer.dwMousePosition.X;
                int toaDoY = mer.dwMousePosition.Y;

                if (mer.dwEventFlags == MOUSE_WHEELED) {
                    short delta = (short)HIWORD(mer.dwButtonState);
                    SuKienNhap suKien = { delta > 0 ? SK_CON_LAN_LEN : SK_CON_LAN_XUONG, 0, toaDoX, toaDoY };
                    return suKien;
                } else if (mer.dwEventFlags == 0) {
                    lastMouseX = toaDoX;
                    lastMouseY = toaDoY;
                    if (mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
                        SuKienNhap suKien = { SK_CHUOT_TRAI, 0, toaDoX, toaDoY };
                        return suKien;
                    } else if (mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
                        SuKienNhap suKien = { SK_CHUOT_PHAI, 0, toaDoX, toaDoY };
                        return suKien;
                    }
                } else if (mer.dwEventFlags == MOUSE_MOVED) {
                    // Chỉ trả về sự kiện di chuyển chuột khi sang ô (X, Y) mới để chống nhấp nháy!
                    if (toaDoX != lastMouseX || toaDoY != lastMouseY) {
                        lastMouseX = toaDoX;
                        lastMouseY = toaDoY;
                        SuKienNhap suKien = { SK_CHUOT_DI_CHUYEN, 0, toaDoX, toaDoY };
                        return suKien;
                    }
                }
            }
        }
    }



