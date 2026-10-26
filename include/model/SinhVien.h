#pragma once

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

SinhVien taoSinhVien(const char* maSV, const char* ho, const char* ten, const char* phai, const char* soDT);
int soSanhTenSV(const SinhVien& sinhVienA, const SinhVien& sinhVienB);
void themSVCoThuTu(PTRSV& root, const SinhVien& sv);
bool xoaSVTheoMa(PTRSV& root, const char* maSV);
int demSVTrongLop(PTRSV root);
void giaiPhongDSSV(PTRSV& root);
