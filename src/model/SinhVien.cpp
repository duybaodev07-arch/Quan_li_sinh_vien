#include "model/SinhVien.h"

#include <cstring>

SinhVien taoSinhVien(const std::string& maSV, const std::string& ho, const std::string& ten, const std::string& phai, const std::string& soDT) {
    SinhVien sinhVien{};
    std::strncpy(sinhVien.maSV, maSV.c_str(), MAX_MASV - 1);
    std::strncpy(sinhVien.ho, ho.c_str(), MAX_HO - 1);
    std::strncpy(sinhVien.ten, ten.c_str(), MAX_TEN - 1);
    std::strncpy(sinhVien.phai, phai.c_str(), MAX_PHAI - 1);
    std::strncpy(sinhVien.soDT, soDT.c_str(), MAX_SODT - 1);
    return sinhVien;
}

int soSanhTenSV(const SinhVien& sinhVienA, const SinhVien& sinhVienB) {
    int ketQua = _stricmp(sinhVienA.ten, sinhVienB.ten);
    if (ketQua != 0) return ketQua;
    ketQua = _stricmp(sinhVienA.ho, sinhVienB.ho);
    if (ketQua != 0) return ketQua;
    return _stricmp(sinhVienA.maSV, sinhVienB.maSV);
}

void themSVCoThuTu(PTRSV& dauDanhSach, const SinhVien& sinhVien) {
    NodeSV* nutMoi = new NodeSV{sinhVien, nullptr};
    if (dauDanhSach == nullptr || soSanhTenSV(sinhVien, dauDanhSach->sv) < 0) {
        nutMoi->next = dauDanhSach;
        dauDanhSach = nutMoi;
        return;
    }
    NodeSV* nutHienTai = dauDanhSach;
    while (nutHienTai->next != nullptr && soSanhTenSV(nutHienTai->next->sv, sinhVien) <= 0) {
        nutHienTai = nutHienTai->next;
    }
    nutMoi->next = nutHienTai->next;
    nutHienTai->next = nutMoi;
}

bool xoaSVTheoMa(PTRSV& dauDanhSach, const std::string& maSV) {
    if (dauDanhSach == nullptr) return false;
    if (std::strcmp(dauDanhSach->sv.maSV, maSV.c_str()) == 0) {
        NodeSV* nutXoa = dauDanhSach;
        dauDanhSach = dauDanhSach->next;
        delete nutXoa;
        return true;
    }
    NodeSV* nutHienTai = dauDanhSach;
    while (nutHienTai->next != nullptr && std::strcmp(nutHienTai->next->sv.maSV, maSV.c_str()) != 0) {
        nutHienTai = nutHienTai->next;
    }
    if (nutHienTai->next == nullptr) return false;
    NodeSV* nutXoa = nutHienTai->next;
    nutHienTai->next = nutXoa->next;
    delete nutXoa;
    return true;
}

int demSVTrongLop(PTRSV dauDanhSach) {
    int soLuong = 0;
    for (NodeSV* nut = dauDanhSach; nut != nullptr; nut = nut->next) ++soLuong;
    return soLuong;
}

void giaiPhongDSSV(PTRSV& dauDanhSach) {
    while (dauDanhSach != nullptr) {
        NodeSV* nutXoa = dauDanhSach;
        dauDanhSach = dauDanhSach->next;
        delete nutXoa;
    }
}