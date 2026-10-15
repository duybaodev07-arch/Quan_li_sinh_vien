#include "Lop.h"
#include "Input.h"
#include<fstream>
#include<iostream>
#include<cstring>
#include<limits>
using namespace std;

const char* DUONG_DAN_FILE_LOP = "data/LOP.txt";

void khoiTaoDanhSachLop(DanhSachLop& dsLop) {
    dsLop.soLuong = 0;
    for (int i = 0; i < MAX_LOP; ++i) {
        dsLop.lop[i].dssv = nullptr;
    }
}
void nhapLop(Lop& lop) {
    lop.dssv = nullptr;
    cout << "Nhap ma lop: ";
    nhapMaBatBuoc(lop.maLop, MAX_MALOP);

    cout << "Nhap ten lop: ";
    nhapChuoi(lop.tenLop, MAX_TENLOP);
}
int timLopTheoMa(const DanhSachLop& dsLop, const char* maLop) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        
        if (strcmp(dsLop.lop[i].maLop, maLop) == 0)
            return i;
    }

    return -1;
}
bool daTonTaiLop(const DanhSachLop& dsLop, const char* maLop) {
    return timLopTheoMa(dsLop, maLop) != -1;
}
PTRSV timSinhVienTrongDanhSachLop(const DanhSachLop& dsLop, const char* maSV) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        PTRSV cur = dsLop.lop[i].dssv;

        while (cur != nullptr) {
            if (strcmp(cur->sv.maSV, maSV) == 0)
                return cur;

            cur = cur->next;
        }
    }
    return nullptr;
}
bool themLopVaoDanhSachLop(DanhSachLop& dsLop, const Lop& lop) {
    if (dsLop.soLuong >= MAX_LOP) {
        cout << "Danh sach lop da day!\n";
        return false;
    }
    dsLop.lop[dsLop.soLuong] = lop;
    dsLop.soLuong++;

    return true;
}
bool themLop(DanhSachLop& dsLop, const Lop& lop) {

    if(daTonTaiLop(dsLop, lop.maLop) == true) {
        cout << "Ma lop da ton tai!\n";
        return false;
    }

    if(themLopVaoDanhSachLop(dsLop, lop) == false) 
        return false;

    return true;
}
bool hieuChinhLop(DanhSachLop& dsLop, const char* maLop) {
    int viTri = timLopTheoMa(dsLop, maLop);

    if (viTri == -1) {
        cout << "Khong tim thay ma lop!\n";
        return false;
    }

    cout << "Nhap ten lop moi: ";
    nhapChuoi(dsLop.lop[viTri].tenLop, MAX_TENLOP);

    return true;
}
bool xoaLop(DanhSachLop& dsLop, const char* maLop) {
    int viTri = timLopTheoMa(dsLop, maLop);

    if (viTri == -1) {
        cout << "Khong tim thay ma lop!\n";
        return false;
    }

    xoaTatCaSinhVien(dsLop.lop[viTri].dssv);

    for (int i = viTri; i < dsLop.soLuong - 1; i++) {
        dsLop.lop[i] = dsLop.lop[i + 1];
    }
    dsLop.soLuong--;
    dsLop.lop[dsLop.soLuong].dssv = nullptr;

    return true;
}
void inLop(const Lop& lop) {
    cout << "Ma lop : " << lop.maLop << endl;
    cout << "Ten lop: " << lop.tenLop << endl;
}
void inDanhSachLop(const DanhSachLop& dsLop) {
    if (dsLop.soLuong == 0) {
        cout << "Danh sach lop rong!\n";
        return;
    }

    for (int i = 0; i < dsLop.soLuong; i++) {
        cout << "Lop thu " << i + 1 << ":\n";
        inLop(dsLop.lop[i]);
        cout << "--------------------\n";
    }
}

bool ghiFileLop(const DanhSachLop& dsLop) {
    ofstream file(DUONG_DAN_FILE_LOP);
    if (!file) {
        cout << "Khong the mo file LOP.txt!\n";
        return false;
    }
    for (int i = 0; i < dsLop.soLuong; i++) {
        const Lop& lop = dsLop.lop[i];

        file << lop.maLop << "|";
        file << lop.tenLop << "\n";

        PTRSV p = lop.dssv;

        while (p != nullptr) {
            file << p->sv.maSV << "|";
            file << p->sv.ho << "|";
            file << p->sv.ten << "|";
            file << p->sv.phai << "|";
            file << p->sv.soDT << "\n";

            p = p->next;
        }

        file << "#\n";
    }

    file.close();
    return true;
}
void xoaTatCaLop(DanhSachLop& dsLop) {
    for (int i = 0; i < dsLop.soLuong; i++) {
        xoaTatCaSinhVien(dsLop.lop[i].dssv);
    }

    dsLop.soLuong = 0;
}
bool docFileLop(DanhSachLop& dsLop) {
    ifstream file(DUONG_DAN_FILE_LOP);
    if (!file) {
        cout << "Khong the mo file LOP.txt!\n";
        return false;
    }
    xoaTatCaLop(dsLop);
    while (true) {
        Lop lop{};

        if (!file.getline(lop.maLop, MAX_MALOP, '|'))
            break;
        chuanHoaMa(lop.maLop);

        if (!file.getline(lop.tenLop, MAX_TENLOP)) {
            cout << "File LOP.txt bi thieu ten lop!\n";
            return false;
        }

        while (true) {
            file >> ws;

            if (!file) {
                xoaTatCaSinhVien(lop.dssv);
                cout << "File LOP.txt bi thieu dau # ket thuc lop!\n";
                return false;
            }

            if (file.peek() == '#') {
                file.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );
                break;
            }

            char maSV[MAX_MASV];

            if (!file.getline(maSV, MAX_MASV, '|')) {
                xoaTatCaSinhVien(lop.dssv);
                cout << "File LOP.txt bi thieu ma sinh vien hoac dau #!\n";
                return false;
            }

            if (maSV[0] == '\0') {
                xoaTatCaSinhVien(lop.dssv);
                cout << "Ma sinh vien trong file khong duoc de trong!\n";
                return false;
            }

            SinhVien sv{};
            strcpy(sv.maSV, maSV);
            chuanHoaMa(sv.maSV);

            if (!file.getline(sv.ho, MAX_HO, '|') ||
                !file.getline(sv.ten, MAX_TEN, '|') ||
                !file.getline(sv.phai, MAX_PHAI, '|') ||
                !file.getline(sv.soDT, MAX_SODT)) {
                xoaTatCaSinhVien(lop.dssv);
                cout << "Thong tin sinh vien trong file khong day du!\n";
                return false;
            }

            PTRSV pSV = taoNodeSinhVien(sv);
            themSinhVienVaoDSSV(lop.dssv, pSV);
        }

        if (dsLop.soLuong >= MAX_LOP) {
            xoaTatCaSinhVien(lop.dssv);
            cout << "Danh sach lop trong file vuot qua gioi han!\n";
            file.close();
            return false;
        }
        dsLop.lop[dsLop.soLuong] = lop;
        dsLop.soLuong++;
    }
    file.close();
    return true;
}
