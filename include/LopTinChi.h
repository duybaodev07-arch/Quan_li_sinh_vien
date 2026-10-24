#pragma once

#include "dangKi.h"
#include <string>
#include <vector>

class LopTinChi {
private:
    int maLopTC = 0;
    std::string maMH;
    std::string nienKhoa;
    int hocKy = 0;
    int nhom = 0;
    int minSV = 0;
    int maxSV = 0;
    bool huyLop = false;
    std::vector<DangKy> dssvdk;
public:
    LopTinChi() = default;
    LopTinChi(int ma, std::string maMon, std::string khoa, int ky, int nhomMon, int min, int max);
    int getMaLopTC() const;
    const std::string& getMaMH() const;
    const std::string& getNienKhoa() const;
    int getHocKy() const;
    int getNhom() const;
    int getMinSV() const;
    int getMaxSV() const;
    bool getHuyLop() const;
    void setMaMH(const std::string& ma);
    void setNienKhoa(const std::string& khoa);
    void setHocKy(int ky);
    void setNhom(int nhomMon);
    void setMinSV(int min);
    void setMaxSV(int max);
    void setHuyLop(bool daHuy);
    std::vector<DangKy>& getDSSVDK();
    const std::vector<DangKy>& getDSSVDK() const;
    int demSoSVDangKy() const;
    DangKy* timDangKyTheoMaSV(const std::string& maSV);
    const DangKy* timDangKyTheoMaSV(const std::string& maSV) const;
    bool themDangKy(const std::string& maSV);
    bool huyDangKyTheoMaSV(const std::string& maSV);
};

class DanhSachLopTinChi {
private:
    std::vector<LopTinChi> dsLopTC;
    int maLopTCTiepTheo = 1;
public:
    std::vector<LopTinChi>& getDanhSach();
    const std::vector<LopTinChi>& getDanhSach() const;
    int taoMaLopTiepTheo();
    LopTinChi* timLopTinChi(int maLopTC);
    const LopTinChi* timLopTinChi(int maLopTC) const;
    LopTinChi* timLopTinChiTheoMonHoc(const std::string& maMH, const std::string& nienKhoa, int hocKy, int nhom);
    bool themLopTinChi(const LopTinChi& lopTC);
    bool xoaLopTinChi(int maLopTC);
};

void inLopTinChi(const LopTinChi& lopTC);
void inDanhSachLopTinChi(const DanhSachLopTinChi& dsLopTC);
