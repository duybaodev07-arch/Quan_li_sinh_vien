#include "SinhVien.h"
#include "Input.h"
#include<iostream>
#include<fstream>
#include<cstring>
using namespace std;
void khoiTaoDanhSachSinhVien(PTRSV& root) {
    root = nullptr;
}
bool nhapSinhVien(SinhVien& sv) {
    
    cout << "Nhap ma sinh vien(rong de thoat): ";

    if(nhapMa(sv.maSV, MAX_MASV) == false) 
        return false; 

    cout << "Nhap ho: ";
    nhapChuoi(sv.ho, MAX_HO);

    cout << "Nhap ten: ";
    nhapChuoi(sv.ten, MAX_TEN);

    cout << "Nhap phai: ";
    nhapChuoi(sv.phai, MAX_PHAI);

    cout << "Nhap so dien thoai: ";
    nhapSoDienThoai(sv.soDT, MAX_SODT);
    
    return true;
}
void nhapThongTinHieuChinh(SinhVien& sv) {
    cout << "Nhap ho moi: ";
    nhapChuoi(sv.ho, MAX_HO);

    cout << "Nhap ten moi: ";
    nhapChuoi(sv.ten, MAX_TEN);

    cout << "Nhap phai moi: ";
    nhapChuoi(sv.phai, MAX_PHAI);

    cout << "Nhap so dien thoai moi: ";
    nhapSoDienThoai(sv.soDT, MAX_SODT);
}
PTRSV taoNodeSinhVien(const SinhVien& sv) {
    PTRSV p = new NodeSV;
    p->sv = sv;
    return p;
}
PTRSV timSinhVien(PTRSV head, const char* maSV) {
    PTRSV p = head;
    while (p != nullptr) {
        if (strcmp(p->sv.maSV, maSV) == 0) {
            return p;
        }
        p = p->next;
    }
    return nullptr;
}
bool daTonTaiSinhVien(PTRSV head, const char* maSV) {
    return timSinhVien(head, maSV) != nullptr;
}
bool sinhVienDungTruoc(const SinhVien& a, const SinhVien& b) {
    int ketQua = strcmp(a.ten, b.ten);
    // check sv1 có đứng trc sv2 ko
    if (ketQua != 0)
        return ketQua < 0;

    ketQua = strcmp(a.ho, b.ho);

    if (ketQua != 0)
        return ketQua < 0;

    return strcmp(a.maSV, b.maSV) < 0;
}
void themSinhVienVaoDSSV(PTRSV& head, PTRSV p) {

    if (head == nullptr || sinhVienDungTruoc(head->sv, p->sv)) {
        p->next = head;
        head = p;
        return;
    }

    PTRSV q = head;
    while (q->next != nullptr && !sinhVienDungTruoc(p->sv, q->next->sv)) {
        q = q->next;
    }
    p->next = q->next;
    q->next = p;
    return;
}
void themSinhVien(PTRSV& head) {
    SinhVien sv;
    while(true) {
        if(nhapSinhVien(sv) == false) {
            cout << "\nDa nhap xong danh sach sinh vien!\n";
            break;
        }
        if(daTonTaiSinhVien(head, sv.maSV) == true) {
            cout << "Ma sinh vien da ton tai!\n";
            cout << "Them that bai!\n\n";
            continue;
        }
        PTRSV p = taoNodeSinhVien(sv);

        themSinhVienVaoDSSV(head, p);
        cout << "Them sinh vien thanh cong!\n\n";
    }
}
bool xoaSinhVien(PTRSV& head, const char* maSV) {
    if (head == nullptr)
        return false;

    PTRSV p = head;
    PTRSV prev = nullptr;

    while (p != nullptr && strcmp(p->sv.maSV, maSV) != 0) {
        prev = p;
        p = p->next;
    }

    // Không tìm thấy
    if (p == nullptr)
        return false;

    // Xóa node đầu
    if (prev == nullptr) {
        head = p->next;
    }
    // Xóa node giữa hoặc cuối
    else {
        prev->next = p->next;
    }

    delete p;
    return true;
}
bool hieuChinhSinhVien(PTRSV& head, const char* maSV) {
    PTRSV p = timSinhVien(head, maSV);

    if (p == nullptr)
        return false;

    SinhVien sv = p->sv;

    nhapThongTinHieuChinh(sv);

    xoaSinhVien(head, maSV);

    PTRSV newNode = taoNodeSinhVien(sv);
    themSinhVienVaoDSSV(head, newNode);

    return true;
}
void inSinhVien(const SinhVien& sv) {
    cout << "Ma SV : " << sv.maSV << endl;
    cout << "Ho    : " << sv.ho << endl;
    cout << "Ten   : " << sv.ten << endl;
    cout << "Phai  : " << sv.phai << endl;
    cout << "So DT : " << sv.soDT << endl;
}   
void inDanhSachSinhVien(PTRSV head) {
    PTRSV p = head;

    while (p != nullptr) {
        inSinhVien(p->sv);
        if (p->next != nullptr)
            cout << "------------------------------" << endl;
        p = p->next;
    }
}
int demSoSinhVien(PTRSV head) {
    int soLuong = 0;

    for (PTRSV p = head; p != nullptr; p = p->next)
        soLuong++;

    return soLuong;
}
void sapXepTheoMaSinhVien(PTRSV pSV[], int soLuong) {
    for (int i = 0; i < soLuong - 1; i++) {
        int viTriNhoNhat = i;

        for (int j = i + 1; j < soLuong; j++) {
            if (strcmp(pSV[j]->sv.maSV,
                       pSV[viTriNhoNhat]->sv.maSV) < 0) {
                viTriNhoNhat = j;
            }
        }

        if (viTriNhoNhat != i) {
            PTRSV temp = pSV[i];
            pSV[i] = pSV[viTriNhoNhat];
            pSV[viTriNhoNhat] = temp;
        }
    }
}
void inDanhSachSinhVienTheoMa(PTRSV head) {
    if (head == nullptr) {
        cout << "Danh sach sinh vien rong!" << endl;
        return;
    }

    int soLuong = demSoSinhVien(head);

    PTRSV* pSV = new PTRSV[soLuong];

    int i = 0;
    for (PTRSV p = head; p != nullptr; p = p->next) {
        pSV[i] = p;
        i++;
    }

    sapXepTheoMaSinhVien(pSV, soLuong);

    for (i = 0; i < soLuong; i++) {
        inSinhVien(pSV[i]->sv);

        if (i < soLuong - 1)
            cout << "------------------------------" << endl;
    }

    delete[] pSV;
}
void xoaTatCaSinhVien(PTRSV& head) {
    while (head != nullptr) {
        PTRSV p = head;
        head = head->next;
        delete p;
    }
}
