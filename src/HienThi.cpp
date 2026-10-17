#include "HienThi.h"
#include "mylib.h"
#include "Input.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
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

static float nhapDiemTaiO(short x, short y, float diemHienTai) {
    gotoxy(x, y);
    clreol();
    if (diemHienTai >= 0.0f) {
        cout << "Diem hien tai " << fixed << setprecision(2) << diemHienTai
             << ". Nhap diem moi: ";
    } else {
        cout << "Nhap diem: ";
    }
    return nhapDiem(0.0f, 10.0f);
}

void nhapDiemDanhSachDangKy(const DanhSachLop& dsLop, PTRDK dssvdk) {
    inTieuDeBangDiem("DIEM");
    int stt = 1;

    for (int i = 0; i < dsLop.soLuong; ++i) {
        PTRSV sv = dsLop.lop[i].dssv;
        while (sv != nullptr) {
            PTRDK dk = timDangKyTheoMaSV(dssvdk, sv->sv.maSV);
            if (dk != nullptr && !dk->dk.huyDangKy) {
                cout << "| " << setw(DO_RONG_STT) << right << stt << " | "
                     << setw(DO_RONG_MASV) << left << sv->sv.maSV << " | "
                     << setw(DO_RONG_HO) << left << sv->sv.ho << " | "
                     << setw(DO_RONG_TEN) << left << sv->sv.ten << " |";

                const short xDiem = wherex();
                const short yDiem = wherey();
                const float diemHienTai = dk->dk.diem;
                if (diemHienTai >= 0.0f)
                    cout << setw(DO_RONG_DIEM) << fixed << setprecision(2) << diemHienTai;
                else
                    cout << setw(DO_RONG_DIEM) << " ";
                cout << " |\n";

                dk->dk.diem = nhapDiemTaiO(xDiem, yDiem, diemHienTai);
                gotoxy(xDiem, yDiem);
                cout << setw(DO_RONG_DIEM) << right << fixed << setprecision(2)
                     << dk->dk.diem << " |\n";
                inKhungBangDiem();
                ++stt;
            }
            sv = sv->next;
        }
    }
}

void inBangDiemLopTinChi(const DanhSachLop& dsLop, const LopTinChi& lopTC, PTRMH rootMonHoc) {
    cout << "\n";
    PTRMH monHoc = timMonHocTheoMa(rootMonHoc, lopTC.maMH);
    const char* tenMonHoc = monHoc == nullptr ? lopTC.maMH : monHoc->mh.tenMH;
    cout << "                 BANG DIEM MON HOC " << tenMonHoc << endl;
    cout << "Nien khoa: " << lopTC.nienKhoa
         << "    Hoc ky: " << lopTC.hocKy
         << "    Nhom: " << lopTC.nhom << endl;

    inTieuDeBangDiem("DIEM");

    int stt = 1;

    for (int i = 0; i < dsLop.soLuong; i++) {
        PTRSV sv = dsLop.lop[i].dssv;

        while (sv != nullptr) {
            PTRDK dk = timDangKyTheoMaSV(lopTC.dssvdk, sv->sv.maSV);

            if (dk != nullptr && !dk->dk.huyDangKy && dk->dk.diem >= 0.0f) {
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
static float tinhDiemTrungBinhSinhVien(
    PTRSV sv,
    const DanhSachLopTinChi& dsLopTC,
    PTRMH root
) {
    char dsMaMH[MAX_LOP_TC][MAX_MAMH]{};
    float dsDiemCaoNhat[MAX_LOP_TC]{};
    int soLuongMon = 0;

    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        const LopTinChi* lopTC = dsLopTC.lopTC[i];
        if (lopTC == nullptr || lopTC->huyLop) {
            continue;
        }

        PTRDK dk = timDangKyTheoMaSV(lopTC->dssvdk, sv->sv.maSV);
        if (dk == nullptr || dk->dk.huyDangKy || dk->dk.diem < 0.0f) {
            continue;
        }

        int viTriMon = 0;
        while (viTriMon < soLuongMon &&
               strcmp(dsMaMH[viTriMon], lopTC->maMH) != 0) {
            ++viTriMon;
        }

        if (viTriMon == soLuongMon) {
            if (soLuongMon >= MAX_LOP_TC) {
                continue;
            }
            strcpy(dsMaMH[soLuongMon], lopTC->maMH);
            dsDiemCaoNhat[soLuongMon] = dk->dk.diem;
            ++soLuongMon;
        }
        else if (dk->dk.diem > dsDiemCaoNhat[viTriMon]) {
            dsDiemCaoNhat[viTriMon] = dk->dk.diem;
        }
    }

    float tongDiemTinChi = 0.0f;
    int tongSoTinChi = 0;

    for (int i = 0; i < soLuongMon; ++i) {
        PTRMH nodeMonHoc = timMonHocTheoMa(root, dsMaMH[i]);
        if (nodeMonHoc == nullptr) {
            continue;
        }

        const int soTinChi =
            nodeMonHoc->mh.soTCLT + nodeMonHoc->mh.soTCTH;
        if (soTinChi <= 0) {
            continue;
        }

        tongDiemTinChi += dsDiemCaoNhat[i] * soTinChi;
        tongSoTinChi += soTinChi;
    }

    if (tongSoTinChi == 0) {
        return -1.0f;
    }

    return tongDiemTinChi / tongSoTinChi;
}void inBangDiemTrungBinh(const DanhSachLop& dsLop, const DanhSachLopTinChi& dsLopTC, PTRMH root, const char* maLop) {
    int viTri = timLopTheoMa(dsLop, maLop);

    if (viTri == -1) {
        cout << "Khong tim thay lop!\n";
        return;
    }

    const Lop& lop = dsLop.lop[viTri];

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

        cout << "| " << setw(DO_RONG_STT) << right << stt << " | "
             << setw(DO_RONG_MASV) << left << sv->sv.maSV << " | "
             << setw(DO_RONG_HO) << left << sv->sv.ho << " | "
             << setw(DO_RONG_TEN) << left << sv->sv.ten << " | "
             << setw(DO_RONG_DIEM) << right;
        if (diemTB < 0.0f) cout << " ";
        else cout << fixed << setprecision(2) << diemTB;
        cout << " |\n";
        inKhungBangDiem();

        stt++;
        sv = sv->next;
    }
}
static float timDiemThiLonNhat(
    const char* maSV,
    const char* maMH,
    const DanhSachLopTinChi& dsLopTC
) {
    float diemLonNhat = -1;

    for (int i = 0; i < dsLopTC.soLuong; i++) {
        const LopTinChi* lopTC = dsLopTC.lopTC[i];

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
static int layDanhSachMonHocCuaLop(
    const Lop& lop,
    const DanhSachLopTinChi& dsLopTC,
    char dsMaMH[][MAX_MAMH]
) {
    int soLuongMon = 0;

    for (int i = 0; i < dsLopTC.soLuong; i++) {
        const LopTinChi* lopTC = dsLopTC.lopTC[i];

        if (lopTC == nullptr || lopTC->huyLop)
            continue;

        bool coSinhVien = false;

        PTRSV sv = lop.dssv;

        while (sv != nullptr) {
            PTRDK dk = timDangKyTheoMaSV(
                lopTC->dssvdk,
                sv->sv.maSV
            );

            if (dk != nullptr && !dk->dk.huyDangKy && dk->dk.diem >= 0.0f) {
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
    const Lop& lop,
    const DanhSachLopTinChi& dsLopTC
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
