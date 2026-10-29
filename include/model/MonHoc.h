#pragma once

#include "model/HangSo.h"

struct MonHoc {
    char maMH[MAX_MAMH] = {};
    char tenMH[MAX_TENMH] = {};
    int soTCLT = 0;
    int soTCTH = 0;
};

struct NodeMH {
    MonHoc mh;
    NodeMH* left = nullptr;
    NodeMH* right = nullptr;
};
typedef NodeMH* PTRMH;

enum class KetQuaDocMonHoc {
    ThanhCong,
    KhongTimThayFile,
    KhongMoDuocFile,
    SaiDinhDang,
    MaMonHocTrung,
    TinChiNgoaiKhoang,
    LoiDocFile
};

MonHoc taoMonHoc(const char* maMH, const char* tenMH, int soTCLT, int soTCTH);
PTRMH timMonHoc(PTRMH root, const char* maMH);
bool themMonHoc(PTRMH& root, const MonHoc& monHoc);
void timNutThayThe(PTRMH& nutCanXoa, PTRMH& nutThayThe);
bool xoaMonHoc(PTRMH& root, const char* maMH);
int demMonHoc(PTRMH root);
void bstSangMang(PTRMH root, MonHoc* mangMonHoc, int& viTri);
void sapXepMonHocTheoTen(MonHoc* mangMonHoc, int soLuong);
void giaiPhongBST(PTRMH& root);
