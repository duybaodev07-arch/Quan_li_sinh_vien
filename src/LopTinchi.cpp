#include "lopTinChi.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <utility>

LopTinChi::LopTinChi(int ma, std::string maMon, std::string khoa, int ky, int nhomMon, int min, int max)
    : maLopTC(ma), maMH(std::move(maMon)), nienKhoa(std::move(khoa)), hocKy(ky), nhom(nhomMon), minSV(min), maxSV(max) {}
int LopTinChi::getMaLopTC() const { return maLopTC; }
const std::string& LopTinChi::getMaMH() const { return maMH; }
const std::string& LopTinChi::getNienKhoa() const { return nienKhoa; }
int LopTinChi::getHocKy() const { return hocKy; }
int LopTinChi::getNhom() const { return nhom; }
int LopTinChi::getMinSV() const { return minSV; }
int LopTinChi::getMaxSV() const { return maxSV; }
bool LopTinChi::getHuyLop() const { return huyLop; }
void LopTinChi::setMaMH(const std::string& ma) { maMH = ma; }
void LopTinChi::setNienKhoa(const std::string& khoa) { nienKhoa = khoa; }
void LopTinChi::setHocKy(int ky) { hocKy = ky; }
void LopTinChi::setNhom(int nhomMon) { nhom = nhomMon; }
void LopTinChi::setMinSV(int min) { minSV = min; }
void LopTinChi::setMaxSV(int max) { maxSV = max; }
void LopTinChi::setHuyLop(bool daHuy) { huyLop = daHuy; }
std::vector<DangKy>& LopTinChi::getDSSVDK() { return dssvdk; }
const std::vector<DangKy>& LopTinChi::getDSSVDK() const { return dssvdk; }
int LopTinChi::demSoSVDangKy() const {
    return static_cast<int>(std::count_if(dssvdk.begin(), dssvdk.end(), [](const DangKy& dk) { return !dk.getHuyDangKy(); }));
}
DangKy* LopTinChi::timDangKyTheoMaSV(const std::string& maSV) {
    for (auto& dk : dssvdk) if (dk.getMaSV() == maSV) return &dk;
    return nullptr;
}
const DangKy* LopTinChi::timDangKyTheoMaSV(const std::string& maSV) const {
    for (const auto& dk : dssvdk) if (dk.getMaSV() == maSV) return &dk;
    return nullptr;
}
bool LopTinChi::themDangKy(const std::string& maSV) {
    if (huyLop || demSoSVDangKy() >= maxSV) return false;
    DangKy* dk = timDangKyTheoMaSV(maSV);
    if (dk) { if (!dk->getHuyDangKy()) return false; dk->setHuyDangKy(false); dk->setDiem(-1.0); return true; }
    dssvdk.emplace_back(maSV); return true;
}
bool LopTinChi::huyDangKyTheoMaSV(const std::string& maSV) {
    DangKy* dk = timDangKyTheoMaSV(maSV);
    if (!dk || dk->getHuyDangKy()) return false;
    dk->setHuyDangKy(true); return true;
}

std::vector<LopTinChi>& DanhSachLopTinChi::getDanhSach() { return dsLopTC; }
const std::vector<LopTinChi>& DanhSachLopTinChi::getDanhSach() const { return dsLopTC; }
int DanhSachLopTinChi::taoMaLopTiepTheo() { return maLopTCTiepTheo++; }
LopTinChi* DanhSachLopTinChi::timLopTinChi(int ma) { for (auto& lop : dsLopTC) if (lop.getMaLopTC() == ma) return &lop; return nullptr; }
const LopTinChi* DanhSachLopTinChi::timLopTinChi(int ma) const { for (const auto& lop : dsLopTC) if (lop.getMaLopTC() == ma) return &lop; return nullptr; }
LopTinChi* DanhSachLopTinChi::timLopTinChiTheoMonHoc(const std::string& maMH, const std::string& khoa, int ky, int nhomMon) {
    for (auto& lop : dsLopTC) if (lop.getMaMH() == maMH && lop.getNienKhoa() == khoa && lop.getHocKy() == ky && lop.getNhom() == nhomMon) return &lop;
    return nullptr;
}
bool DanhSachLopTinChi::themLopTinChi(const LopTinChi& lopTC) {
    if (dsLopTC.size() >= 10000 || timLopTinChiTheoMonHoc(lopTC.getMaMH(), lopTC.getNienKhoa(), lopTC.getHocKy(), lopTC.getNhom())) return false;
    dsLopTC.push_back(lopTC);
    if (lopTC.getMaLopTC() >= maLopTCTiepTheo) maLopTCTiepTheo = lopTC.getMaLopTC() + 1;
    return true;
}
bool DanhSachLopTinChi::xoaLopTinChi(int ma) {
    auto it = std::find_if(dsLopTC.begin(), dsLopTC.end(), [ma](const LopTinChi& lop) { return lop.getMaLopTC() == ma; });
    if (it == dsLopTC.end()) return false;
    dsLopTC.erase(it); return true;
}
void inLopTinChi(const LopTinChi& lop) {
    std::cout << std::left << std::setw(8) << lop.getMaLopTC() << std::setw(13) << lop.getMaMH()
        << std::setw(13) << lop.getNienKhoa() << std::setw(8) << lop.getHocKy() << std::setw(8) << lop.getNhom()
        << std::setw(10) << lop.getMinSV() << std::setw(10) << lop.getMaxSV() << std::setw(12) << lop.demSoSVDangKy()
        << (lop.getHuyLop() ? "Da huy" : "Dang mo") << '\n';
}
void inDanhSachLopTinChi(const DanhSachLopTinChi& dsLopTC) {
    std::cout << std::left << std::setw(8) << "MA LTC" << std::setw(13) << "MA MH" << std::setw(13) << "NIEN KHOA"
        << std::setw(8) << "HOC KY" << std::setw(8) << "NHOM" << std::setw(10) << "MIN SV" << std::setw(10)
        << "MAX SV" << std::setw(12) << "DA DANG KY" << "TRANG THAI\n";
    for (const auto& lop : dsLopTC.getDanhSach()) inLopTinChi(lop);
}
