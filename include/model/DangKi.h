#pragma once

#include <string>
#include "model/HangSo.h"

struct DangKy {
    char maSV[MAX_MASV] = {};
    float diem = -1.0f;
    bool huyDangKy = false;
};

struct NodeDK {
    DangKy dk;
    NodeDK* next = nullptr;
};
typedef NodeDK* PTRDK;

DangKy taoDangKy(const std::string& maSV, float diem = -1.0f, bool huyDangKy = false);
bool themDangKy(PTRDK& dauDanhSach, const DangKy& dangKy);
NodeDK* timDangKy(PTRDK dauDanhSach, const std::string& maSV);
int demSVDangKyHieuLuc(PTRDK dauDanhSach);
int demTongDangKy(PTRDK dauDanhSach);
void giaiPhongDSDK(PTRDK& dauDanhSach);