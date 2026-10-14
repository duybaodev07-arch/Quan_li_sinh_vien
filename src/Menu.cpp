#include "Menu.h"
#include "Input.h"
#include <cstdlib>
#include <iostream>

using namespace std;

void menuMonHoc(PTRMH& root) {
    int luaChon;

    do {
        system("cls");

        cout << "========== QUAN LY MON HOC ==========\n";
        cout << "1. Them mon hoc\n";
        cout << "2. Hieu chinh mon hoc\n";
        cout << "3. Xoa mon hoc\n";
        cout << "4. Tim mon hoc theo ma\n";
        cout << "5. In danh sach mon hoc theo ma\n";
        cout << "6. In danh sach mon hoc theo ten\n";
        cout << "0. Quay lai\n";
        cout << "=====================================\n";
        cout << "Nhap lua chon: ";

        luaChon = nhapSoNguyenTrongKhoang(0, 6);

        char maMH[11];

        switch (luaChon) {
        case 1:
            themMonHoc(root);
            system("pause");
            break;

        case 2:
            cout << "Nhap ma mon hoc can hieu chinh: ";
            nhapChuoi(maMH, 11);

            if (hieuChinhMonHoc(root, maMH))
                cout << "Hieu chinh mon hoc thanh cong!\n";
            else
                cout << "Khong tim thay mon hoc!\n";

            system("pause");
            break;

        case 3:
            cout << "Nhap ma mon hoc can xoa: ";
            nhapChuoi(maMH, 11);

            if (xoaMonHoc(root, maMH))
                cout << "Xoa mon hoc thanh cong!\n";
            else
                cout << "Khong tim thay mon hoc!\n";

            system("pause");
            break;

        case 4: {
            cout << "Nhap ma mon hoc can tim: ";
            nhapChuoi(maMH, 11);

            PTRMH p = timMonHocTheoMa(root, maMH);

            if (p != nullptr) {
                cout << "\nDa tim thay mon hoc!\n";
                inMonHoc(p->mh);
            }
            else
                cout << "\nKhong tim thay mon hoc!\n";

            system("pause");
            break;
        }

        case 5:
            inDanhSachMonHocTheoMa(root);
            system("pause");
            break;

        case 6:
            inDanhSachMonHocTheoTen(root);
            system("pause");
            break;

        case 0:
            break;
        }

    } while (luaChon != 0);
}
void menuLop(DanhSachLop& dsLop) {
    int luaChon;

    do {
        system("cls");

        cout << "========== QUAN LY LOP ==========\n";
        cout << " 1. Them lop\n";
        cout << " 2. Hieu chinh lop\n";
        cout << " 3. Xoa lop\n";
        cout << " 4. Tim lop theo ma\n";
        cout << " 5. In danh sach lop\n";
        cout << " 6. Them sinh vien vao lop\n";
        cout << " 7. Hieu chinh sinh vien\n";
        cout << " 8. Xoa sinh vien\n";
        cout << " 9. In sinh vien trong lop theo ten\n";
        cout << "10. In sinh vien trong lop theo ma\n";
        cout << " 0. Quay lai\n";
        cout << "=================================\n";
        cout << "Nhap lua chon: ";

        luaChon = nhapSoNguyenTrongKhoang(0, 10);

        char maLop[16];
        char maSV[20];

        switch (luaChon) {

        case 1:
            {
                Lop lop{};
                nhapLop(lop);
                if (themLop(dsLop, lop)) cout << "Them lop thanh cong!\n";
            }
            system("pause");
            break;

        case 2:
            cout << "Nhap ma lop can hieu chinh: ";
            nhapChuoi(maLop, 16);

            if (hieuChinhLop(dsLop, maLop))
                cout << "Hieu chinh lop thanh cong!\n";

            system("pause");
            break;

        case 3:
            cout << "Nhap ma lop can xoa: ";
            nhapChuoi(maLop, 16);

            if (xoaLop(dsLop, maLop))
                cout << "Xoa lop thanh cong!\n";

            system("pause");
            break;

        case 4: {
            cout << "Nhap ma lop can tim: ";
            nhapChuoi(maLop, 16);

            int viTri = timLopTheoMa(dsLop, maLop);

            if (viTri != -1)
                inLop(dsLop.lop[viTri]);
            else
                cout << "Khong tim thay lop!\n";

            system("pause");
            break;
        }

        case 5:
            inDanhSachLop(dsLop);
            system("pause");
            break;

        case 6: {
            cout << "Nhap ma lop: ";
            nhapChuoi(maLop, 16);

            int viTriLop = timLopTheoMa(dsLop, maLop);

            if (viTriLop == -1) {
                cout << "Khong tim thay lop!\n";
                system("pause");
                break;
            }

            SinhVien sv;
            if (nhapSinhVien(sv)) {
                PTRSV& dssv = dsLop.lop[viTriLop].dssv;
                if (daTonTaiSinhVien(dssv, sv.maSV)) cout << "Ma sinh vien da ton tai!\n";
                else {
                    themSinhVienVaoDSSV(dssv, taoNodeSinhVien(sv));
                    cout << "Them sinh vien thanh cong!\n";
                }
            }

            system("pause");
            break;
        }

        case 7: {
            cout << "Nhap ma lop: ";
            nhapChuoi(maLop, 16);

            int viTriLop = timLopTheoMa(dsLop, maLop);

            if (viTriLop == -1) {
                cout << "Khong tim thay lop!\n";
                system("pause");
                break;
            }

            cout << "Nhap ma sinh vien can hieu chinh: ";
            nhapChuoi(maSV, 20);

            if (hieuChinhSinhVien(dsLop.lop[viTriLop].dssv, maSV))
                cout << "Hieu chinh sinh vien thanh cong!\n";
            else
                cout << "Khong tim thay sinh vien!\n";

            system("pause");
            break;
        }

        case 8: {
            cout << "Nhap ma lop: ";
            nhapChuoi(maLop, 16);

            int viTriLop = timLopTheoMa(dsLop, maLop);

            if (viTriLop == -1) {
                cout << "Khong tim thay lop!\n";
                system("pause");
                break;
            }

            cout << "Nhap ma sinh vien can xoa: ";
            nhapChuoi(maSV, 20);

            if (xoaSinhVien(dsLop.lop[viTriLop].dssv, maSV))
                cout << "Xoa sinh vien thanh cong!\n";
            else
                cout << "Khong tim thay sinh vien!\n";

            system("pause");
            break;
        }

        case 9: {
            cout << "Nhap ma lop: ";
            nhapChuoi(maLop, 16);

            int viTriLop = timLopTheoMa(dsLop, maLop);

            if (viTriLop == -1) {
                cout << "Khong tim thay lop!\n";
                system("pause");
                break;
            }

            inDanhSachSinhVien(dsLop.lop[viTriLop].dssv);

            system("pause");
            break;
        }

        case 10: {
            cout << "Nhap ma lop: ";
            nhapChuoi(maLop, 16);

            int viTriLop = timLopTheoMa(dsLop, maLop);

            if (viTriLop == -1) {
                cout << "Khong tim thay lop!\n";
                system("pause");
                break;
            }

            inDanhSachSinhVienTheoMa(dsLop.lop[viTriLop].dssv);

            system("pause");
            break;
        }

        case 0:
            break;
        }

    } while (luaChon != 0);
}
void menuLopTinChi(
    DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH root
) {
    int luaChon;

    do {
        system("cls");

        cout << "========= QUAN LY LOP TIN CHI =========\n";
        cout << "1. Them lop tin chi\n";
        cout << "2. Hieu chinh lop tin chi\n";
        cout << "3. Xoa lop tin chi\n";
        cout << "4. Huy lop tin chi\n";
        cout << "5. In danh sach lop tin chi\n";
        cout << "6. Tu dong huy lop khong du sinh vien\n";
        cout << "7. Nhap diem\n";
        cout << "8. Xem bang diem lop tin chi\n";
        cout << "9. In bang diem trung binh\n";
        cout << "10. In bang diem tong ket\n";
        cout << "0. Quay lai\n";
        cout << "========================================\n";
        cout << "Nhap lua chon: ";

        luaChon = nhapSoNguyenTrongKhoang(0, 10);

        switch (luaChon) {

        case 1:
            themLopTinChiTuBanPhim(dsLopTC);
            system("pause");
            break;

        case 2: {
            int maLopTC;

            cout << "Nhap ma lop tin chi can hieu chinh: ";
            maLopTC = nhapSoNguyenKhongAm();

            if (hieuChinhLopTinChi(dsLopTC, maLopTC))
                cout << "Hieu chinh lop tin chi thanh cong!\n";
            else
                cout << "Khong tim thay lop tin chi!\n";

            system("pause");
            break;
        }

        case 3: {
            int maLopTC;

            cout << "Nhap ma lop tin chi can xoa: ";
            maLopTC = nhapSoNguyenKhongAm();

            if (xoaLopTinChi(dsLopTC, maLopTC))
                cout << "Xoa lop tin chi thanh cong!\n";
            else
                cout << "Khong tim thay lop tin chi!\n";

            system("pause");
            break;
        }

        case 4: {
            int maLopTC;

            cout << "Nhap ma lop tin chi can huy: ";
            maLopTC = nhapSoNguyenKhongAm();

            if (huyLopTinChi(dsLopTC, maLopTC))
                cout << "Huy lop tin chi thanh cong!\n";
            else
                cout << "Khong tim thay lop tin chi!\n";

            system("pause");
            break;
        }

        case 5:
            inDanhSachLopTinChi(dsLopTC);
            system("pause");
            break;

        case 6:
            tuDongHuyDanhSachLopKhongDuSinhVien(dsLopTC);
            system("pause");
            break;

        case 7:
            nhapDiem(dsLopTC, dsLop);
            system("pause");
            break;

        case 8:
            xemBangDiemLopTinChi(dsLopTC, dsLop);
            system("pause");
            break;

        case 9: {
            char maLop[16];

            cout << "Nhap ma lop: ";
            nhapChuoi(maLop, 16);

            inBangDiemTrungBinh(
                dsLop,
                dsLopTC,
                root,
                maLop
            );

            system("pause");
            break;
        }

        case 10: {
            char maLop[16];

            cout << "Nhap ma lop: ";
            nhapChuoi(maLop, 16);

            int viTri = timLopTheoMa(dsLop, maLop);

            if (viTri == -1) {
                cout << "Khong tim thay lop!\n";
            }
            else {
                inBangDiemTongKet(
                    dsLop.lop[viTri],
                    dsLopTC
                );
            }

            system("pause");
            break;
        }

        case 0:
            break;
        }

    } while (luaChon != 0);
}
void menuDangKy(
    DanhSachLopTinChi& dsLopTC,
    DanhSachLop& dsLop,
    PTRMH rootMonHoc
) {
    int luaChon;

    do {
        system("cls");

        cout << "========= DANG KY LOP TIN CHI =========\n";
        cout << "1. Dang ky lop tin chi\n";
        cout << "2. Huy dang ky lop tin chi\n";
        cout << "3. Xem danh sach sinh vien dang ky\n";
        cout << "0. Quay lai\n";
        cout << "=======================================\n";
        cout << "Nhap lua chon: ";

        luaChon = nhapSoNguyenTrongKhoang(0, 3);

        switch (luaChon) {

        case 1:
            dangKyLopTinChi(dsLopTC, dsLop, rootMonHoc);
            system("pause");
            break;

        case 2:
            huyDangKyLopTinChi(dsLopTC);
            system("pause");
            break;

        case 3: {
            char maMH[11];
            char nienKhoa[10];
            int hocKy;
            int nhom;

            nhapThongTinTimLopTinChi(maMH, nienKhoa, hocKy, nhom);
        int viTri = timLopTinChiTheoMonHoc(dsLopTC, maMH, nienKhoa, hocKy, nhom);

        if (viTri == -1) {
        cout << "Khong tim thay lop tin chi!\n";
        }
        else {
            inDanhSachSinhVienDangKy(dsLop, *dsLopTC.lopTC[viTri]);
        }

        system("pause");
        break;
    }
        case 0:
            break;
        }

    } while (luaChon != 0);
}

void menuChinh(
    PTRMH& root,
    DanhSachLop& dsLop,
    DanhSachLopTinChi& dsLopTC
) {
    int luaChon;

    do {
        system("cls");

        cout << "============= MENU CHINH =============\n";
        cout << "1. Quan ly mon hoc\n";
        cout << "2. Quan ly lop va sinh vien\n";
        cout << "3. Quan ly lop tin chi\n";
        cout << "4. Dang ky lop tin chi\n";
        cout << "5. Nhap diem\n";
        cout << "6. Xem bang diem lop tin chi\n";
        cout << "7. In bang diem trung binh\n";
        cout << "8. In bang diem tong ket\n";
        cout << "0. Thoat\n";
        cout << "=======================================\n";
        cout << "Nhap lua chon: ";

        luaChon = nhapSoNguyenTrongKhoang(0, 8);

        switch (luaChon) {
        case 1:
            menuMonHoc(root);
            break;

        case 2:
            menuLop(dsLop);
            break;

        case 3:
            menuLopTinChi(dsLopTC, dsLop, root);
            break;

        case 4:
            menuDangKy(dsLopTC, dsLop, root);
            break;

        case 5:
            nhapDiem(dsLopTC, dsLop);
            system("pause");
            break;

        case 6:
            xemBangDiemLopTinChi(dsLopTC, dsLop);
            system("pause");
            break;

        case 7: {
            char maLop[16];

            cout << "Nhap ma lop: ";
            nhapChuoi(maLop, 16);

            inBangDiemTrungBinh(
                dsLop,
                dsLopTC,
                root,
                maLop
            );

            system("pause");
            break;
        }

        case 8: {
            char maLop[16];

            cout << "Nhap ma lop: ";
            nhapChuoi(maLop, 16);

            int viTri = timLopTheoMa(dsLop, maLop);

            if (viTri == -1) {
                cout << "Khong tim thay lop!\n";
            }
            else {
                inBangDiemTongKet(
                    dsLop.lop[viTri],
                    dsLopTC
                );
            }
            system("pause");
            break;
        }

        case 0:
            break;
        }

    } while (luaChon != 0);
}
