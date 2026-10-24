#include "ui/GiaoDienConsole.h"
#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <cmath>

using namespace std;

#ifdef _WIN32
HANDLE GiaoDienConsole::hOut = nullptr;
HANDLE GiaoDienConsole::hIn = nullptr;
#else
struct termios GiaoDienConsole::origTermios{};
bool GiaoDienConsole::termInitialized = false;
int GiaoDienConsole::pendingByte = -1;
#endif
int GiaoDienConsole::lastMouseX = -1;
int GiaoDienConsole::lastMouseY = -1;

#ifndef _WIN32
void GiaoDienConsole::khoiPhucConsole() {
        if (termInitialized) {
            cout << "\033[?1003l\033[?1002l\033[?1000l\033[?1006l"
                 << "\033[?25h\033[0m" << flush;
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios);
            termInitialized = false;
        }
    }

void GiaoDienConsole::xuLyTinHieu(int) {
        khoiPhucConsole();
        cout << "\033[2J\033[H" << flush;
        _exit(0);
    }

bool GiaoDienConsole::docByteCoThoiHan(unsigned char& c, int thoiGianChoMs) {
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
#endif

void GiaoDienConsole::khoiTaoConsole(const char* tieuDe) {
#ifdef _WIN32
        hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hIn = GetStdHandle(STD_INPUT_HANDLE);

        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
        SetConsoleTitleA(tieuDe);

        batNhapChuot();
        hienConTro(false);
        xoaManHinh();
#else
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
#endif
    }

void GiaoDienConsole::batNhapChuot() {
#ifdef _WIN32
        DWORD cheDoMoi = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
        SetConsoleMode(hIn, cheDoMoi);
#else
        cout << "\033[?1000h\033[?1002h\033[?1003h\033[?1006h" << flush;
#endif
    }

void GiaoDienConsole::hienConTro(bool dangHien) {
#ifdef _WIN32
        CONSOLE_CURSOR_INFO thongTinConTro;
        GetConsoleCursorInfo(hOut, &thongTinConTro);
        if ((thongTinConTro.bVisible == TRUE) != dangHien) {
            thongTinConTro.bVisible = dangHien ? TRUE : FALSE;
            thongTinConTro.dwSize = 20;
            SetConsoleCursorInfo(hOut, &thongTinConTro);
        }
#else
        cout << (dangHien ? "\033[?25h" : "\033[?25l") << flush;
#endif
    }

void GiaoDienConsole::diDenToaDo(int x, int y) {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
#ifdef _WIN32
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(hOut, coord);
#else
        cout << "\033[" << (y + 1) << ";" << (x + 1) << "H";
#endif
    }

void GiaoDienConsole::datMau(int mauChu, int mauNen) {
#ifdef _WIN32
        SetConsoleTextAttribute(hOut, (WORD)((mauNen << 4) | (mauChu & 0x0F)));
#else
        static const int fgMap[16] = { 30, 34, 32, 36, 31, 35, 33, 37, 90, 94, 92, 96, 91, 95, 93, 97 };
        static const int bgMap[16] = { 40, 44, 42, 46, 41, 45, 43, 47, 100, 104, 102, 106, 101, 105, 103, 107 };
        cout << "\033[0;" << fgMap[mauChu & 0x0F] << ";" << bgMap[mauNen & 0x0F] << "m";
#endif
    }

void GiaoDienConsole::datLaiMau() {
        datMau(XAM_NHAT, DEN);
    }

void GiaoDienConsole::xoaManHinh(int mauNen) {
        hienConTro(false);
        datMau(XAM_NHAT, mauNen);
#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(hOut, &csbi)) {
            DWORD soLuongO = csbi.dwSize.X * csbi.dwSize.Y;
            DWORD count;
            COORD toaDoBanDau = { 0, 0 };
            FillConsoleOutputCharacterA(hOut, ' ', soLuongO, toaDoBanDau, &count);
            FillConsoleOutputAttribute(hOut, csbi.wAttributes, soLuongO, toaDoBanDau, &count);
            SetConsoleCursorPosition(hOut, toaDoBanDau);
        }
#else
        cout << "\033[2J\033[H" << flush;
#endif
    }

void GiaoDienConsole::toMauVung(int x, int y, int w, int h, int mauChu, int mauNen, char ch) {
        datMau(mauChu, mauNen);
        string dong(w, ch);
        for (int r = 0; r < h; ++r) {
            diDenToaDo(x, y + r);
            cout << dong;
        }
    }

int GiaoDienConsole::doDaiUtf8(const string& str) {
        int doDai = 0;
        for (size_t i = 0; i < str.size(); ++i) {
            if ((str[i] & 0xC0) != 0x80) doDai++;
        }
        return doDai;
    }

string GiaoDienConsole::catChuoiUtf8(const string& str, int soKyTuToiDa) {
        int soKyTu = 0;
        size_t i = 0;
        while (i < str.size() && soKyTu < soKyTuToiDa) {
            if ((str[i] & 0xC0) != 0x80) soKyTu++;
            i++;
            while (i < str.size() && (str[i] & 0xC0) == 0x80) i++;
        }
        return str.substr(0, i);
    }

string GiaoDienConsole::canPhai(const string& str, int chieuRong) {
        if (chieuRong <= 0) return "";
        int doDai = doDaiUtf8(str);
        if (doDai > chieuRong) {
            if (chieuRong > 2) return catChuoiUtf8(str, chieuRong - 2) + "..";
            return catChuoiUtf8(str, chieuRong);
        }
        return str + string(chieuRong - doDai, ' ');
    }

string GiaoDienConsole::canGiua(const string& str, int chieuRong) {
        if (chieuRong <= 0) return "";
        int doDai = doDaiUtf8(str);
        if (doDai >= chieuRong) return canPhai(str, chieuRong);
        int trai = (chieuRong - doDai) / 2;
        int phai = chieuRong - doDai - trai;
        return string(trai, ' ') + str + string(phai, ' ');
    }

void GiaoDienConsole::veKhung(int x, int y, int w, int h, int mauChu, int mauNen, const string& tieuDe) {
        datMau(mauChu, mauNen);
        diDenToaDo(x, y);
        cout << "+" << string(w - 2, '-') << "+";
        string giua(w - 2, ' ');
        for (int i = 1; i < h - 1; ++i) {
            diDenToaDo(x, y + i);
            cout << "|" << giua << "|";
        }
        diDenToaDo(x, y + h - 1);
        cout << "+" << string(w - 2, '-') << "+";

        if (!tieuDe.empty()) {
            string t = " " + tieuDe + " ";
            int tLen = doDaiUtf8(t);
            if (tLen > w - 4) {
                t = " " + catChuoiUtf8(tieuDe, w - 6) + " ";
                tLen = doDaiUtf8(t);
            }
            int tx = x + (w - tLen) / 2;
            if (tx > x) {
                diDenToaDo(tx, y);
                datMau(VANG, mauNen);
                cout << t;
            }
        }
    }

void GiaoDienConsole::veTieuDe(const string& tieuDePhu) {
        datMau(VANG, LAM);
        diDenToaDo(1, 0);
        cout << canGiua("QUẢN LÍ SINH VIÊN HỆ TÍN CHỈ", 114);
        if (!tieuDePhu.empty()) {
            datMau(TRANG, XANH_NGOC);
            diDenToaDo(1, 1);
            cout << canGiua(tieuDePhu, 114);
        }
        datLaiMau();
    }

void GiaoDienConsole::veThanhTrangThai(const string& goiY, const string& thongBao, bool coLoi) {
        diDenToaDo(1, 26);
        if (!thongBao.empty()) {
            datMau(TRANG, coLoi ? DO : XANH_LA);
            cout << canGiua(thongBao, 114);
        } else {
            datMau(XAM_NHAT, DEN);
            cout << string(114, ' ');
        }

        diDenToaDo(1, 27);
        datMau(DEN, XAM_NHAT);
        cout << canPhai(" " + goiY, 114);
        datLaiMau();
    }

void GiaoDienConsole::veNut(const NutChuNhat& btn, bool dangHoatDong, int mauChuThuong, int mauNenThuong, int mauChuChon, int mauNenChon) {
        datMau(dangHoatDong ? mauChuChon : mauChuThuong, dangHoatDong ? mauNenChon : mauNenThuong);
        for (int r = 0; r < btn.h; ++r) {
            diDenToaDo(btn.x, btn.y + r);
            if (r == btn.h / 2) {
                cout << canGiua(btn.label, btn.w);
            } else {
                cout << string(btn.w, ' ');
            }
        }
        datLaiMau();
    }

SuKienNhap GiaoDienConsole::docSuKien() {
#ifdef _WIN32
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
                    case VK_TAB:    suKien.type = SK_PHIM_TAB; return suKien;
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
#else
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
#endif
    }

bool GiaoDienConsole::hopXacNhan(const string& tieuDe, const string& cauHoi, const string& chiTiet) {
        hienConTro(false);
        int w = 68, h = 9;
        int x = (CHIEU_RONG_GIAO_DIEN - w) / 2;
        int y = (CHIEU_CAO_GIAO_DIEN - h) / 2;

        veKhung(x, y, w, h, VANG, LAM, tieuDe);
        datMau(TRANG, LAM);
        diDenToaDo(x + 2, y + 2);
        cout << canGiua(cauHoi, w - 4);
        if (!chiTiet.empty()) {
            datMau(XANH_NGOC_NHAT, LAM);
            diDenToaDo(x + 2, y + 4);
            cout << canGiua(chiTiet, w - 4);
        }

        int soNut = 2;
        NutChuNhat* cacNut = new NutChuNhat[soNut];
        cacNut[0] = { x + 13, y + 6, 16, 1, 1, "[ ĐỒNG Ý ]" };
        cacNut[1] = { x + 39, y + 6, 16, 1, 0, "[ HỦY BỎ ]" };

        int mucDangChon = 0;
        int mucChonTruoc = -1;
        bool ketQua = false;
        bool dangChay = true;

        while (dangChay) {
            if (mucDangChon != mucChonTruoc) {
                for (int i = 0; i < soNut; ++i) {
                    veNut(cacNut[i], i == mucDangChon, TRANG, XAM_DAM, DEN, VANG);
                }
                mucChonTruoc = mucDangChon;
            }

            SuKienNhap suKien = docSuKien();
            if (suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI || suKien.type == SK_PHIM_TAB) {
                mucDangChon = 1 - mucDangChon;
            } else if (suKien.type == SK_PHIM_ENTER) {
                ketQua = (mucDangChon == 0);
                dangChay = false;
            } else if (suKien.type == SK_PHIM_ESC) {
                ketQua = false;
                dangChay = false;
            } else if (suKien.type == SK_CHUOT_DI_CHUYEN) {
                for (int i = 0; i < soNut; ++i) {
                    if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                        mucDangChon = i;
                    }
                }
            } else if (suKien.type == SK_CHUOT_TRAI) {
                for (int i = 0; i < soNut; ++i) {
                    if (cacNut[i].contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                        mucDangChon = i;
                        ketQua = (mucDangChon == 0);
                        dangChay = false;
                        break;
                    }
                }
            }
        }

        delete[] cacNut;
        return ketQua;
    }

void GiaoDienConsole::hopThongBao(const string& tieuDe, const string& msg, bool coLoi) {
        hienConTro(false);
        int w = 68, h = 8;
        int x = (CHIEU_RONG_GIAO_DIEN - w) / 2;
        int y = (CHIEU_CAO_GIAO_DIEN - h) / 2;
        int mauNen = coLoi ? DO : LAM;

        veKhung(x, y, w, h, VANG, mauNen, tieuDe);
        datMau(TRANG, mauNen);
        diDenToaDo(x + 2, y + 2);
        cout << canGiua(msg, w - 4);

        NutChuNhat okBtn = { x + (w - 18) / 2, y + 5, 18, 1, 1, "[ ĐÓNG (Enter) ]" };
        veNut(okBtn, true, TRANG, XAM_DAM, DEN, VANG);

        while (true) {
            SuKienNhap suKien = docSuKien();
            if (suKien.type == SK_PHIM_ENTER || suKien.type == SK_PHIM_ESC) break;
            if (suKien.type == SK_CHUOT_TRAI && okBtn.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) break;
        }
    }

string GiaoDienConsole::catKhoangTrang(const string& s) {
        size_t start = 0;
        while (start < s.size() && s[start] == ' ') start++;
        if (start == s.size()) return "";
        size_t end = s.size() - 1;
        while (end > start && s[end] == ' ') end--;
        string res = "";
        bool khoangTrangTruoc = false;
        for (size_t i = start; i <= end; ++i) {
            if (s[i] == ' ') {
                if (!khoangTrangTruoc) {
                    res += ' ';
                    khoangTrangTruoc = true;
                }
            } else {
                res += s[i];
                khoangTrangTruoc = false;
            }
        }
        return res;
    }

string GiaoDienConsole::vietHoa(const string& s) {
        string res = catKhoangTrang(s);
        for (size_t i = 0; i < res.size(); ++i) {
            if (res[i] >= 'a' && res[i] <= 'z') {
                res[i] = res[i] - 'a' + 'A';
            }
        }
        return res;
    }

string GiaoDienConsole::vietHoaDauTu(const string& s) {
        string res = catKhoangTrang(s);
        bool dauTuMoi = true;
        for (size_t i = 0; i < res.size(); ++i) {
            if (res[i] == ' ') {
                dauTuMoi = true;
            } else if (dauTuMoi) {
                if (res[i] >= 'a' && res[i] <= 'z') res[i] = res[i] - 'a' + 'A';
                dauTuMoi = false;
            }
        }
        return res;
    }

bool chayBieuMau(const string& tieuDe, TruongNhap* fields, int soTruong, string& thongBaoLoi, const string& nhanNutLuu, const string& nhanNutHuy) {
    GiaoDienConsole::hienConTro(false);
    int chieuRongBieuMau = 76;
    int chieuCaoBieuMau = soTruong * 2 + 7;
    int toaDoXBieuMau = (CHIEU_RONG_GIAO_DIEN - chieuRongBieuMau) / 2;
    int toaDoYBieuMau = (CHIEU_CAO_GIAO_DIEN - chieuCaoBieuMau) / 2;
    if (toaDoYBieuMau < 3) toaDoYBieuMau = 3;

    GiaoDienConsole::veKhung(toaDoXBieuMau, toaDoYBieuMau, chieuRongBieuMau, chieuCaoBieuMau, XANH_NGOC_NHAT, DEN, tieuDe);

    for (int i = 0; i < soTruong; ++i) {
        fields[i].x = toaDoXBieuMau + 27;
        fields[i].y = toaDoYBieuMau + 2 + i * 2;
        fields[i].w = chieuRongBieuMau - 30;
        GiaoDienConsole::datMau(TRANG, DEN);
        GiaoDienConsole::diDenToaDo(toaDoXBieuMau + 3, fields[i].y);
        cout << GiaoDienConsole::canPhai(fields[i].label + ":", 23);
    }

    NutChuNhat nutLuu = { toaDoXBieuMau + 16, toaDoYBieuMau + chieuCaoBieuMau - 2, 18, 1, 1, nhanNutLuu };
    NutChuNhat nutHuy = { toaDoXBieuMau + 42, toaDoYBieuMau + chieuCaoBieuMau - 2, 18, 1, 0, nhanNutHuy };

    auto veMotTruong = [&](int i, bool dangHoatDong) {
        if (i < 0 || i >= soTruong) return;
        if (fields[i].mode == TRUONG_CHI_DOC) {
            GiaoDienConsole::datMau(VANG, XAM_DAM);
        } else if (dangHoatDong) {
            GiaoDienConsole::datMau(TRANG, LAM);
        } else {
            GiaoDienConsole::datMau(XAM_NHAT, XAM_DAM);
        }
        GiaoDienConsole::diDenToaDo(fields[i].x, fields[i].y);
        cout << " " << GiaoDienConsole::canPhai(fields[i].value, fields[i].w - 2) << " ";
    };

    auto veDongThongBao = [&]() {
        GiaoDienConsole::diDenToaDo(toaDoXBieuMau + 2, toaDoYBieuMau + chieuCaoBieuMau - 4);
        if (!thongBaoLoi.empty()) {
            GiaoDienConsole::datMau(DO_NHAT, DEN);
            cout << GiaoDienConsole::canGiua(thongBaoLoi, chieuRongBieuMau - 4);
        } else {
            GiaoDienConsole::datMau(XAM_DAM, DEN);
            cout << GiaoDienConsole::canGiua("Click chuột vào ô hoặc dùng phím Lên/Xuống/Tab để di chuyển", chieuRongBieuMau - 4);
        }
    };

    int viTriTruongDangChon = 0;
    while (viTriTruongDangChon < soTruong && fields[viTriTruongDangChon].mode == TRUONG_CHI_DOC) {
        viTriTruongDangChon++;
    }
    if (viTriTruongDangChon >= soTruong) viTriTruongDangChon = 0;

    // Vẽ toàn bộ các ô và nút lần đầu tiên
    for (int i = 0; i < soTruong; ++i) {
        veMotTruong(i, i == viTriTruongDangChon);
    }
    GiaoDienConsole::veNut(nutLuu, viTriTruongDangChon == soTruong, TRANG, XANH_LA, DEN, VANG);
    GiaoDienConsole::veNut(nutHuy, viTriTruongDangChon == soTruong + 1, TRANG, DO, DEN, VANG);
    veDongThongBao();

    int viTriTruongTruoc = viTriTruongDangChon;
    string thongBaoTruoc = thongBaoLoi;
    bool giaTriDaSua = false;

    while (true) {
        // Chỉ cập nhật đúng phần thay đổi, tuyệt đối không vẽ lại cả Form
        if (thongBaoLoi != thongBaoTruoc) {
            veDongThongBao();
            thongBaoTruoc = thongBaoLoi;
        }
        if (viTriTruongDangChon != viTriTruongTruoc) {
            GiaoDienConsole::hienConTro(false);
            if (viTriTruongTruoc < soTruong) veMotTruong(viTriTruongTruoc, false);
            else if (viTriTruongTruoc == soTruong) GiaoDienConsole::veNut(nutLuu, false, TRANG, XANH_LA, DEN, VANG);
            else if (viTriTruongTruoc == soTruong + 1) GiaoDienConsole::veNut(nutHuy, false, TRANG, DO, DEN, VANG);

            if (viTriTruongDangChon < soTruong) veMotTruong(viTriTruongDangChon, true);
            else if (viTriTruongDangChon == soTruong) GiaoDienConsole::veNut(nutLuu, true, TRANG, XANH_LA, DEN, VANG);
            else if (viTriTruongDangChon == soTruong + 1) GiaoDienConsole::veNut(nutHuy, true, TRANG, DO, DEN, VANG);

            viTriTruongTruoc = viTriTruongDangChon;
            giaTriDaSua = false;
        } else if (giaTriDaSua && viTriTruongDangChon < soTruong) {
            GiaoDienConsole::hienConTro(false);
            veMotTruong(viTriTruongDangChon, true);
            giaTriDaSua = false;
        }

        // Đặt con trỏ nhấp nháy tại vị trí đang gõ
        if (viTriTruongDangChon >= 0 && viTriTruongDangChon < soTruong && fields[viTriTruongDangChon].mode != TRUONG_CHI_DOC) {
            int doDai = GiaoDienConsole::doDaiUtf8(fields[viTriTruongDangChon].value);
            if (doDai > fields[viTriTruongDangChon].w - 2) doDai = fields[viTriTruongDangChon].w - 2;
            GiaoDienConsole::diDenToaDo(fields[viTriTruongDangChon].x + 1 + doDai, fields[viTriTruongDangChon].y);
            GiaoDienConsole::hienConTro(true);
        } else {
            GiaoDienConsole::hienConTro(false);
        }

        SuKienNhap suKien = GiaoDienConsole::docSuKien();
        if (suKien.type == SK_PHIM_ESC) {
            GiaoDienConsole::hienConTro(false);
            return false;
        } else if (suKien.type == SK_PHIM_LEN) {
            int viTriTiepTheo = viTriTruongDangChon;
            do {
                viTriTiepTheo = (viTriTiepTheo - 1 + soTruong + 2) % (soTruong + 2);
            } while (viTriTiepTheo < soTruong && fields[viTriTiepTheo].mode == TRUONG_CHI_DOC);
            viTriTruongDangChon = viTriTiepTheo;
        } else if (suKien.type == SK_PHIM_XUONG || suKien.type == SK_PHIM_TAB) {
            int viTriTiepTheo = viTriTruongDangChon;
            do {
                viTriTiepTheo = (viTriTiepTheo + 1) % (soTruong + 2);
            } while (viTriTiepTheo < soTruong && fields[viTriTiepTheo].mode == TRUONG_CHI_DOC);
            viTriTruongDangChon = viTriTiepTheo;
        } else if ((suKien.type == SK_PHIM_TRAI || suKien.type == SK_PHIM_PHAI) && viTriTruongDangChon >= soTruong) {
            viTriTruongDangChon = (viTriTruongDangChon == soTruong) ? soTruong + 1 : soTruong;
        } else if (suKien.type == EVT_KEY_F2) {
            GiaoDienConsole::hienConTro(false);
            return true;
        } else if (suKien.type == SK_PHIM_ENTER) {
            if (viTriTruongDangChon == soTruong + 1) {
                GiaoDienConsole::hienConTro(false);
                return false;
            } else if (viTriTruongDangChon == soTruong) {
                GiaoDienConsole::hienConTro(false);
                return true;
            } else {
                int nextIdx = viTriTruongDangChon + 1;
                while (nextIdx < soTruong && fields[nextIdx].mode == TRUONG_CHI_DOC) nextIdx++;
                if (nextIdx < soTruong) {
                    viTriTruongDangChon = nextIdx;
                } else {
                    GiaoDienConsole::hienConTro(false);
                    return true;
                }
            }
        } else if (suKien.type == EVT_KEY_BACKSPACE) {
            if (viTriTruongDangChon >= 0 && viTriTruongDangChon < soTruong && fields[viTriTruongDangChon].mode != TRUONG_CHI_DOC) {
                if (!fields[viTriTruongDangChon].value.empty()) {
                    fields[viTriTruongDangChon].value.pop_back();
                    giaTriDaSua = true;
                    thongBaoLoi = "";
                }
            }
        } else if (suKien.type == SK_KY_TU) {
            if (viTriTruongDangChon >= 0 && viTriTruongDangChon < soTruong && fields[viTriTruongDangChon].mode != TRUONG_CHI_DOC) {
                TruongNhap& f = fields[viTriTruongDangChon];
                if ((int)f.value.size() < f.maxLen) {
                    char c = suKien.ch;
                    if (f.mode == TRUONG_MA) {
                        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_') {
                            if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
                            f.value += c;
                            giaTriDaSua = true;
                            thongBaoLoi = "";
                        }
                    } else if (f.mode == TRUONG_SO || f.mode == TRUONG_DIEN_THOAI) {
                        if (c >= '0' && c <= '9') {
                            f.value += c;
                            giaTriDaSua = true;
                            thongBaoLoi = "";
                        }
                    } else if (f.mode == TRUONG_SO_THUC) {
                        if ((c >= '0' && c <= '9') || (c == '.' && f.value.find('.') == string::npos)) {
                            f.value += c;
                            giaTriDaSua = true;
                            thongBaoLoi = "";
                        }
                    } else if (f.mode == TRUONG_CHU) {
                        if (c >= 32 && c <= 126) {
                            f.value += c;
                            giaTriDaSua = true;
                            thongBaoLoi = "";
                        }
                    }
                }
            }
        } else if (suKien.type == SK_CHUOT_TRAI) {
            for (int i = 0; i < soTruong; ++i) {
                if (fields[i].mode != TRUONG_CHI_DOC &&
                    suKien.toaDoChuotX >= fields[i].x && suKien.toaDoChuotX < fields[i].x + fields[i].w &&
                    suKien.toaDoChuotY == fields[i].y) {
                    viTriTruongDangChon = i;
                    break;
                }
            }
            if (nutLuu.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                GiaoDienConsole::hienConTro(false);
                return true;
            }
            if (nutHuy.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) {
                GiaoDienConsole::hienConTro(false);
                return false;
            }
        } else if (suKien.type == SK_CHUOT_DI_CHUYEN) {
            if (nutLuu.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) viTriTruongDangChon = soTruong;
            else if (nutHuy.contains(suKien.toaDoChuotX, suKien.toaDoChuotY)) viTriTruongDangChon = soTruong + 1;
        }
    }
}
