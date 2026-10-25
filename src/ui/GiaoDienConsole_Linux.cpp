#include "ui/GiaoDienConsole.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <csignal>
#include <cstdlib>
#include <strings.h>

using namespace std;

namespace {
struct termios origTermios{};
bool termInitialized = false;
int pendingByte = -1;
int lastMouseX = -1;
int lastMouseY = -1;

void khoiPhucConsole() {
        if (termInitialized) {
            cout << "\033[?1003l\033[?1002l\033[?1000l\033[?1006l"
                 << "\033[?25h\033[0m" << flush;
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios);
            termInitialized = false;
        }
    }

void xuLyTinHieu(int) {
        khoiPhucConsole();
        cout << "\033[2J\033[H" << flush;
        _exit(0);
    }

bool docByteCoThoiHan(unsigned char& c, int thoiGianChoMs) {
        if (pendingByte != -1) {
            c = (unsigned char)pendingByte;
            pendingByte = -1;
            return true;
        }
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        struct timeval tv;
        tv.tv_sec = thoiGianChoMs / 1000;
        tv.tv_usec = (thoiGianChoMs % 1000) * 1000;
        int ketQua = select(STDIN_FILENO + 1, &fds, nullptr, nullptr, &tv);
        if (ketQua > 0 && FD_ISSET(STDIN_FILENO, &fds)) {
            return (read(STDIN_FILENO, &c, 1) == 1);
        }
        return false;
    }
} // namespace

void GiaoDienConsole::khoiTaoConsole(const char* tieuDe) {
        if (!termInitialized) {
            if (tcgetattr(STDIN_FILENO, &origTermios) == 0) {
                termInitialized = true;
                atexit(khoiPhucConsole);
                signal(SIGINT, xuLyTinHieu);
                signal(SIGTERM, xuLyTinHieu);

                struct termios raw = origTermios;
                raw.c_lflag &= ~(ICANON | ECHO);
                raw.c_iflag &= ~(IXON | ICRNL);
                raw.c_cc[VMIN] = 1;
                raw.c_cc[VTIME] = 0;
                tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
            }
        }
        // Đặt tiêu đề cửa sổ terminal và gợi ý kích thước 120x30 nếu terminal hỗ trợ
        cout << "\033]0;" << tieuDe << "\007";
        struct winsize ws;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
            if (ws.ws_col < 120 || ws.ws_row < 30) {
                cout << "\033[8;30;120t";
            }
        }
        batNhapChuot();
        hienConTro(false);
        xoaManHinh();
    }

void GiaoDienConsole::batNhapChuot() {
        cout << "\033[?1000h\033[?1002h\033[?1003h\033[?1006h" << flush;
    }

void GiaoDienConsole::hienConTro(bool dangHien) {
        cout << (dangHien ? "\033[?25h" : "\033[?25l") << flush;
    }

void GiaoDienConsole::diDenToaDo(int x, int y) {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
    }

void GiaoDienConsole::datMau(int mauChu, int mauNen) {
        static const int fgMap[16] = { 30, 34, 32, 36, 31, 35, 33, 37, 90, 94, 92, 96, 91, 95, 93, 97 };
        static const int bgMap[16] = { 40, 44, 42, 46, 41, 45, 43, 47, 100, 104, 102, 106, 101, 105, 103, 107 };
        cout << "\033[0;" << fgMap[mauChu & 0x0F] << ";" << bgMap[mauNen & 0x0F] << "m";
    }

void GiaoDienConsole::xoaManHinh(int mauNen) {
        hienConTro(false);
        datMau(XAM_NHAT, mauNen);
        cout << "\033[2J\033[H" << flush;
    }

SuKienNhap GiaoDienConsole::docSuKien() {
        cout.flush();
        while (true) {
            unsigned char ch = 0;
            if (pendingByte != -1) {
                ch = (unsigned char)pendingByte;
                pendingByte = -1;
            } else {
                ssize_t n = read(STDIN_FILENO, &ch, 1);
                if (n == 0) {
                    khoiPhucConsole();
                    exit(0);
                }
                if (n < 0) {
                    continue;
                }
            }
            SuKienNhap suKien = { SK_KHONG, 0, lastMouseX, lastMouseY };

            if (ch == 27) { // ESC hoặc chuỗi thoát ANSI
                unsigned char byteTiepTheo1 = 0;
                if (!docByteCoThoiHan(byteTiepTheo1, 25)) {
                    suKien.type = SK_PHIM_ESC;
                    return suKien;
                }
                if (byteTiepTheo1 == '[') {
                    unsigned char byteTiepTheo2 = 0;
                    if (!docByteCoThoiHan(byteTiepTheo2, 25)) {
                        suKien.type = SK_PHIM_ESC;
                        return suKien;
                    }
                    if (byteTiepTheo2 == '<') {
                        // Chuột SGR: \033[<b;x;yM hoặc \033[<b;x;ym
                        string buf = "";
                        unsigned char c = 0;
                        char kyTuCuoi = 0;
                        while (docByteCoThoiHan(c, 25)) {
                            if (c == 'M' || c == 'm') {
                                kyTuCuoi = (char)c;
                                break;
                            }
                            buf += (char)c;
                            if (buf.size() > 32) break;
                        }
                        if (kyTuCuoi == 'M') {
                            int b = 0, x = 0, y = 0;
                            char dauPhanCach1 = 0, dauPhanCach2 = 0;
                            stringstream ss(buf);
                            if (ss >> b >> dauPhanCach1 >> x >> dauPhanCach2 >> y) {
                                int toaDoX = (x > 0) ? (x - 1) : 0;
                                int toaDoY = (y > 0) ? (y - 1) : 0;
                                if (b & 64) {
                                    suKien.toaDoChuotX = toaDoX;
                                    suKien.toaDoChuotY = toaDoY;
                                    suKien.type = ((b & 3) == 0) ? SK_CON_LAN_LEN : SK_CON_LAN_XUONG;
                                    return suKien;
                                } else if (b & 32) {
                                    if (toaDoX != lastMouseX || toaDoY != lastMouseY) {
                                        lastMouseX = toaDoX;
                                        lastMouseY = toaDoY;
                                        suKien.type = SK_CHUOT_DI_CHUYEN;
                                        suKien.toaDoChuotX = toaDoX;
                                        suKien.toaDoChuotY = toaDoY;
                                        return suKien;
                                    }
                                } else {
                                    lastMouseX = toaDoX;
                                    lastMouseY = toaDoY;
                                    suKien.toaDoChuotX = toaDoX;
                                    suKien.toaDoChuotY = toaDoY;
                                    if ((b & 3) == 0) {
                                        suKien.type = SK_CHUOT_TRAI;
                                        return suKien;
                                    } else if ((b & 3) == 2) {
                                        suKien.type = SK_CHUOT_PHAI;
                                        return suKien;
                                    }
                                }
                            }
                        }
                        continue;
                    } else if (byteTiepTheo2 == '[') {
                        // Linux console F1..F5: \033[[A .. \033[[E
                        unsigned char byteTiepTheo3 = 0;
                        if (docByteCoThoiHan(byteTiepTheo3, 25)) {
                            if (byteTiepTheo3 == 'A') { suKien.type = EVT_KEY_F1; return suKien; }
                            if (byteTiepTheo3 == 'B') { suKien.type = EVT_KEY_F2; return suKien; }
                            if (byteTiepTheo3 == 'C') { suKien.type = EVT_KEY_F3; return suKien; }
                            if (byteTiepTheo3 == 'D') { suKien.type = EVT_KEY_F4; return suKien; }
                            if (byteTiepTheo3 == 'E') { suKien.type = EVT_KEY_F5; return suKien; }
                        }
                        continue;
                    } else {
                        string seq = "";
                        char byteCuoi = 0;
                        if (byteTiepTheo2 >= 0x40 && byteTiepTheo2 <= 0x7E) {
                            byteCuoi = (char)byteTiepTheo2;
                        } else {
                            seq += (char)byteTiepTheo2;
                            unsigned char c = 0;
                            while (docByteCoThoiHan(c, 25)) {
                                if (c >= 0x40 && c <= 0x7E) {
                                    byteCuoi = (char)c;
                                    break;
                                }
                                seq += (char)c;
                                if (seq.size() > 16) break;
                            }
                        }
                        if (byteCuoi == 'A') { suKien.type = SK_PHIM_LEN; return suKien; }
                        if (byteCuoi == 'B') { suKien.type = SK_PHIM_XUONG; return suKien; }
                        if (byteCuoi == 'C') { suKien.type = SK_PHIM_PHAI; return suKien; }
                        if (byteCuoi == 'D') { suKien.type = SK_PHIM_TRAI; return suKien; }
                        if (byteCuoi == 'H') { suKien.type = EVT_KEY_HOME; return suKien; }
                        if (byteCuoi == 'F') { suKien.type = EVT_KEY_END; return suKien; }
                        if (byteCuoi == 'P') { suKien.type = EVT_KEY_F1; return suKien; }
                        if (byteCuoi == 'Q') { suKien.type = EVT_KEY_F2; return suKien; }
                        if (byteCuoi == 'R') { suKien.type = EVT_KEY_F3; return suKien; }
                        if (byteCuoi == 'S') { suKien.type = EVT_KEY_F4; return suKien; }
                        if (byteCuoi == '~') {
                            int code = atoi(seq.c_str());
                            switch (code) {
                                case 1:
                                case 7:  suKien.type = EVT_KEY_HOME; return suKien;
                                case 2:  suKien.type = EVT_KEY_INSERT; return suKien;
                                case 3:  suKien.type = EVT_KEY_DELETE; return suKien;
                                case 4:
                                case 8:  suKien.type = EVT_KEY_END; return suKien;
                                case 5:  suKien.type = EVT_KEY_PGUP; return suKien;
                                case 6:  suKien.type = EVT_KEY_PGDN; return suKien;
                                case 11: suKien.type = EVT_KEY_F1; return suKien;
                                case 12: suKien.type = EVT_KEY_F2; return suKien;
                                case 13: suKien.type = EVT_KEY_F3; return suKien;
                                case 14: suKien.type = EVT_KEY_F4; return suKien;
                                case 15: suKien.type = EVT_KEY_F5; return suKien;
                                default: break;
                            }
                        }
                        continue;
                    }
                } else if (byteTiepTheo1 == 'O') {
                    unsigned char byteTiepTheo2 = 0;
                    if (docByteCoThoiHan(byteTiepTheo2, 25)) {
                        switch (byteTiepTheo2) {
                            case 'A': suKien.type = SK_PHIM_LEN; return suKien;
                            case 'B': suKien.type = SK_PHIM_XUONG; return suKien;
                            case 'C': suKien.type = SK_PHIM_PHAI; return suKien;
                            case 'D': suKien.type = SK_PHIM_TRAI; return suKien;
                            case 'H': suKien.type = EVT_KEY_HOME; return suKien;
                            case 'F': suKien.type = EVT_KEY_END; return suKien;
                            case 'P': suKien.type = EVT_KEY_F1; return suKien;
                            case 'Q': suKien.type = EVT_KEY_F2; return suKien;
                            case 'R': suKien.type = EVT_KEY_F3; return suKien;
                            case 'S': suKien.type = EVT_KEY_F4; return suKien;
                            default: break;
                        }
                    }
                    continue;
                } else {
                    pendingByte = byteTiepTheo1;
                    suKien.type = SK_PHIM_ESC;
                    return suKien;
                }
            } else if (ch == '\r' || ch == '\n') {
                suKien.type = SK_PHIM_ENTER;
                return suKien;
            } else if (ch == '\t') {
                suKien.type = SK_PHIM_TAB;
                return suKien;
            } else if (ch == 127 || ch == 8) {
                suKien.type = EVT_KEY_BACKSPACE;
                return suKien;
            } else if (ch >= 32 && ch <= 126) {
                suKien.type = SK_KY_TU;
                suKien.ch = (char)ch;
                return suKien;
            }
        }
    }

