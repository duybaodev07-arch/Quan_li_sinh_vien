#pragma once

#include "Const.h"

struct MonHoc {
    char maMH[MAX_MAMH];
    char tenMH[MAX_TENMH];
    int soTCLT;
    int soTCTH;
};

struct NodeMH {
    MonHoc mh;
    NodeMH* left = nullptr;
    NodeMH* right = nullptr;
};

typedef NodeMH* PTRMH;

void khoiTaoDanhSachMonHoc(PTRMH& root);
bool nhapMonHoc(MonHoc& mh);
void nhapThongTinHieuChinh(MonHoc& mh);
PTRMH taoNodeMonHoc(const MonHoc& mh);
PTRMH timMonHocTheoMa(PTRMH root, const char* maMH);
bool themMonHocVaoDSMH(PTRMH& root, const MonHoc& mh);
void themMonHoc(PTRMH& root);
bool hieuChinhMonHoc(PTRMH root, const char* maMH);
PTRMH timNodeNhoNhatBenPhai(PTRMH root);
bool xoaMonHoc(PTRMH& root, const char* maMH);
void inMonHoc(const MonHoc& mh);
void inDanhSachMonHocTheoMa(PTRMH root);
void demSoMonHoc(PTRMH root, int& n);
void duaMonHocVaoMang(PTRMH root, MonHoc mh[], int& n);
bool monHocDungTruoc(const MonHoc& a, const MonHoc& b);
void inDanhSachMonHocTheoTen(PTRMH root);
void giaiPhongMonHoc(PTRMH& root);
bool ghiFileMonHoc(PTRMH root);
bool docFileMonHoc(PTRMH& root);