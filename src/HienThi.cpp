#include "HienThi.h"
#include "mylib.h"
#include "LopTinChi.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

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
    cout << "+-----+---------------+-------------------------+-----------------+--------+\n";
    cout << "| STT | MASV          | HO                      | TEN             | DIEM   |\n";
    cout << "+-----+---------------+-------------------------+-----------------+--------+\n";

    int stt = 1;

    for (int i = 0; i < dsLop.soLuong; i++) {
        PTRSV sv = dsLop.lop[i].dssv;

        while (sv != nullptr) {
            PTRDK dk = timDangKyTheoMaSV(dssvdk, sv->sv.maSV);

            if (dk != nullptr) {
                cout << "| "
                     << setw(3) << left << stt << " | "
                     << setw(13) << left << sv->sv.maSV << " | "
                     << setw(23) << left << sv->sv.ho << " | "
                     << setw(15) << left << sv->sv.ten << " |";

                short xDiem = wherex();
                short yDiem = wherey();

                cout << setw(8) << " " << "|\n";

                dk->dk.diem = nhapDiemTrenBang(xDiem, yDiem);

                gotoxy(xDiem, yDiem);
                cout << setw(7) << right << dk->dk.diem << " |\n";

                cout << "+-----+---------------+-------------------------+-----------------+--------+\n";

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

    cout << "+-----+---------------+-------------------------+-----------------+--------+\n";
    cout << "| STT | MASV          | HO                      | TEN             | DIEM   |\n";
    cout << "+-----+---------------+-------------------------+-----------------+--------+\n";

    int stt = 1;

    for (int i = 0; i < dsLop.soLuong; i++) {
        PTRSV sv = dsLop.lop[i].dssv;

        while (sv != nullptr) {
            PTRDK dk = timDangKyTheoMaSV(lopTC.dssvdk, sv->sv.maSV);

            if (dk != nullptr) {
                cout << "| "
                     << setw(3) << right << stt << " | "
                     << setw(13) << left << sv->sv.maSV << " | "
                     << setw(23) << left << sv->sv.ho << " | "
                     << setw(15) << left << sv->sv.ten << " | "
                     << setw(6) << right << dk->dk.diem << " |\n";

                cout << "+-----+---------------+-------------------------+-----------------+--------+\n";

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

    cout << "+-----+-------------------+-------------------------+-----------------+----------+\n";
    cout << "| STT | MASV              | HO                      | TEN             | DIEM TB  |\n";
    cout << "+-----+-------------------+-------------------------+-----------------+----------+\n";

    int stt = 1;
    PTRSV sv = lop.dssv;

    while (sv != nullptr) {
        float diemTB = tinhDiemTrungBinhSinhVien(
            sv,
            dsLopTC,
            root
        );

        cout << "| "
             << setw(3) << right << stt << " | "
             << setw(17) << left << sv->sv.maSV << " | "
             << setw(23) << left << sv->sv.ho << " | "
             << setw(15) << left << sv->sv.ten << " | "
             << setw(8) << right << fixed << setprecision(2) << diemTB << " |\n";

        cout << "+-----+-------------------+-------------------------+-----------------+----------+\n";

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
    char dsMaMH[][11]
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
    char dsMaMH[MAX_LOP_TC][11];

    int soLuongMon = layDanhSachMonHocCuaLop(
        lop,
        dsLopTC,
        dsMaMH
    );

    cout << "\n";
    cout << "                         BANG DIEM TONG KET\n";
    cout << "Lop: " << lop.maLop << endl;
    cout << "\n";

    cout << "+-----+---------------+-------------------------+";

    for (int i = 0; i < soLuongMon; i++)
        cout << "----------+";

    cout << "\n";

    cout << "| STT | MASV          | HO TEN                  |";

    for (int i = 0; i < soLuongMon; i++)
        cout << " " << setw(8) << left << dsMaMH[i] << "|";

    cout << "\n";

    cout << "+-----+---------------+-------------------------+";

    for (int i = 0; i < soLuongMon; i++)
        cout << "----------+";

    cout << "\n";

    int stt = 1;

    PTRSV sv = lop.dssv;

    while (sv != nullptr) {
        cout << "| "
             << setw(3) << right << stt << " | "
             << setw(13) << left << sv->sv.maSV << " | ";

        char hoTen[82];
        strcpy(hoTen, sv->sv.ho);
        strcat(hoTen, " ");
        strcat(hoTen, sv->sv.ten);

        cout << setw(23) << left << hoTen << " |";

        for (int i = 0; i < soLuongMon; i++) {
            float diem = timDiemThiLonNhat(
                sv->sv.maSV,
                dsMaMH[i],
                dsLopTC
            );

            if (diem == -1) {
                cout << setw(10) << " " << "|";
            }
            else {
                cout << setw(9) << right
                     << fixed << setprecision(2)
                     << diem << " |";
            }
        }

        cout << "\n";

        cout << "+-----+---------------+-------------------------+";

        for (int i = 0; i < soLuongMon; i++)
            cout << "----------+";

        cout << "\n";

        stt++;
        sv = sv->next;
    }
}
