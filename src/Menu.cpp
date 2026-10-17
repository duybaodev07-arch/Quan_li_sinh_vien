#include "Menu.h"
#include "ConsoleUI.h"
#include "ConsoleSelection.h"
#include "Input.h"
#include <cstdlib>
#include <iostream>
#include <cstring>

using namespace std;

namespace {
bool danhSachLopTinChiRong(const DanhSachLopTinChi& dsLopTC) {
    if (dsLopTC.soLuong > 0) {
        return false;
    }

    cout << "Danh sach lop tin chi rong! Hay them lop truoc.\n";
    return true;
}

bool monHocDangDuocMoLop(
    const DanhSachLopTinChi& dsLopTC,
    const char* maMH
) {
    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        if (dsLopTC.lopTC[i] != nullptr &&
            strcmp(dsLopTC.lopTC[i]->maMH, maMH) == 0) {
            return true;
        }
    }
    return false;
}

void timSinhVienTheoTuKhoa(PTRSV dssv, const char* tuKhoa) {
    int soSinhVienTimThay = 0;
    for (PTRSV p = dssv; p != nullptr; p = p->next) {
        char maSV[MAX_MASV];
        char ho[MAX_HO];
        char ten[MAX_TEN];

        strcpy(maSV, p->sv.maSV);
        strcpy(ho, p->sv.ho);
        strcpy(ten, p->sv.ten);
        chuanHoaMa(maSV);
        chuanHoaMa(ho);
        chuanHoaMa(ten);

        if (strstr(maSV, tuKhoa) != nullptr ||
            strstr(ho, tuKhoa) != nullptr ||
            strstr(ten, tuKhoa) != nullptr) {
            cout << p->sv.maSV << " | "
                 << p->sv.ho << " " << p->sv.ten << " | "
                 << p->sv.phai << " | " << p->sv.soDT << '\n';
            ++soSinhVienTimThay;
        }
    }

    if (soSinhVienTimThay == 0) {
        cout << "Khong tim thay sinh vien phu hop.\n";
    }
    else {
        cout << "Tong so sinh vien tim thay: "
             << soSinhVienTimThay << '\n';
    }
}
void inDanhSachLopTinChiDaLoc(
    const DanhSachLopTinChi& dsLopTC,
    PTRMH rootMonHoc
) {
    char nienKhoa[MAX_NIENKHOA];
    char tuKhoa[MAX_TENMH];

    cout << "Nhap nien khoa can xem (YYYY-YYYY): ";
    nhapNienKhoa(nienKhoa, MAX_NIENKHOA);
    cout << "Nhap ma/ten mon hoc can tim (Enter de bo qua): ";
    nhapChuoiChoPhepRong(tuKhoa, MAX_TENMH);
    chuanHoaMa(tuKhoa);

    int soLopTimThay = 0;
    for (int i = 0; i < dsLopTC.soLuong; ++i) {
        const LopTinChi& lopTC = *dsLopTC.lopTC[i];
        if (strcmp(lopTC.nienKhoa, nienKhoa) != 0) {
            continue;
        }

        bool khopTuKhoa = tuKhoa[0] == '\0';
        if (!khopTuKhoa) {
            char maMH[MAX_MAMH];
            strcpy(maMH, lopTC.maMH);
            chuanHoaMa(maMH);
            khopTuKhoa = strstr(maMH, tuKhoa) != nullptr;

            PTRMH monHoc = timMonHocTheoMa(rootMonHoc, lopTC.maMH);
            if (!khopTuKhoa && monHoc != nullptr) {
                char tenMH[MAX_TENMH];
                strcpy(tenMH, monHoc->mh.tenMH);
                chuanHoaMa(tenMH);
                khopTuKhoa = strstr(tenMH, tuKhoa) != nullptr;
            }
        }

        if (khopTuKhoa) {
            inLopTinChi(lopTC);
            ++soLopTimThay;
        }
    }

    if (soLopTimThay == 0) {
        cout << "Khong co lop tin chi phu hop.\n";
    }
    else {
        cout << "Tong so lop tin chi: " << soLopTimThay << '\n';
    }
}
}

void menuMonHoc(PTRMH& root, const DanhSachLopTinChi& dsLopTC) {
    const char* cacMuc[] = {
        "Thao tac mon hoc",
        "Tim mon hoc theo ma",
        "In danh sach mon hoc theo ma",
        "In danh sach mon hoc theo ten",
        "Quay lai"
    };

    const char* cacThaoTac[] = {
        "[F1] Them mon hoc",
        "[F3] Hieu chinh mon hoc",
        "[DEL] Xoa mon hoc",
        "[F2] Luu danh sach mon hoc",
        "Xoa toan bo mon hoc",
        "Quay lai"
    };

    while (true) {
        const int luaChon = chonMenuConsole(
            "QUAN LY MON HOC",
            cacMuc,
            static_cast<int>(sizeof(cacMuc) / sizeof(cacMuc[0])),
            "[LEN/XUONG]: Di chuyen  |  [ENTER]: Chon  |  [ESC]: Quay lai"
        );

        if (luaChon < 0 || luaChon == 4) {
            return;
        }

        if (luaChon == 0) {
            while (true) {
                const int thaoTac = chonMenuConsole(
                    "THAO TAC MON HOC",
                    cacThaoTac,
                    static_cast<int>(sizeof(cacThaoTac) / sizeof(cacThaoTac[0])),
                    "[LEN/XUONG]: Di chuyen  |  [ENTER]: Chon  |  [ESC]: Quay lai"
                );

                if (thaoTac < 0 || thaoTac == 5) {
                    break;
                }

                if (thaoTac == 0) {
                    themMonHoc(root);
                    choNhanPhim();
                }
                else if (thaoTac == 1) {
                    char maMH[MAX_MAMH];
                    if (!chonMonHocTuDanhSach(root, maMH)) {
                        continue;
                    }

                    if (hieuChinhMonHoc(root, maMH)) {
                        cout << "Hieu chinh mon hoc thanh cong!\n";
                    }
                    else {
                        cout << "Khong tim thay mon hoc!\n";
                    }
                    choNhanPhim();
                }
                else if (thaoTac == 2) {
                    char maMH[MAX_MAMH];
                    if (!chonMonHocTuDanhSach(root, maMH)) {
                        continue;
                    }

                    if (monHocDangDuocMoLop(dsLopTC, maMH)) {
                        cout << "Mon hoc dang duoc lop tin chi su dung, khong the xoa!\n";
                    }
                    else if (!xacNhanConsole("Xac nhan xoa mon hoc")) {
                        cout << "Da huy thao tac xoa.\n";
                    }
                    else if (xoaMonHoc(root, maMH)) {
                        cout << "Xoa mon hoc thanh cong!\n";
                    }
                    else {
                        cout << "Khong tim thay mon hoc!\n";
                    }
                    choNhanPhim();
                }
                else if (thaoTac == 3) {
                    if (ghiFileMonHoc(root)) {
                        cout << "Da luu danh sach mon hoc.\n";
                    }
                    else {
                        cout << "Luu danh sach mon hoc that bai.\n";
                    }
                    choNhanPhim();
                }
                else if (thaoTac == 4) {
                    if (dsLopTC.soLuong > 0) {
                        cout << "Khong the xoa mon hoc khi van con lop tin chi.\n";
                    }
                    else if (root == nullptr) {
                        cout << "Danh sach mon hoc da rong.\n";
                    }
                    else if (xacNhanConsole("Xac nhan xoa toan bo mon hoc")) {
                        giaiPhongMonHoc(root);
                        cout << "Da xoa toan bo mon hoc.\n";
                    }
                    else {
                        cout << "Da huy thao tac xoa.\n";
                    }
                    choNhanPhim();
                }
            }
        }
        else if (luaChon == 1) {
            char maMH[MAX_MAMH];
            cout << "Nhap ma mon hoc can tim: ";
            nhapMaBatBuoc(maMH, MAX_MAMH);
            PTRMH monHoc = timMonHocTheoMa(root, maMH);

            if (monHoc == nullptr) {
                cout << "Khong tim thay mon hoc!\n";
            }
            else {
                inMonHoc(monHoc->mh);
            }
            choNhanPhim();
        }
        else if (luaChon == 2) {
            inDanhSachMonHocTheoMa(root);
            choNhanPhim();
        }
        else if (luaChon == 3) {
            inDanhSachMonHocTheoTen(root);
            choNhanPhim();
        }
    }
}

void menuSinhVien(DanhSachLop& dsLop, int viTri) {
    if (viTri < 0 || viTri >= dsLop.soLuong) {
        return;
    }

    Lop& lop = dsLop.lop[viTri];
    const char* cacMuc[] = {
        "Thao tac sinh vien",
        "In danh sach theo ma",
        "In danh sach theo ten",
        "Tim sinh vien theo ma/ho/ten",
        "Quay lai"
    };
    const char* cacThaoTac[] = {
        "[F1] Them sinh vien vao lop",
        "[F3] Hieu chinh sinh vien",
        "[DEL] Xoa sinh vien khoi lop",
        "[F2] Luu danh sach sinh vien",
        "Quay lai"
    };

    while (true) {
        const int luaChon = chonMenuConsole(
            "QUAN LY SINH VIEN TRONG LOP",
            cacMuc,
            static_cast<int>(sizeof(cacMuc) / sizeof(cacMuc[0])),
            "[LEN/XUONG]: Di chuyen  |  [ENTER]: Chon  |  [ESC]: Quay lai"
        );

        if (luaChon < 0 || luaChon == 4) {
            return;
        }

        if (luaChon == 0) {
            while (true) {
                const int thaoTac = chonMenuConsole(
                    "THAO TAC SINH VIEN",
                    cacThaoTac,
                    static_cast<int>(sizeof(cacThaoTac) / sizeof(cacThaoTac[0])),
                    "[LEN/XUONG]: Di chuyen  |  [ENTER]: Chon  |  [ESC]: Quay lai"
                );

                if (thaoTac < 0 || thaoTac == 4) {
                    break;
                }

                if (thaoTac == 3) {
                    if (ghiFileLop(dsLop)) {
                        cout << "Da luu danh sach sinh vien.\n";
                    }
                    else {
                        cout << "Luu danh sach sinh vien that bai.\n";
                    }
                    choNhanPhim();
                    continue;
                }

                char maSV[MAX_MASV];
                if (thaoTac == 0) {
                    themSinhVienVaoLop(dsLop, viTri);
                    choNhanPhim();
                }
                else if (thaoTac == 1) {
                    if (!chonSinhVienTuDanhSach(lop.dssv, maSV)) {
                        continue;
                    }
                    if (hieuChinhSinhVien(lop.dssv, maSV)) {
                        cout << "Hieu chinh sinh vien thanh cong!\n";
                    }
                    else {
                        cout << "Khong tim thay sinh vien!\n";
                    }
                    choNhanPhim();
                }
                else if (thaoTac == 2) {
                    if (!chonSinhVienTuDanhSach(lop.dssv, maSV)) {
                        continue;
                    }
                    if (!xacNhanConsole("Xac nhan xoa sinh vien")) {
                        cout << "Da huy thao tac xoa.\n";
                    }
                    else if (xoaSinhVien(lop.dssv, maSV)) {
                        cout << "Xoa sinh vien thanh cong!\n";
                    }
                    else {
                        cout << "Khong tim thay sinh vien!\n";
                    }
                    choNhanPhim();
                }
            }
        }
        else if (luaChon == 1) {
            inDanhSachSinhVienTheoMa(lop.dssv);
            choNhanPhim();
        }
        else if (luaChon == 2) {
            inDanhSachSinhVienTheoTen(lop.dssv);
            choNhanPhim();
        }
        else if (luaChon == 3) {
            char tuKhoa[MAX_HO];
            cout << "Nhap ma, ho hoac ten sinh vien can tim: ";
            nhapChuoi(tuKhoa, MAX_HO);
            chuanHoaMa(tuKhoa);
            timSinhVienTheoTuKhoa(lop.dssv, tuKhoa);
            choNhanPhim();
        }
    }
}

void menuLop(DanhSachLop& dsLop) {
    const char* cacMuc[] = {
        "Thao tac lop",
        "Cap nhat sinh vien cua lop",
        "In danh sach lop",
        "Tim sinh vien theo ma/ho/ten",
        "Quay lai"
    };
    const char* cacThaoTac[] = {
        "[F1] Them lop",
        "[F3] Hieu chinh lop",
        "[DEL] Xoa lop",
        "[F2] Luu danh sach lop",
        "Quay lai"
    };

    while (true) {
        const int luaChon = chonMenuConsole(
            "QUAN LY LOP VA SINH VIEN",
            cacMuc,
            static_cast<int>(sizeof(cacMuc) / sizeof(cacMuc[0])),
            "[LEN/XUONG]: Di chuyen  |  [ENTER]: Chon  |  [ESC]: Quay lai"
        );

        if (luaChon < 0 || luaChon == 4) {
            return;
        }

        if (luaChon == 0) {
            while (true) {
                const int thaoTac = chonMenuConsole(
                    "THAO TAC LOP",
                    cacThaoTac,
                    static_cast<int>(sizeof(cacThaoTac) / sizeof(cacThaoTac[0])),
                    "[LEN/XUONG]: Di chuyen  |  [ENTER]: Chon  |  [ESC]: Quay lai"
                );

                if (thaoTac < 0 || thaoTac == 4) {
                    break;
                }

                if (thaoTac == 3) {
                    if (ghiFileLop(dsLop)) {
                        cout << "Da luu danh sach lop.\n";
                    }
                    else {
                        cout << "Luu danh sach lop that bai.\n";
                    }
                    choNhanPhim();
                    continue;
                }

                if (thaoTac == 0) {
                    themLop(dsLop);
                    choNhanPhim();
                }
                else if (thaoTac == 1) {
                    char maLop[MAX_MALOP];
                    if (!chonLopTuDanhSach(dsLop, maLop)) {
                        continue;
                    }
                    if (hieuChinhLop(dsLop, maLop)) {
                        cout << "Hieu chinh lop thanh cong!\n";
                    }
                    else {
                        cout << "Khong tim thay lop!\n";
                    }
                    choNhanPhim();
                }
                else if (thaoTac == 2) {
                    char maLop[MAX_MALOP];
                    if (!chonLopTuDanhSach(dsLop, maLop)) {
                        continue;
                    }
                    if (!xacNhanConsole("Xac nhan xoa lop")) {
                        cout << "Da huy thao tac xoa.\n";
                    }
                    else if (xoaLop(dsLop, maLop)) {
                        cout << "Xoa lop thanh cong!\n";
                    }
                    else {
                        cout << "Khong tim thay lop!\n";
                    }
                    choNhanPhim();
                }
            }
        }
        else if (luaChon == 1) {
            char maLop[MAX_MALOP];
            cout << "Nhap ma lop can cap nhat sinh vien: ";
            nhapMaBatBuoc(maLop, MAX_MALOP);
            const int viTri = timLopTheoMa(dsLop, maLop);

            if (viTri == -1) {
                cout << "Khong tim thay lop!\n";
                choNhanPhim();
                continue;
            }

            inLop(dsLop.lop[viTri]);
            cout << "Cap nhat sinh vien lop nay? (Y/N): ";
            if (nhapTrangThaiHuy()) {
                menuSinhVien(dsLop, viTri);
            }
            choNhanPhim();
        }
        else if (luaChon == 2) {
            inDanhSachLop(dsLop);
            choNhanPhim();
        }
        else if (luaChon == 3) {
            char maSV[MAX_MASV];
            cout << "Nhap ma sinh vien can tim: ";
            nhapMaBatBuoc(maSV, MAX_MASV);
            PTRSV sinhVien = timSinhVienTrongDanhSachLop(dsLop, maSV);

            if (sinhVien == nullptr) {
                cout << "Khong tim thay sinh vien!\n";
            }
            else {
                inSinhVien(sinhVien->sv);
            }
            choNhanPhim();
        }
    }
}

void menuLopTinChi(
    DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH root
) {
    const char* cacMuc[] = {
        "Thao tac lop tin chi",
        "Huy lop tin chi",
        "Loc lop tin chi theo nien khoa va ma/ten mon hoc",
        "Tu dong huy lop khong du sinh vien",
        "Nhap diem",
        "Xem bang diem lop tin chi",
        "In bang diem trung binh",
        "In bang diem tong ket",
        "Quay lai"
    };
    const char* cacThaoTac[] = {
        "[F1] Them lop tin chi",
        "[F3] Hieu chinh lop tin chi",
        "[DEL] Xoa lop tin chi",
        "[F2] Luu danh sach lop tin chi",
        "Quay lai"
    };

    while (true) {
        const int luaChon = chonMenuConsole(
            "QUAN LY LOP TIN CHI",
            cacMuc,
            static_cast<int>(sizeof(cacMuc) / sizeof(cacMuc[0])),
            "[LEN/XUONG]: Di chuyen  |  [ENTER]: Chon  |  [ESC]: Quay lai"
        );

        if (luaChon < 0 || luaChon == 8) {
            return;
        }

        if (luaChon == 0) {
            while (true) {
                const int thaoTac = chonMenuConsole(
                    "THAO TAC LOP TIN CHI",
                    cacThaoTac,
                    static_cast<int>(sizeof(cacThaoTac) / sizeof(cacThaoTac[0])),
                    "[LEN/XUONG]: Di chuyen  |  [ENTER]: Chon  |  [ESC]: Quay lai"
                );

                if (thaoTac < 0 || thaoTac == 4) {
                    break;
                }

                if (thaoTac == 3) {
                    if (ghiFileLopTinChi(dsLopTC)) {
                        cout << "Da luu danh sach lop tin chi.\n";
                    }
                    else {
                        cout << "Luu danh sach lop tin chi that bai.\n";
                    }
                    choNhanPhim();
                    continue;
                }

                if (thaoTac == 0) {
                    themLopTinChi(dsLopTC, root);
                    choNhanPhim();
                }
                else if (thaoTac == 1) {
                    if (danhSachLopTinChiRong(dsLopTC)) {
                        choNhanPhim();
                        continue;
                    }
                    int maLopTC = 0;
                    if (!chonLopTinChiTuDanhSach(dsLopTC, maLopTC)) {
                        continue;
                    }
                    if (hieuChinhLopTinChi(dsLopTC, maLopTC, root)) {
                        cout << "Hieu chinh lop tin chi thanh cong!\n";
                    }
                    else {
                        cout << "Khong tim thay lop tin chi!\n";
                    }
                    choNhanPhim();
                }
                else if (thaoTac == 2) {
                    if (danhSachLopTinChiRong(dsLopTC)) {
                        choNhanPhim();
                        continue;
                    }
                    int maLopTC = 0;
                    if (!chonLopTinChiTuDanhSach(dsLopTC, maLopTC)) {
                        continue;
                    }
                    if (!xacNhanConsole("Xac nhan xoa lop tin chi")) {
                        cout << "Da huy thao tac xoa.\n";
                    }
                    else if (xoaLopTinChi(dsLopTC, maLopTC)) {
                        cout << "Xoa lop tin chi thanh cong!\n";
                    }
                    else {
                        cout << "Khong tim thay lop tin chi!\n";
                    }
                    choNhanPhim();
                }
            }
        }
        else if (luaChon == 1) {
            if (!danhSachLopTinChiRong(dsLopTC)) {
                int maLopTC = 0;
                if (!chonLopTinChiTuDanhSach(dsLopTC, maLopTC)) {
                    choNhanPhim();
                    continue;
                }
                if (!xacNhanConsole("Xac nhan huy lop tin chi")) {
                    cout << "Da huy thao tac.\n";
                }
                else if (huyLopTinChi(dsLopTC, maLopTC)) {
                    cout << "Huy lop tin chi thanh cong!\n";
                }
                else {
                    cout << "Khong tim thay lop tin chi!\n";
                }
            }
            choNhanPhim();
        }
        else if (luaChon == 2) {
            if (!danhSachLopTinChiRong(dsLopTC)) {
                inDanhSachLopTinChiDaLoc(dsLopTC, root);
            }
            choNhanPhim();
        }
        else if (luaChon == 3) {
            if (!danhSachLopTinChiRong(dsLopTC)) {
                char nienKhoa[MAX_NIENKHOA];
                cout << "Nhap nien khoa can xet huy: ";
                nhapNienKhoa(nienKhoa, MAX_NIENKHOA);
                cout << "Nhap hoc ky can xet huy: ";
                const int hocKy = nhapSoNguyenKhongAm();
                tuDongHuyDanhSachLopKhongDuSinhVien(
                    dsLopTC,
                    nienKhoa,
                    hocKy
                );
            }
            choNhanPhim();
        }
        else if (luaChon == 4) {
            if (!danhSachLopTinChiRong(dsLopTC)) {
                nhapDiem(dsLopTC, dsLop);
            }
            choNhanPhim();
        }
        else if (luaChon == 5) {
            if (!danhSachLopTinChiRong(dsLopTC)) {
                xemBangDiemLopTinChi(dsLopTC, dsLop, root);
            }
            choNhanPhim();
        }
        else if (luaChon == 6) {
            if (!danhSachLopTinChiRong(dsLopTC)) {
                char maLop[MAX_MALOP];
                cout << "Nhap ma lop: ";
                nhapMaBatBuoc(maLop, MAX_MALOP);
                inBangDiemTrungBinh(dsLop, dsLopTC, root, maLop);
            }
            choNhanPhim();
        }
        else if (luaChon == 7) {
            if (!danhSachLopTinChiRong(dsLopTC)) {
                char maLop[MAX_MALOP];
                cout << "Nhap ma lop: ";
                nhapMaBatBuoc(maLop, MAX_MALOP);
                const int viTri = timLopTheoMa(dsLop, maLop);

                if (viTri == -1) {
                    cout << "Khong tim thay lop!\n";
                }
                else {
                    inBangDiemTongKet(dsLop.lop[viTri], dsLopTC);
                }
            }
            choNhanPhim();
        }
    }
}

void menuDangKy(
    DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH rootMonHoc
) {
    const char* cacMuc[] = {
        "Dang ky lop tin chi",
        "Huy dang ky lop tin chi",
        "Xem danh sach sinh vien dang ky",
        "Quay lai"
    };

    while (true) {
        const int luaChon = chonMenuConsole(
            "DANG KY LOP TIN CHI",
            cacMuc,
            static_cast<int>(sizeof(cacMuc) / sizeof(cacMuc[0])),
            "[LEN/XUONG]: Di chuyen  |  [ENTER]: Chon  |  [ESC]: Quay lai"
        );

        if (luaChon < 0 || luaChon == 3) {
            return;
        }

        if (danhSachLopTinChiRong(dsLopTC)) {
            choNhanPhim();
            continue;
        }

        if (luaChon == 0) {
            dangKyLopTinChi(dsLopTC, dsLop, rootMonHoc);
        }
        else if (luaChon == 1) {
            huyDangKyLopTinChi(dsLopTC);
        }
        else if (luaChon == 2) {
            char maMH[MAX_MAMH];
            char nienKhoa[MAX_NIENKHOA];
            int hocKy = 0;
            int nhom = 0;
            nhapThongTinTimLopTinChi(
                maMH,
                nienKhoa,
                hocKy,
                nhom
            );

            const int viTri = timLopTinChiTheoMonHoc(
                dsLopTC,
                maMH,
                nienKhoa,
                hocKy,
                nhom
            );

            if (viTri == -1) {
                cout << "Khong tim thay lop tin chi!\n";
            }
            else {
                inDanhSachSinhVienDangKy(
                    dsLop,
                    *dsLopTC.lopTC[viTri]
                );
            }
        }

        choNhanPhim();
    }
}
void menuChinh(
    PTRMH& root,
    DanhSachLop& dsLop,
    DanhSachLopTinChi& dsLopTC
) {
    const char* cacMuc[] = {
        "Quan ly mon hoc",
        "Quan ly lop va sinh vien",
        "Quan ly lop tin chi",
        "Dang ky lop tin chi",
        "Nhap diem",
        "Xem bang diem lop tin chi",
        "In bang diem trung binh",
        "In bang diem tong ket",
        "[F2] Luu du lieu",
        "Thoat"
    };

    using HanhDong = void (*)(
        PTRMH&,
        DanhSachLop&,
        DanhSachLopTinChi&
    );

    const HanhDong cacHanhDong[] = {
        [](PTRMH& rootMenu, DanhSachLop&, DanhSachLopTinChi& dsLopTCMenu) {
            menuMonHoc(rootMenu, dsLopTCMenu);
        },
        [](PTRMH&, DanhSachLop& dsLopMenu, DanhSachLopTinChi&) {
            menuLop(dsLopMenu);
        },
        [](PTRMH& rootMenu, DanhSachLop& dsLopMenu, DanhSachLopTinChi& dsLopTCMenu) {
            menuLopTinChi(dsLopTCMenu, dsLopMenu, rootMenu);
        },
        [](PTRMH& rootMenu, DanhSachLop& dsLopMenu, DanhSachLopTinChi& dsLopTCMenu) {
            menuDangKy(dsLopTCMenu, dsLopMenu, rootMenu);
        },
        [](PTRMH&, DanhSachLop& dsLopMenu, DanhSachLopTinChi& dsLopTCMenu) {
            if (!danhSachLopTinChiRong(dsLopTCMenu)) {
                nhapDiem(dsLopTCMenu, dsLopMenu);
            }
            choNhanPhim();
        },
        [](PTRMH& rootMenu, DanhSachLop& dsLopMenu, DanhSachLopTinChi& dsLopTCMenu) {
            if (!danhSachLopTinChiRong(dsLopTCMenu)) {
                xemBangDiemLopTinChi(dsLopTCMenu, dsLopMenu, rootMenu);
            }
            choNhanPhim();
        },
        [](PTRMH& rootMenu, DanhSachLop& dsLopMenu, DanhSachLopTinChi& dsLopTCMenu) {
            if (danhSachLopTinChiRong(dsLopTCMenu)) {
                choNhanPhim();
                return;
            }

            char maLop[MAX_MALOP];
            cout << "Nhap ma lop: ";
            nhapMaBatBuoc(maLop, MAX_MALOP);
            inBangDiemTrungBinh(dsLopMenu, dsLopTCMenu, rootMenu, maLop);
            choNhanPhim();
        },
        [](PTRMH&, DanhSachLop& dsLopMenu, DanhSachLopTinChi& dsLopTCMenu) {
            if (danhSachLopTinChiRong(dsLopTCMenu)) {
                choNhanPhim();
                return;
            }

            char maLop[MAX_MALOP];
            cout << "Nhap ma lop: ";
            nhapMaBatBuoc(maLop, MAX_MALOP);
            const int viTri = timLopTheoMa(dsLopMenu, maLop);

            if (viTri == -1) {
                cout << "Khong tim thay lop!\n";
            }
            else {
                inBangDiemTongKet(dsLopMenu.lop[viTri], dsLopTCMenu);
            }
            choNhanPhim();
        },
        [](PTRMH& rootMenu, DanhSachLop& dsLopMenu, DanhSachLopTinChi& dsLopTCMenu) {
            const bool daLuuMonHoc = ghiFileMonHoc(rootMenu);
            const bool daLuuLop = ghiFileLop(dsLopMenu);
            const bool daLuuLopTC = ghiFileLopTinChi(dsLopTCMenu);

            if (daLuuMonHoc && daLuuLop && daLuuLopTC) {
                cout << "Da luu tat ca du lieu thanh cong.\n";
            }
            else {
                cout << "Luu du lieu chua day du. Kiem tra quyen ghi va thu muc Data.\n";
            }
            choNhanPhim();
        },
        nullptr
    };

    const char* huongDan =
        "[LEN/XUONG]: Di chuyen  |  [ENTER]: Chon  |  [ESC]: Thoat";

    while (true) {
        const int luaChon = chonMenuConsole(
            "QUAN LY DIEM SINH VIEN THEO TIN CHI",
            cacMuc,
            static_cast<int>(sizeof(cacMuc) / sizeof(cacMuc[0])),
            huongDan
        );

        if (luaChon < 0 || cacHanhDong[luaChon] == nullptr) {
            break;
        }

        cacHanhDong[luaChon](root, dsLop, dsLopTC);
    }
}
