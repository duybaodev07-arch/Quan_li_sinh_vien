#pragma once

#include <string>
#include "model/HangSo.h"

struct SinhVien {
    char maSV[MAX_MASV] = {};
    char ho[MAX_HO] = {};
    char ten[MAX_TEN] = {};
    char phai[MAX_PHAI] = {};
    char soDT[MAX_SODT] = {};
};

struct NodeSV {
    SinhVien sv;
    NodeSV* next = nullptr;
};
typedef NodeSV* PTRSV;

SinhVien taoSinhVien(const std::string& maSV, const std::string& ho, const std::string& ten, const std::string& phai, const std::string& soDT);
int soSanhTenSV(const SinhVien& sinhVienA, const SinhVien& sinhVienB);
void themSVCoThuTu(PTRSV& dauDanhSach, const SinhVien& sinhVien);
bool xoaSVTheoMa(PTRSV& dauDanhSach, const std::string& maSV);
int demSVTrongLop(PTRSV dauDanhSach);
void giaiPhongDSSV(PTRSV& dauDanhSach);