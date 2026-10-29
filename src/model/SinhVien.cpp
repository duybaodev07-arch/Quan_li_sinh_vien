#include "model/SinhVien.h"
#include "model/SoSanhChuoi.h"

#include <cstring>

SinhVien taoSinhVien(const char* maSV, const char* ho, const char* ten, const char* phai, const char* soDT) {
    SinhVien sv{};
    std::strncpy(sv.maSV, maSV, MAX_MASV - 1);
    std::strncpy(sv.ho, ho, MAX_HO - 1);
    std::strncpy(sv.ten, ten, MAX_TEN - 1);
    std::strncpy(sv.phai, phai, MAX_PHAI - 1);
    std::strncpy(sv.soDT, soDT, MAX_SODT - 1);
    return sv;
}

int soSanhTenSV(const SinhVien& sinhVienA, const SinhVien& sinhVienB) {
    int ketQua = soSanhKhongPhanBietHoaThuong(sinhVienA.ten, sinhVienB.ten);
    if (ketQua != 0) return ketQua;
    ketQua = soSanhKhongPhanBietHoaThuong(sinhVienA.ho, sinhVienB.ho);
    if (ketQua != 0) return ketQua;
    return soSanhKhongPhanBietHoaThuong(sinhVienA.maSV, sinhVienB.maSV);
}

void themSVCoThuTu(PTRSV& first, const SinhVien& sv) {
    NodeSV* newNode = new NodeSV{sv, nullptr};
    if (first == nullptr || soSanhTenSV(sv, first->sv) < 0) {
        newNode->next = first;
        first = newNode;
        return;
    }
    NodeSV* cur = first;
    while (cur->next != nullptr && soSanhTenSV(cur->next->sv, sv) <= 0) {
        cur = cur->next;
    }
    newNode->next = cur->next;
    cur->next = newNode;
}

bool xoaSVTheoMa(PTRSV& first, const char* maSV) {
    if (first == nullptr) return false;
    if (std::strcmp(first->sv.maSV, maSV) == 0) {
        NodeSV* nutXoa = first;
        first = first->next;
        delete nutXoa;
        return true;
    }
    NodeSV* cur = first;
    while (cur->next != nullptr && std::strcmp(cur->next->sv.maSV, maSV) != 0) {
        cur = cur->next;
    }
    if (cur->next == nullptr) return false;
    NodeSV* nutXoa = cur->next;
    cur->next = nutXoa->next;
    delete nutXoa;
    return true;
}

int demSVTrongLop(PTRSV first) {
    int soLuong = 0;
    for (NodeSV* nut = first; nut != nullptr; nut = nut->next) ++soLuong;
    return soLuong;
}

void giaiPhongDSSV(PTRSV& first) {
    while (first != nullptr) {
        NodeSV* nutXoa = first;
        first = first->next;
        delete nutXoa;
    }
}
