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
// LƯU Ý: CÂY SẮP XẾP THEO MÃ, IN THEO TÊN CẦN XỬ LÍ RIÊNG

// KHỞI TẠO MÔN HỌC
void khoiTaoDanhSachMonHoc(PTRMH& root);

// TẠO NODE MÔN HỌC
PTRMH taoNodeMonHoc(const MonHoc& mh);

//TÌM MÔN HỌC THEO MÃ
PTRMH timMonHocTheoMa(PTRMH root, const char* maMH);

// NHẬP MÔN HỌC, NHẬP TT HIỆU CHỈNH, HÀM DÙNG CHO THÊM / HIỆU CHỈNH
bool nhapMonHoc(MonHoc& mh);
void nhapThongTinHieuChinh(MonHoc& mh);

// THÊM, XÓA, HIỆU CHỈNH MÔN HỌC
void themMonHoc(PTRMH& root);
bool xoaMonHoc(PTRMH& root, const char* maMH);
bool hieuChinhMonHoc(PTRMH root, const char* maMH);

// IN 1 MÔN HỌC & IN DANH SÁCH MÔN HỌC THEO MÃ / TÊN
void inMonHoc(const MonHoc& mh);
void inDanhSachMonHocTheoMa(PTRMH root);
void inDanhSachMonHocTheoTen(PTRMH root);

// ĐẾM SÀI ĐỆ QUY, CẦN TRUYỀN THAM CHIẾU ĐỂ ĐẾM
void demSoMonHoc(PTRMH root, int& n);

// GIẢI PHÓNG DANH SÁCH MÔN HỌC
void giaiPhongMonHoc(PTRMH& root);

// ĐỌC, GHI FILE
bool ghiFileMonHoc(PTRMH root);
enum class KetQuaDocMonHoc {
    ThanhCong,
    KhongTimThayFile,
    KhongMoDuocFile,
    SaiDinhDang,
    MaMonHocTrung,
    TinChiNgoaiKhoang,
    LoiDocFile
};

KetQuaDocMonHoc docFileMonHoc(PTRMH& root);