#pragma once

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

DangKy taoDangKy(const char* maSV, float diem = -1.0f, bool huyDangKy = false);
bool themDangKy(PTRDK& first, const DangKy& dangKy); // cap phat new o day
PTRDK timDangKy(PTRDK first, const char* maSV);
int demSVDangKyHieuLuc(PTRDK first);
int demTongDangKy(PTRDK first);
void giaiPhongDSDK(PTRDK& first);
