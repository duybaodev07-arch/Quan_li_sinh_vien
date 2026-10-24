#pragma once

#include <string>
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

MonHoc taoMonHoc(const std::string& maMH, const std::string& tenMH, int soTCLT, int soTCTH);
NodeMH* timMonHoc(PTRMH cayMonHoc, const char* maMH);
bool themMonHoc(PTRMH& cayMonHoc, const MonHoc& monHoc);
void timNutTheMang(PTRMH& nutCanXoa, PTRMH& nutThayThe);
bool xoaMonHoc(PTRMH& cayMonHoc, const char* maMH);
int demMonHoc(PTRMH cayMonHoc);
void bstSangMang(PTRMH cayMonHoc, MonHoc* mangMonHoc, int& viTri);
void sapXepMonHocTheoTen(MonHoc* mangMonHoc, int soLuong);
void giaiPhongBST(PTRMH& cayMonHoc);