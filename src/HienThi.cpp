#include "HienThi.h"
#include "mylib.h"
#include "LopTinChi.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;

namespace {
const int DO_RONG_STT = 3;
const int DO_RONG_MASV = MAX_MASV - 1;
const int DO_RONG_HO = MAX_HO - 1;
const int DO_RONG_TEN = MAX_TEN - 1;
const int DO_RONG_DIEM = 8;

void inKhungBangDiem() {
    cout << "+" << string(DO_RONG_STT + 2, '-')
         << "+" << string(DO_RONG_MASV + 2, '-')
         << "+" << string(DO_RONG_HO + 2, '-')
         << "+" << string(DO_RONG_TEN + 2, '-')
         << "+" << string(DO_RONG_DIEM + 2, '-')
         << "+\n";
}

void inTieuDeBangDiem(const char* tieuDeDiem) {
    inKhungBangDiem();
    cout << "| " << right << setw(DO_RONG_STT) << "STT"
         << " | " << left << setw(DO_RONG_MASV) << "MASV"
         << " | " << setw(DO_RONG_HO) << "HO"
         << " | " << setw(DO_RONG_TEN) << "TEN"
         << " | " << right << setw(DO_RONG_DIEM) << tieuDeDiem
         << " |\n";
    inKhungBangDiem();
}
}

float nhapDiemTrenBang(short x, short y) {
    float diem;

    while (true) {
        gotoxy(x, y);
        clreol();

        cin >> diem;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (diem < 0 || diem > 10) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return diem;
    }
}

void nhapDiemTrenBang(const DanhSachLop& dsLop, PTRDK dssvdk) {
    inTieuDeBangDiem("DIEM");

    int stt = 1;

    for (int i = 0; i < dsLop.soLuong; i++) {
        PTRSV sv = dsLop.lop[i].dssv;

        while (sv != nullptr) {
            PTRDK dk = timDangKyTheoMaSV(dssvdk, sv->sv.maSV);

            if (dk != nullptr) {
                cout << "| "
                     << setw(DO_RONG_STT) << right << stt << " | "
                     << setw(DO_RONG_MASV) << left << sv->sv.maSV << " | "
                     << setw(DO_RONG_HO) << left << sv->sv.ho << " | "
                     << setw(DO_RONG_TEN) << left << sv->sv.ten << " |";

                short xDiem = wherex();
                short yDiem = wherey();

                cout << setw(DO_RONG_DIEM) << " " << "|\n";

                dk->dk.diem = nhapDiemTrenBang(xDiem, yDiem);

                gotoxy(xDiem, yDiem);
                cout << setw(DO_RONG_DIEM) << right << dk->dk.diem << " |\n";

                inKhungBangDiem();

                stt++;
            }

            sv = sv->next;
        }
    }
}
void inBangDiemLopTinChi(const DanhSachLop& dsLop, const LopTinChi& lopTC) {
    cout << "\n";
    cout << "                 BANG DIEM MON HOC " << lopTC.maMH << endl;
    cout << "Nien khoa: " << lopTC.nienKhoa
         << "    Hoc ky: " << lopTC.hocKy
         << "    Nhom: " << lopTC.nhom << endl;

    inTieuDeBangDiem("DIEM");

    int stt = 1;

    for (int i = 0; i < dsLop.soLuong; i++) {
        PTRSV sv = dsLop.lop[i].dssv;

        while (sv != nullptr) {
            PTRDK dk = timDangKyTheoMaSV(lopTC.dssvdk, sv->sv.maSV);

            if (dk != nullptr) {
                cout << "| "
                     << setw(DO_RONG_STT) << right << stt << " | "
                     << setw(DO_RONG_MASV) << left << sv->sv.maSV << " | "
                     << setw(DO_RONG_HO) << left << sv->sv.ho << " | "
                     << setw(DO_RONG_TEN) << left << sv->sv.ten << " | "
                     << setw(DO_RONG_DIEM) << right << dk->dk.diem << " |\n";

                inKhungBangDiem();

                stt++;
            }

            sv = sv->next;
        }
    }
}
float tinhDiemTrungBinhSinhVien(
    PTRSV sv,
    DanhSachLopTinChi& dsLopTC,
    PTRMH root
) {
    float tongDiemTinChi = 0;
    int tongSoTinChi = 0;

    for (int i = 0; i < dsLopTC.soLuong; i++) {
        LopTinChi* lopTC = dsLopTC.lopTC[i];

        if (lopTC == nullptr || lopTC->huyLop)
            continue;

        PTRDK dk = timDangKyTheoMaSV(lopTC->dssvdk,sv->sv.maSV);

        if (dk == nullptr || dk->dk.huyDangKy || dk->dk.diem < 0)
            continue;

        PTRMH monHoc = timMonHocTheoMa(root, lopTC->maMH);

        if (monHoc == nullptr)
            continue;

        int soTinChi = monHoc->mh.soTCLT + monHoc->mh.soTCTH;

        tongDiemTinChi += dk->dk.diem * soTinChi;
        tongSoTinChi += soTinChi;
    }

    if (tongSoTinChi == 0)
        return 0;

    return tongDiemTinChi / tongSoTinChi;
}
void inBangDiemTrungBinh(DanhSachLop& dsLop, DanhSachLopTinChi& dsLopTC, PTRMH root, const char* maLop) {
    int viTri = timLopTheoMa(dsLop, maLop);

    if (viTri == -1) {
        cout << "Khong tim thay lop!\n";
        return;
    }

    Lop& lop = dsLop.lop[viTri];

    cout << "\n";
    cout << "                 BANG DIEM TRUNG BINH\n";
    cout << "Ma lop : " << lop.maLop << endl;
    cout << "Ten lop: " << lop.tenLop << endl;

    inTieuDeBangDiem("DIEM TB");

    int stt = 1;
    PTRSV sv = lop.dssv;

    while (sv != nullptr) {
        float diemTB = tinhDiemTrungBinhSinhVien(
            sv,
            dsLopTC,
            root
        );

        cout << "| "
             << setw(DO_RONG_STT) << right << stt << " | "
             << setw(DO_RONG_MASV) << left << sv->sv.maSV << " | "
             << setw(DO_RONG_HO) << left << sv->sv.ho << " | "
             << setw(DO_RONG_TEN) << left << sv->sv.ten << " | "
             << setw(DO_RONG_DIEM) << right << fixed << setprecision(2)
             << diemTB << " |\n";

        inKhungBangDiem();

        stt++;
        sv = sv->next;
    }
}
float timDiemThiLonNhat(
    const char* maSV,
    const char* maMH,
    DanhSachLopTinChi& dsLopTC
) {
    float diemLonNhat = -1;

    for (int i = 0; i < dsLopTC.soLuong; i++) {
        LopTinChi* lopTC = dsLopTC.lopTC[i];

        if (lopTC == nullptr || lopTC->huyLop)
            continue;

        if (strcmp(lopTC->maMH, maMH) != 0)
            continue;

        PTRDK dk = timDangKyTheoMaSV(
            lopTC->dssvdk,
            maSV
        );

        if (dk != nullptr && !dk->dk.huyDangKy && dk->dk.diem >= 0 && dk->dk.diem > diemLonNhat)
            diemLonNhat = dk->dk.diem;
    }

    return diemLonNhat;
}
int layDanhSachMonHocCuaLop(
    Lop& lop,
    DanhSachLopTinChi& dsLopTC,
    char dsMaMH[][MAX_MAMH]
) {
    int soLuongMon = 0;

    for (int i = 0; i < dsLopTC.soLuong; i++) {
        LopTinChi* lopTC = dsLopTC.lopTC[i];

        if (lopTC == nullptr || lopTC->huyLop)
            continue;

        bool coSinhVien = false;

        PTRSV sv = lop.dssv;

        while (sv != nullptr) {
            PTRDK dk = timDangKyTheoMaSV(
                lopTC->dssvdk,
                sv->sv.maSV
            );

            if (dk != nullptr && !dk->dk.huyDangKy) {
                coSinhVien = true;
                break;
            }

            sv = sv->next;
        }

        if (!coSinhVien)
            continue;

        bool daCo = false;

        for (int j = 0; j < soLuongMon; j++) {
            if (strcmp(dsMaMH[j], lopTC->maMH) == 0) {
                daCo = true;
                break;
            }
        }

        if (!daCo) {
            strcpy(dsMaMH[soLuongMon], lopTC->maMH);
            soLuongMon++;
        }
    }

    return soLuongMon;
}
void inBangDiemTongKet(
    Lop& lop,
    DanhSachLopTinChi& dsLopTC
) {
    char dsMaMH[MAX_LOP_TC][MAX_MAMH];

    int soLuongMon = layDanhSachMonHocCuaLop(
        lop,
        dsLopTC,
        dsMaMH
    );

    cout << "\n";
    cout << "                         BANG DIEM TONG KET\n";
    cout << "Lop: " << lop.maLop << endl;
    cout << "\n";

    const int doRongSTT = 3;
    const int doRongMaSV = MAX_MASV - 1;
    const int doRongHoTen = MAX_HO + MAX_TEN - 1;
    const int doRongMaMH = MAX_MAMH - 1;

    const auto inDuongKe = [&]() {
        cout << "+" << string(doRongSTT + 2, '-')
             << "+" << string(doRongMaSV + 2, '-')
             << "+" << string(doRongHoTen + 2, '-');

        for (int i = 0; i < soLuongMon; ++i) {
            cout << "+" << string(doRongMaMH + 2, '-');
        }
        cout << "+\n";
    };

    inDuongKe();
    cout << "| " << right << setw(doRongSTT) << "STT"
         << " | " << left << setw(doRongMaSV) << "MASV"
         << " | " << setw(doRongHoTen) << "HO TEN" << " |";

    for (int i = 0; i < soLuongMon; ++i) {
        cout << " " << setw(doRongMaMH) << left << dsMaMH[i] << " |";
    }
    cout << "\n";
    inDuongKe();

    int stt = 1;

    PTRSV sv = lop.dssv;

    while (sv != nullptr) {
        cout << "| "
             << setw(doRongSTT) << right << stt << " | "
             << setw(doRongMaSV) << left << sv->sv.maSV << " | ";

        char hoTen[82];
        strcpy(hoTen, sv->sv.ho);
        strcat(hoTen, " ");
        strcat(hoTen, sv->sv.ten);

        cout << setw(doRongHoTen) << left << hoTen << " |";

        for (int i = 0; i < soLuongMon; i++) {
            float diem = timDiemThiLonNhat(
                sv->sv.maSV,
                dsMaMH[i],
                dsLopTC
            );

            if (diem == -1) {
                cout << " " << setw(doRongMaMH) << " " << " |";
            }
            else {
                cout << " " << setw(doRongMaMH) << right
                     << fixed << setprecision(2)
                     << diem << " |";
            }
        }

        cout << "\n";

        inDuongKe();

        stt++;
        sv = sv->next;
    }
}
