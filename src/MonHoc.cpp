#include "monHoc.h"
#include <algorithm>
#include <iomanip>
#include <iostream>

MonHoc::MonHoc(std::string ma, std::string ten, unsigned tclt, unsigned tcth)
    : maMH(std::move(ma)), tenMH(std::move(ten)), soTCLT(tclt), soTCTH(tcth) {}
const std::string& MonHoc::getMaMH() const { return maMH; }
const std::string& MonHoc::getTenMH() const { return tenMH; }
unsigned MonHoc::getSoTCLT() const { return soTCLT; }
unsigned MonHoc::getSoTCTH() const { return soTCTH; }
void MonHoc::setMaMH(const std::string& ma) { maMH = ma; }
void MonHoc::setTenMH(const std::string& ten) { tenMH = ten; }
void MonHoc::setSoTCLT(unsigned soTinChi) { soTCLT = soTinChi; }
void MonHoc::setSoTCTH(unsigned soTinChi) { soTCTH = soTinChi; }

bool themMonHoc(std::vector<MonHoc>& dsMonHoc, const MonHoc& mh) {
    if (timMonHocTheoMa(dsMonHoc, mh.getMaMH())) return false;
    dsMonHoc.push_back(mh);
    return true;
}
bool xoaMonHoc(std::vector<MonHoc>& dsMonHoc, const std::string& maMH) {
    auto it = std::find_if(dsMonHoc.begin(), dsMonHoc.end(), [&](const MonHoc& mh) { return mh.getMaMH() == maMH; });
    if (it == dsMonHoc.end()) return false;
    dsMonHoc.erase(it);
    return true;
}
MonHoc* timMonHocTheoMa(std::vector<MonHoc>& dsMonHoc, const std::string& maMH) {
    auto it = std::find_if(dsMonHoc.begin(), dsMonHoc.end(), [&](const MonHoc& mh) { return mh.getMaMH() == maMH; });
    return it == dsMonHoc.end() ? nullptr : &*it;
}
const MonHoc* timMonHocTheoMa(const std::vector<MonHoc>& dsMonHoc, const std::string& maMH) {
    auto it = std::find_if(dsMonHoc.begin(), dsMonHoc.end(), [&](const MonHoc& mh) { return mh.getMaMH() == maMH; });
    return it == dsMonHoc.end() ? nullptr : &*it;
}
void inMonHoc(const MonHoc& mh) {
    std::cout << std::left << std::setw(13) << mh.getMaMH() << std::setw(36) << mh.getTenMH()
              << std::setw(10) << mh.getSoTCLT() << mh.getSoTCTH() << '\n';
}
void inDanhSachMonHocTheoTen(const std::vector<MonHoc>& dsMonHoc) {
    auto ds = dsMonHoc;
    std::stable_sort(ds.begin(), ds.end(), [](const MonHoc& a, const MonHoc& b) {
        if (a.getTenMH() != b.getTenMH()) return a.getTenMH() < b.getTenMH();
        return a.getMaMH() < b.getMaMH();
    });
    std::cout << std::left << std::setw(13) << "MA MH" << std::setw(36) << "TEN MON HOC" << std::setw(10) << "TCLT" << "TCTH\n";
    for (const auto& mh : ds) inMonHoc(mh);
}
