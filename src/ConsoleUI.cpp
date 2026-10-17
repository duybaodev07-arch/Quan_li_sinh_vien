#include "ConsoleUI.h"
#include "mylib.h"

#include <algorithm>
#include <conio.h>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

namespace {
const int SO_DONG_MOI_TRANG = 14;

void inDongKe(int doRong) {
    cout << "+" << string(doRong, '-') << "+\n";
}

void inNoiDungCanGiua(const string& noiDung, int doRong) {
    const int khoangTrong = max(0, doRong - static_cast<int>(noiDung.length()));
    const int benTrai = khoangTrong / 2;
    const int benPhai = khoangTrong - benTrai;

    cout << "|" << string(benTrai, ' ')
         << noiDung
         << string(benPhai, ' ') << "|\n";
}

int tinhDoRongMenu(
    const char* tieuDe,
    const char* const cacMuc[],
    int soLuong,
    const char* huongDan
) {
    int doRong = static_cast<int>(strlen(tieuDe));
    doRong = max(doRong, static_cast<int>(strlen(huongDan)));

    for (int i = 0; i < soLuong; ++i) {
        const int doRongMuc = static_cast<int>(strlen(cacMuc[i])) + 12;
        doRong = max(doRong, doRongMuc);
    }

    return doRong + 4;
}

void veMenu(
    const char* tieuDe,
    const char* const cacMuc[],
    int soLuong,
    const char* huongDan,
    int dangChon,
    int doRong
) {
    clrscr();
    inDongKe(doRong);

    SetColor(14);
    inNoiDungCanGiua(tieuDe, doRong);
    SetColor(7);

    inDongKe(doRong);
    inNoiDungCanGiua(huongDan, doRong);

    const int batDau = (dangChon / SO_DONG_MOI_TRANG) * SO_DONG_MOI_TRANG;
    const int ketThuc = min(soLuong, batDau + SO_DONG_MOI_TRANG);
    const int soTrang = (soLuong + SO_DONG_MOI_TRANG - 1) / SO_DONG_MOI_TRANG;
    const int trangHienTai = batDau / SO_DONG_MOI_TRANG + 1;
    const string thongTinTrang =
        "Trang " + to_string(trangHienTai) + "/" + to_string(soTrang);

    inNoiDungCanGiua(thongTinTrang, doRong);
    inDongKe(doRong);

    for (int i = batDau; i < ketThuc; ++i) {
        const string soThuTu = i + 1 < 10
            ? "0" + to_string(i + 1)
            : to_string(i + 1);
        const string noiDung =
            string(i == dangChon ? ">> [" : "   [") +
            soThuTu + "] " + cacMuc[i];
        const int khoangTrong = max(
            0,
            doRong - static_cast<int>(noiDung.length())
        );

        cout << "|";
        if (i == dangChon) {
            SetColor(15);
            SetBGColor(1);
        }
        cout << noiDung << string(khoangTrong, ' ');
        SetBGColor(0);
        SetColor(7);
        cout << "|\n";
    }

    inDongKe(doRong);
}

int dichChuyen(int dangChon, int buoc, int soLuong) {
    int moi = (dangChon + buoc) % soLuong;
    if (moi < 0) {
        moi += soLuong;
    }
    return moi;
}
}

int chonMenuConsole(
    const char* tieuDe,
    const char* const cacMuc[],
    int soLuong,
    const char* huongDan
) {
    if (tieuDe == nullptr || cacMuc == nullptr || huongDan == nullptr || soLuong <= 0) {
        return -1;
    }

    int dangChon = 0;
    const int doRong = tinhDoRongMenu(
        tieuDe,
        cacMuc,
        soLuong,
        huongDan
    );

    while (true) {
        veMenu(
            tieuDe,
            cacMuc,
            soLuong,
            huongDan,
            dangChon,
            doRong
        );

        const int phim = _getch();
        if (phim == 0 || phim == 224) {
            const int phimMuiTen = _getch();
            const char* phimTat = nullptr;

            if (phim == 0 && phimMuiTen == 59) phimTat = "[F1]";
            else if (phim == 0 && phimMuiTen == 60) phimTat = "[F2]";
            else if (phim == 0 && phimMuiTen == 61) phimTat = "[F3]";
            else if (phim == 224 && phimMuiTen == 83) phimTat = "[DEL]";

            if (phimTat != nullptr) {
                for (int i = 0; i < soLuong; ++i) {
                    if (strncmp(cacMuc[i], phimTat, strlen(phimTat)) == 0) {
                        return i;
                    }
                }
            }

            if (phimMuiTen == 72) {
                dangChon = dichChuyen(dangChon, -1, soLuong);
            }
            else if (phimMuiTen == 80) {
                dangChon = dichChuyen(dangChon, 1, soLuong);
            }
            else if ((phimMuiTen == 75 || phimMuiTen == 73) &&
                     soLuong > SO_DONG_MOI_TRANG) {
                dangChon = max(0, dangChon - SO_DONG_MOI_TRANG);
            }
            else if ((phimMuiTen == 77 || phimMuiTen == 81) &&
                     soLuong > SO_DONG_MOI_TRANG) {
                dangChon = min(
                    soLuong - 1,
                    dangChon + SO_DONG_MOI_TRANG
                );
            }
            else if (phimMuiTen == 71) {
                dangChon = 0;
            }
            else if (phimMuiTen == 79) {
                dangChon = soLuong - 1;
            }
        }
        else if (phim == 13) {
            return dangChon;
        }
        else if (phim == 27) {
            return -1;
        }
    }
}

void choNhanPhim() {
    cout << "\nNhan phim bat ky de tiep tuc...";
    _getch();
}


