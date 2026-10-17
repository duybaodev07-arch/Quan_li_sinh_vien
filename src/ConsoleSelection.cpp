#include "ConsoleSelection.h"
#include "ConsoleUI.h"
#include "Input.h"

#include <cstdio>
#include <cstring>
#include <iostream>

namespace {
const int DO_DAI_DONG_CHON = 180;

void taoDong(char* dong, const char* ma, const char* ten) {
    std::snprintf(dong, DO_DAI_DONG_CHON, "%-16s | %s", ma, ten);
}

void giaiPhongMuc(char** noiDung, const char** cacMuc, int soLuong) {
    if (noiDung != nullptr) {
        for (int i = 0; i < soLuong; ++i) {
            delete[] noiDung[i];
        }
        delete[] noiDung;
    }
    delete[] cacMuc;
}

void thuThapMonHoc(PTRMH node, MonHoc* danhSach, int& viTri) {
    if (node == nullptr) {
        return;
    }

    thuThapMonHoc(node->left, danhSach, viTri);
    danhSach[viTri] = node->mh;
    ++viTri;
    thuThapMonHoc(node->right, danhSach, viTri);
}

bool chonDong(
    const char* tieuDe,
    char** noiDung,
    int soLuong,
    int& viTri
) {
    if (soLuong <= 0) {
        std::cout << "Danh sach rong!\n";
        return false;
    }

    const char** cacMuc = new const char*[soLuong];
    for (int i = 0; i < soLuong; ++i) {
        cacMuc[i] = noiDung[i];
    }

    viTri = chonMenuConsole(
        tieuDe,
        cacMuc,
        soLuong,
        "[LEN/XUONG]: Chon  |  [TRAI/PHAI]: Trang  |  [ENTER]: Xac nhan  |  [ESC]: Quay lai"
    );

    delete[] cacMuc;
    return viTri >= 0 && viTri < soLuong;
}

char** taoMangDong(int soLuong) {
    char** noiDung = new char*[soLuong];
    for (int i = 0; i < soLuong; ++i) {
        noiDung[i] = new char[DO_DAI_DONG_CHON];
        noiDung[i][0] = '\0';
    }
    return noiDung;
}
}

bool chonMonHocTuDanhSach(PTRMH root, char* maMH) {
    int soLuong = 0;
    demSoMonHoc(root, soLuong);
    if (soLuong == 0) {
        std::cout << "Danh sach mon hoc rong!\n";
        return false;
    }

    MonHoc* danhSach = new MonHoc[soLuong];
    int viTriThuThap = 0;
    thuThapMonHoc(root, danhSach, viTriThuThap);

    char** noiDung = taoMangDong(soLuong);
    for (int i = 0; i < soLuong; ++i) {
        taoDong(noiDung[i], danhSach[i].maMH, danhSach[i].tenMH);
    }

    int viTri = -1;
    const bool daChon = chonDong(
        "CHON MON HOC",
        noiDung,
        soLuong,
        viTri
    );

    if (daChon) {
        std::strcpy(maMH, danhSach[viTri].maMH);
    }

    giaiPhongMuc(noiDung, nullptr, soLuong);
    delete[] danhSach;
    return daChon;
}

bool chonSinhVienTuDanhSach(PTRSV dssv, char* maSV) {
    const int soLuong = demSoSinhVien(dssv);
    if (soLuong == 0) {
        std::cout << "Danh sach sinh vien rong!\n";
        return false;
    }

    PTRSV* danhSach = new PTRSV[soLuong];
    char** noiDung = taoMangDong(soLuong);
    int viTri = 0;

    for (PTRSV p = dssv; p != nullptr; p = p->next) {
        danhSach[viTri] = p;
        std::snprintf(
            noiDung[viTri],
            DO_DAI_DONG_CHON,
            "%-16s | %-35s | %s",
            p->sv.maSV,
            p->sv.ho,
            p->sv.ten
        );
        ++viTri;
    }

    const bool daChon = chonDong(
        "CHON SINH VIEN",
        noiDung,
        soLuong,
        viTri
    );

    if (daChon) {
        std::strcpy(maSV, danhSach[viTri]->sv.maSV);
    }

    giaiPhongMuc(noiDung, nullptr, soLuong);
    delete[] danhSach;
    return daChon;
}

bool chonLopTuDanhSach(const DanhSachLop& dsLop, char* maLop) {
    const int soLuong = dsLop.soLuong;
    if (soLuong == 0) {
        std::cout << "Danh sach lop rong!\n";
        return false;
    }

    char** noiDung = taoMangDong(soLuong);
    for (int i = 0; i < soLuong; ++i) {
        taoDong(noiDung[i], dsLop.lop[i].maLop, dsLop.lop[i].tenLop);
    }

    int viTri = -1;
    const bool daChon = chonDong(
        "CHON LOP",
        noiDung,
        soLuong,
        viTri
    );

    if (daChon) {
        std::strcpy(maLop, dsLop.lop[viTri].maLop);
    }

    giaiPhongMuc(noiDung, nullptr, soLuong);
    return daChon;
}

bool chonLopTinChiTuDanhSach(
    const DanhSachLopTinChi& dsLopTC,
    int& maLopTC
) {
    const int soLuong = dsLopTC.soLuong;
    if (soLuong == 0) {
        std::cout << "Danh sach lop tin chi rong!\n";
        return false;
    }

    char** noiDung = taoMangDong(soLuong);
    for (int i = 0; i < soLuong; ++i) {
        const LopTinChi& lopTC = *dsLopTC.lopTC[i];
        char thongTin[DO_DAI_DONG_CHON];
        std::snprintf(
            thongTin,
            sizeof(thongTin),
            "%s | NK %s | HK %d | Nhom %d",
            lopTC.maMH,
            lopTC.nienKhoa,
            lopTC.hocKy,
            lopTC.nhom
        );
        std::snprintf(
            noiDung[i],
            DO_DAI_DONG_CHON,
            "Ma %d | %s",
            lopTC.maLopTC,
            thongTin
        );
    }

    int viTri = -1;
    const bool daChon = chonDong(
        "CHON LOP TIN CHI",
        noiDung,
        soLuong,
        viTri
    );

    if (daChon) {
        maLopTC = dsLopTC.lopTC[viTri]->maLopTC;
    }

    giaiPhongMuc(noiDung, nullptr, soLuong);
    return daChon;
}

bool xacNhanConsole(const char* thongBao) {
    std::cout << thongBao << " (Y/N): ";
    return nhapXacNhan() == 'Y';
}

