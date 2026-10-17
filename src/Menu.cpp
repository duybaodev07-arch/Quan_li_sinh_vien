#include "Menu.h"
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
}

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

        switch (luaChon) {
        case 1:
            themMonHoc(root);
            system("pause");
            break;

        case 2: {
            char maMH[MAX_MAMH];
            cout << "Nhap ma mon hoc can hieu chinh: ";
            nhapMaBatBuoc(maMH, MAX_MAMH);

            if (hieuChinhMonHoc(root, maMH))
                cout << "Hieu chinh mon hoc thanh cong!\n";
            else
                cout << "Khong tim thay mon hoc!\n";

            system("pause");
            break;
        }
        case 3: {
            char maMH[MAX_MAMH];
            cout << "Nhap ma mon hoc can xoa: ";
            nhapMaBatBuoc(maMH, MAX_MAMH);

            if (xoaMonHoc(root, maMH))
                cout << "Xoa mon hoc thanh cong!\n";
            else
                cout << "Khong tim thay mon hoc!\n";

            system("pause");
            break;
        }
        case 4: {
            char maMH[MAX_MAMH];
            cout << "Nhap ma mon hoc can tim: ";
            nhapMaBatBuoc(maMH, MAX_MAMH);

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

        case 5: {
            inDanhSachMonHocTheoMa(root);
            system("pause");
            break;
        }
        case 6: {
            inDanhSachMonHocTheoTen(root);
            system("pause");
            break;
        }
        case 0:
            break;
        }

    } while (luaChon != 0);
}
void menuSinhVien(DanhSachLop& dsLop, int viTri) {
    int luaChon;
    Lop& lop = dsLop.lop[viTri];
    do {
        system("cls");

        cout << "========== CAP NHAT SINH VIEN TRONG LOP ==========\n"
             << "1. Them sinh vien vao lop\n"
             << "2. Hieu chinh thong tin sinh vien\n"
             << "3. Xoa sinh vien khoi lop\n"
             << "4. In danh dach sinh vien theo ma\n"
             << "5. In danh sach sinh vien theo ten\n"
             << "6. Tim sinh vien\n"
             << "0. Quay lai\n"
             << "=================================\n"
             << "Nhap lua chon: ";
        luaChon = nhapSoNguyenTrongKhoang(0, 6);

        switch(luaChon) {
            case 1: {
                themSinhVienVaoLop(dsLop, viTri);
                cout << "\nKet thuc nhap!\n";
                system("pause");
                break;
            }
            case 2: {
                char maSV[MAX_MASV];
                cout << "Nhap ma sinh vien can hieu chinh: ";
                nhapMaBatBuoc(maSV, MAX_MASV);
                if(hieuChinhSinhVien(lop.dssv, maSV)) 
                    cout << "Hieu chinh thong tin sinh vien thanh cong!\n";
                else 
                    cout << "Khong tim thay ma sinh vien!\n";
                system("pause");
                break;
            }
            case 3: {
                char maSV[MAX_MASV];
                cout << "Nhap ma sinh vien can xoa: ";
                nhapMaBatBuoc(maSV, MAX_MASV);
                if(xoaSinhVien(lop.dssv, maSV)) 
                    cout << "Xoa sinh vien thanh cong!\n";
                else 
                    cout << "Khong tim thay ma sinh vien";
                system("pause");
                break;
            }
            case 4: {
                inDanhSachSinhVienTheoMa(lop.dssv);
                system("pause");
                break;
            }
            case 5: {
                inDanhSachSinhVienTheoTen(lop.dssv);
                system("pause");
                break;
            }
            case 6: {
                char maSV[MAX_MASV];
                cout << "Nhap ma sinh vien can tim: ";
                nhapMaBatBuoc(maSV, MAX_MASV);
                PTRSV found = timSinhVien(lop.dssv, maSV);

                if(found != nullptr)
                    inSinhVien(found->sv);
                else 
                    cout << "Khong tim thay ma sinh vien!\n";

                system("pause");
                break;
            }
            case 0: break;
        }
    } while(luaChon != 0);
}
void menuLop(DanhSachLop& dsLop) {
    int luaChon;

    do {
        system("cls");

        cout << "========== QUAN LY LOP ==========\n"
             << "1. Them lop\n"
             << "2. Hieu chinh lop\n"
             << "3. Xoa lop\n"
             << "4. Cap nhat sinh vien cua 1 lop\n"
             << "5. In danh sach lop\n"
             << "6. Tim thong tin sinh vien\n"
             << "0. Quay lai\n"
             << "=================================\n"
             << "Nhap lua chon: ";

        luaChon = nhapSoNguyenTrongKhoang(0, 6);

        switch(luaChon) {
            case 1: {
                themLop(dsLop);
                system("pause");
                break;
            }
            case 2: {
                char maLop[MAX_MALOP];
                
                cout << "Nhap ma lop can hieu chinh: ";
                nhapMaBatBuoc(maLop, MAX_MALOP);
                if(hieuChinhLop(dsLop, maLop)) 
                    cout << "Hieu chinh thanh cong!\n";
                else 
                    cout << "Khong tim thay ma lop!\n";

                system("pause");
                break;
            }
            case 3: {
                char maLop[MAX_MALOP];
                cout << "Nhap ma lop can xoa: ";
                nhapMaBatBuoc(maLop, MAX_MALOP);
                if(xoaLop(dsLop, maLop)) 
                    cout << "Xoa lop thanh cong!\n";
                else 
                    cout << "Khong tim thay ma lop!\n";

                system("pause");
                break;
            }
            case 4: {
                char maLop[MAX_MALOP];
                cout << "Nhap ma lop can cap nhat sinh vien: ";
                nhapMaBatBuoc(maLop, MAX_MALOP);

                int viTri = timLopTheoMa(dsLop, maLop);

                if(viTri != -1) {
                    cout << "Thong tin lop: ";
                    inLop(dsLop.lop[viTri]);
                    cout << "Ban co muon cap nhat sinh vien lop nay(Y/N)?\n";
                    if(nhapTrangThaiHuy()) {
                        menuSinhVien(dsLop, viTri);
                        cout << "Cap nhat sinh vien thanh cong!\n";
                        system("pause");
                    }
                    else {
                        cout<< "Thoat thanh cong!\n";
                        system("pause");
                    }
                }
                else 
                    cout << "Khong tim thay ma lop!\n";

                system("pause");
                break;
            }
            case 5: {
                inDanhSachLop(dsLop);
                system("pause");
                break;
            }
            case 6: {
                char maSV[MAX_MASV];
                cout << "Nhap ma sinh vien can tim: ";
                nhapMaBatBuoc(maSV, MAX_MASV);
                PTRSV found = timSinhVienTrongDanhSachLop(dsLop, maSV);

                if(found != nullptr) 
                    inSinhVien(found->sv);
                else 
                    cout << "Khong tim thay ma sinh vien!\n";

                system("pause");
                break;
            }
            case 0: break;
        }   
    } while(luaChon != 0);

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
            themLopTinChi(dsLopTC, root);
            system("pause");
            break;

        case 2: {
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            int maLopTC;

            cout << "Nhap ma lop tin chi can hieu chinh: ";
            maLopTC = nhapSoNguyenKhongAm();

            if (hieuChinhLopTinChi(dsLopTC, maLopTC, root))
                cout << "Hieu chinh lop tin chi thanh cong!\n";
            else
                cout << "Khong tim thay lop tin chi!\n";

            system("pause");
            break;
        }

        case 3: {
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
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
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
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
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            inDanhSachLopTinChi(dsLopTC);
            system("pause");
            break;

        case 6: {
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            char nienKhoa[MAX_NIENKHOA];
            cout << "Nhap nien khoa can xet huy: ";
            nhapNienKhoa(nienKhoa, MAX_NIENKHOA);
            cout << "Nhap hoc ky can xet huy: ";
            const int hocKy = nhapSoNguyenKhongAm();
            tuDongHuyDanhSachLopKhongDuSinhVien(dsLopTC, nienKhoa, hocKy);
            system("pause");
            break;
        }

        case 7:
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            nhapDiem(dsLopTC, dsLop);
            system("pause");
            break;

        case 8:
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            xemBangDiemLopTinChi(dsLopTC, dsLop, root);
            system("pause");
            break;

        case 9: {
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            char maLop[MAX_MALOP];

            cout << "Nhap ma lop: ";
            nhapMaBatBuoc(maLop, MAX_MALOP);

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
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            char maLop[MAX_MALOP];

            cout << "Nhap ma lop: ";
            nhapMaBatBuoc(maLop, MAX_MALOP);

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
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            dangKyLopTinChi(dsLopTC, dsLop, rootMonHoc);
            system("pause");
            break;

        case 2:
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            huyDangKyLopTinChi(dsLopTC);
            system("pause");
            break;

        case 3: {
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }

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
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            nhapDiem(dsLopTC, dsLop);
            system("pause");
            break;

        case 6: {
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            xemBangDiemLopTinChi(dsLopTC, dsLop, root);
            system("pause");
            break;
        }

        case 7: {
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            char maLop[MAX_MALOP];

            cout << "Nhap ma lop: ";
            nhapMaBatBuoc(maLop, MAX_MALOP);

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
            if (danhSachLopTinChiRong(dsLopTC)) {
                system("pause");
                break;
            }
            char maLop[MAX_MALOP];

            cout << "Nhap ma lop: ";
            nhapMaBatBuoc(maLop, MAX_MALOP);

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


