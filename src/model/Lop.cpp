#include "model/Lop.h"

#include <cstring>

Lop taoLop(const char* maLop, const char* tenLop) {
    Lop lop{};
    std::strncpy(lop.maLop, maLop, MAX_MALOP - 1);
    std::strncpy(lop.tenLop, tenLop, MAX_TENLOP - 1);
    lop.dssv = nullptr;
    return lop;
}

int timLop(const DanhSachLop& danhSachLop, const char* maLop) {
    for (int i = 0; i < danhSachLop.soLuong; ++i) {
        if (std::strcmp(danhSachLop.lop[i].maLop, maLop) == 0) return i;
    }
    return -1;
}

NodeSV* timSinhVienToanTruong(const DanhSachLop& danhSachLop, const char* maSV, int* viTriLopTraVe) {
    for (int i = 0; i < danhSachLop.soLuong; ++i) {
        for (NodeSV* nut = danhSachLop.lop[i].dssv; nut != nullptr; nut = nut->next) {
            if (std::strcmp(nut->sv.maSV, maSV) == 0) {
                if (viTriLopTraVe != nullptr) *viTriLopTraVe = i;
                return nut;
            }
        }
    }
    if (viTriLopTraVe != nullptr) *viTriLopTraVe = -1;
    return nullptr;
}

void giaiPhongDanhSachLop(DanhSachLop& danhSachLop) {
    for (int i = 0; i < danhSachLop.soLuong; ++i) giaiPhongDSSV(danhSachLop.lop[i].dssv);
    danhSachLop.soLuong = 0;
}
