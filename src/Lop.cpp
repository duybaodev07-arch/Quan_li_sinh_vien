#include "lop.h"
#include <iostream>
#include <iomanip>
#include <utility>

Lop::Lop(std::string ma, std::string ten) : maLop(std::move(ma)), tenLop(std::move(ten)) {}
Lop::Lop(const Lop& other) : maLop(other.maLop), tenLop(other.tenLop) {
    for (PTRSV p = other.dssv; p; p = p->getNext()) themSinhVien(p->getSinhVien());
}
Lop& Lop::operator=(const Lop& other) {
    if (this == &other) return *this;
    Lop copy(other);
    *this = std::move(copy);
    return *this;
}
Lop::Lop(Lop&& other) noexcept : maLop(std::move(other.maLop)), tenLop(std::move(other.tenLop)), dssv(other.dssv) { other.dssv = nullptr; }
Lop& Lop::operator=(Lop&& other) noexcept {
    if (this != &other) { xoaTatCaSinhVien(dssv); maLop = std::move(other.maLop); tenLop = std::move(other.tenLop); dssv = other.dssv; other.dssv = nullptr; }
    return *this;
}
Lop::~Lop() { xoaTatCaSinhVien(dssv); }
const std::string& Lop::getMaLop() const { return maLop; }
const std::string& Lop::getTenLop() const { return tenLop; }
PTRSV Lop::getDSSV() const { return dssv; }
void Lop::setTenLop(const std::string& ten) { tenLop = ten; }
bool Lop::themSinhVien(const SinhVien& sv) {
    if (daTonTaiSinhVien(dssv, sv.getMaSV())) return false;
    themSinhVienVaoDSSV(dssv, new NodeSV(sv)); return true;
}
bool Lop::capNhatSinhVien(const std::string& maSV, const SinhVien& svMoi) {
    if (!daTonTaiSinhVien(dssv, maSV) || (maSV != svMoi.getMaSV() && daTonTaiSinhVien(dssv, svMoi.getMaSV()))) return false;
    ::xoaSinhVien(dssv, maSV); return themSinhVien(svMoi);
}
bool Lop::xoaSinhVien(const std::string& maSV) { return ::xoaSinhVien(dssv, maSV); }
std::vector<Lop>& DanhSachLop::getDanhSach() { return dsLop; }
const std::vector<Lop>& DanhSachLop::getDanhSach() const { return dsLop; }
Lop* DanhSachLop::timLopTheoMa(const std::string& ma) { for (auto& lop : dsLop) if (lop.getMaLop() == ma) return &lop; return nullptr; }
const Lop* DanhSachLop::timLopTheoMa(const std::string& ma) const { for (const auto& lop : dsLop) if (lop.getMaLop() == ma) return &lop; return nullptr; }
bool DanhSachLop::themLop(const Lop& lop) { if (timLopTheoMa(lop.getMaLop()) || dsLop.size() >= 10000) return false; dsLop.push_back(lop); return true; }
bool DanhSachLop::xoaLop(const std::string& ma) {
    auto& ds = dsLop;
    for (auto it = ds.begin(); it != ds.end(); ++it) if (it->getMaLop() == ma) { ds.erase(it); return true; }
    return false;
}
void inLop(const Lop& lop) { std::cout << lop.getMaLop() << " | " << lop.getTenLop() << " | " << demSoSinhVien(lop.getDSSV()) << " sinh vien\n"; }
void inDanhSachLop(const DanhSachLop& dsLop) { for (const auto& lop : dsLop.getDanhSach()) inLop(lop); }
