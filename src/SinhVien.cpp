#include "SinhVien.h"
#include "Input.h"
#include<iostream>
#include<fstream>
#include<cstring>
#include<iomanip>
#include<string>
using namespace std;
namespace {
void inKhungDuoiDanhSachSinhVien();

void inTieuDeDanhSachSinhVien() {
    cout << "+" << string(MAX_MASV + 2, '-')
         << "+" << string(MAX_HO + 2, '-')
         << "+" << string(MAX_TEN + 2, '-')
         << "+" << string(MAX_PHAI + 2, '-')
         << "+" << string(MAX_SODT + 2, '-') << "+\n";
    cout << "| " << left << setw(MAX_MASV) << "Ma SV"
         << " | " << left << setw(MAX_HO) << "Ho"
         << " | " << left << setw(MAX_TEN) << "Ten"
         << " | " << left << setw(MAX_PHAI) << "Phai"
         << " | " << left << setw(MAX_SODT) << "So dien thoai" << " |\n";
    inKhungDuoiDanhSachSinhVien();
}

void inKhungDuoiDanhSachSinhVien() {
    cout << "+" << string(MAX_MASV + 2, '-')
         << "+" << string(MAX_HO + 2, '-')
         << "+" << string(MAX_TEN + 2, '-')
         << "+" << string(MAX_PHAI + 2, '-')
         << "+" << string(MAX_SODT + 2, '-') << "+\n";
}

void inDongSinhVien(const SinhVien& sv) {
    cout << "| " << left << setw(MAX_MASV) << sv.maSV
         << " | " << left << setw(MAX_HO) << sv.ho
         << " | " << left << setw(MAX_TEN) << sv.ten
         << " | " << left << setw(MAX_PHAI) << sv.phai
         << " | " << left << setw(MAX_SODT) << sv.soDT << " |\n";
}
}
void khoiTaoDanhSachSinhVien(PTRSV& root) {
    root = nullptr;
}
bool nhapSinhVien(SinhVien& sv) {
    
    cout << "Nhap ma sinh vien(rong de thoat): ";

    if(nhapMa(sv.maSV, MAX_MASV) == false) 
        return false; 

    cout << "Nhap ho: ";
    nhapHoTenSinhVien(sv.ho, MAX_HO);

    cout << "Nhap ten: ";
    nhapHoTenSinhVien(sv.ten, MAX_TEN);

    cout << "Nhap phai (Nam/Nu): ";
    nhapPhai(sv.phai, MAX_PHAI);

    cout << "Nhap so dien thoai: ";
    nhapSoDienThoai(sv.soDT, MAX_SODT);
    
    return true;
}
void nhapThongTinHieuChinh(SinhVien& sv) {
    cout << "Nhap ho moi: ";
    nhapHoTenSinhVien(sv.ho, MAX_HO);

    cout << "Nhap ten moi: ";
    nhapHoTenSinhVien(sv.ten, MAX_TEN);

    cout << "Nhap phai moi (Nam/Nu): ";
    nhapPhai(sv.phai, MAX_PHAI);

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
    
    if (head == nullptr || sinhVienDungTruoc(p->sv, head->sv)) {
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
    inTieuDeDanhSachSinhVien();
    inDongSinhVien(sv);
    inKhungDuoiDanhSachSinhVien();
}

void inDanhSachSinhVienTheoTen(PTRSV head) {
    if (head == nullptr) {
        cout << "Danh sach sinh vien rong!\n";
        return;
    }

    inTieuDeDanhSachSinhVien();
    for (PTRSV p = head; p != nullptr; p = p->next)
        inDongSinhVien(p->sv);
    inKhungDuoiDanhSachSinhVien();
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
        cout << "Danh sach sinh vien rong!\n";
        return;
    }

    int soLuong = demSoSinhVien(head);
    PTRSV* pSV = new PTRSV[soLuong];

    int i = 0;
    for (PTRSV p = head; p != nullptr; p = p->next)
        pSV[i++] = p;

    sapXepTheoMaSinhVien(pSV, soLuong);

    inTieuDeDanhSachSinhVien();
    for (i = 0; i < soLuong; ++i)
        inDongSinhVien(pSV[i]->sv);
    inKhungDuoiDanhSachSinhVien();

    delete[] pSV;
}
void xoaTatCaSinhVien(PTRSV& head) {
    while (head != nullptr) {
        PTRSV p = head;
        head = head->next;
        delete p;
    }
}
