#include "model/DangKi.h"

#include <cstring>

DangKy taoDangKy(const char* maSV, float diem, bool huyDangKy) {
    DangKy dangKy{};
    std::strncpy(dangKy.maSV, maSV, MAX_MASV - 1);
    dangKy.diem = diem;
    dangKy.huyDangKy = huyDangKy;
    return dangKy;
}

bool themDangKy(PTRDK& first, const DangKy& dangKy) {
    if (first == nullptr) {
        first = new NodeDK{dangKy, nullptr};
        return true;
    }
    NodeDK* nutHienTai = first;
    while (true) {
        if (std::strcmp(nutHienTai->dk.maSV, dangKy.maSV) == 0) {
            if (nutHienTai->dk.huyDangKy) {
                nutHienTai->dk.huyDangKy = false;
                return true;
            }
            return false;
        }
        if (nutHienTai->next == nullptr) break;
        nutHienTai = nutHienTai->next;
    }
    nutHienTai->next = new NodeDK{dangKy, nullptr};
    return true;
}

NodeDK* timDangKy(PTRDK first, const char* maSV) {
    for (NodeDK* nut = first; nut != nullptr; nut = nut->next) {
        if (std::strcmp(nut->dk.maSV, maSV) == 0) return nut;
    }
    return nullptr;
}

int demSVDangKyHieuLuc(PTRDK first) {
    int soLuong = 0;
    for (NodeDK* nut = first; nut != nullptr; nut = nut->next) {
        if (!nut->dk.huyDangKy) ++soLuong;
    }
    return soLuong;
}

int demTongDangKy(PTRDK first) {
    int soLuong = 0;
    for (NodeDK* nut = first; nut != nullptr; nut = nut->next) ++soLuong;
    return soLuong;
}

void giaiPhongDSDK(PTRDK& first) {
    while (first != nullptr) {
        NodeDK* nutXoa = first;
        first = first->next;
        delete nutXoa;
    }
}
