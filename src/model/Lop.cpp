#include "model/Lop.h"

#include <cstring>

Lop taoLop(const std::string& maLop, const std::string& tenLop) {
    Lop lop{};
    std::strncpy(lop.maLop, maLop.c_str(), MAX_MALOP - 1);
    std::strncpy(lop.tenLop, tenLop.c_str(), MAX_TENLOP - 1);
    lop.dssv = nullptr;
    return lop;
}

int timLop(const DanhSachLop& danhSachLop, const std::string& maLop) {
    for (int i = 0; i < danhSachLop.soLuong; ++i) {
        if (std::strcmp(danhSachLop.lop[i].maLop, maLop.c_str()) == 0) return i;
    }
    return -1;
}

NodeSV* timSinhVienToanTruong(const DanhSachLop& danhSachLop, const std::string& maSV, int* viTriLopTraVe) {
    for (int i = 0; i < danhSachLop.soLuong; ++i) {
        for (NodeSV* nut = danhSachLop.lop[i].dssv; nut != nullptr; nut = nut->next) {
            if (std::strcmp(nut->sv.maSV, maSV.c_str()) == 0) {
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