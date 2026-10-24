#pragma once

#include "duLieu.h"

class HeThong {
private:
    std::vector<MonHoc> dsMonHoc;
    DanhSachLop dsLop;
    DanhSachLopTinChi dsLopTC;
    std::string thuMucDuLieu;
    bool tuDongLuu = true;
    void luuNeuCan();
    void menuMonHoc();
    void menuLop();
    void menuLopTinChi();
    void menuDangKy();
    LopTinChi* nhapThongTinTimLopTinChi();
    void nhapDiemLopTinChi(LopTinChi& lopTC);
    void inDanhSachDangKy(const LopTinChi& lopTC) const;
    void inBangDiemLopTinChi(const LopTinChi& lopTC) const;
    void inDiemTrungBinhTheoLop(const Lop& lop) const;
    void inBangDiemTongKetTheoLop(const Lop& lop) const;
public:
    explicit HeThong(std::string thuMuc);
    bool khoiDong();
    void chay();
};
